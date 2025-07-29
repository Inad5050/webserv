#include "handler/StaticFileHandler.hpp"
#include "response/IResponseBuilder.hpp"
#include "utils/AutoIndex.hpp"
#include "utils/ErrorPageHandler.hpp"
#include "utils/MimeTypes.hpp"
#include "utils/Utils.hpp"
#include "handler/UploadHandler.hpp"
#include "utils/Debug.hpp"
#include <fstream>
#include <iostream>
#include <sys/stat.h>

UploadHandler::UploadHandler(const std::string &uploadsPath, IResponseBuilder *builder,	const ConfigParser &cfg):
 _uploadsPath(uploadsPath), _builder(builder), _cfg(cfg)
{}

UploadHandler::~UploadHandler()
{}

Response UploadHandler::handleRequest(const Request &req)
{
	bool		autoindexFlag;
	Response	resAutoindex;
	Request		modifiedRequest;

    debug << "[DEBUG][UploadHandler][handleRequest] START" << std::endl;

	std::string method = req.getMethod();
	std::string originalUri = req.getPath();
	std::string uploadsPrefix = "/uploads";
	std::string relativePath = originalUri;
	if (relativePath.rfind(uploadsPrefix, 0) == 0)
		relativePath = relativePath.substr(uploadsPrefix.length());
	if (relativePath.empty() || relativePath[0] != '/')
		relativePath = "/" + relativePath;
	std::string fullPath = _uploadsPath + relativePath;

	if (method == "GET")
	{
		if (!checkCfgPermission(req, method))
		{
			std::cerr << "[ERROR][UploadHandler] 403 forbidden method" << std::endl; 
			return uploadResponse(req, 403, "Forbidden", "text/html", "");
		}
		
		debug << "[DEBUG][UploadHandler][autoindex] START" << std::endl;

		autoindexFlag = false;
		resAutoindex = AutoIndex::autoindex(autoindexFlag, originalUri,
				fullPath, req, _builder);
		if (autoindexFlag)
			return (resAutoindex);
	}

	if (method == "GET" || method == "DELETE")
	{
        debug << "[DEBUG][UploadHandler][handleRequest] staticHandler" << std::endl;

		if (!checkCfgPermission(req, method))
		{
			std::cerr << "[ERROR][UploadHandler] 403 forbidden method" << std::endl; 
			return uploadResponse(req, 403, "Forbidden", "text/html", "");
		}

		modifiedRequest = req;
		modifiedRequest.setPath(relativePath);
		modifiedRequest.setRedirection(true);
		
		StaticFileHandler staticHandler(_uploadsPath, _builder, _cfg);
		return (staticHandler.handleRequest(modifiedRequest));
	}

    if (method != "POST") 
	{
        std::cerr << "[ERROR][UploadHandler] 405 not a POST method" << std::endl; 
        return uploadResponse(req, 405, "Method Not Allowed", "text/html", "405 - Method Not Allowed");
    }

    if (!checkCfgPermission(req, "POST")) 
	{
        std::cerr << "[ERROR][UploadHandler] 403 forbidden method" << std::endl; 
        return uploadResponse(req, 403, "Forbidden", "text/html", "");
    }

	return (getBodyType(req));
}

Response UploadHandler::getBodyType(const Request &req)
{
    debug << "[DEBUG][UploadHandler][getBodyType]" << std::endl;

	std::string contentType = req.getHeader("content-type");
	if (contentType.empty())
		return (std::cerr << "[ERROR][UploadHandler] 400 empty content-type,contentType = " << contentType << std::endl, (uploadResponse(req,400,"Bad Request", "text/html", "")));
    
	debug << "[DEBUG][UploadHandler][getBodyType] Content-Type = " << contentType << std::endl;
	
	if (contentType.find("multipart/form-data") != std::string::npos)
		return (handleMultipartUpload(req, contentType));
	else if (contentType.find("application/x-www-form-urlencoded") != std::string::npos)
		return (handleUrlEncodedUpload(req));
	else if (contentType.find("image/jpeg") != std::string::npos
		|| contentType.find("image/png") != std::string::npos
		|| contentType.find("image/gif") != std::string::npos
		|| contentType.find("text/plain") != std::string::npos
		|| contentType.find("text/css") != std::string::npos
		|| contentType.find("text/csv") != std::string::npos
		|| contentType.find("text/html") != std::string::npos
		|| contentType.find("application/pdf") != std::string::npos
		|| contentType.find("application/zip") != std::string::npos
		|| contentType.find("video/mp4") != std::string::npos
		|| contentType.find("application/octet-stream") != std::string::npos)
		return (handleRawUpload(req));
	return uploadResponse(req ,415, "Unsupported Media Type", "text/html","");
}

Response UploadHandler::uploadResponse(const Request &req, int status, const std::string &reason, const std::string &mime, const std::string &body)
{
	Payload	payload;

	payload.status = status;
	payload.reason = reason;
	payload.mime = mime;
	if (body.empty())
	{
		ErrorPageHandler errorHandler(_uploadsPath);
		payload.body = errorHandler.render(req, status, reason);
	}
	else
	{
		payload.body = body;
	}
	payload.keepAlive = true;
	return (_builder->build(payload));
}

bool UploadHandler::checkCfgPermission(const Request &req, std::string method)
{
	ConfigParser	*cfg;
	size_t			serverIndex;

    debug << "[DEBUG][post][checkCfgPermission] START" << std::endl;

	cfg = req.getCfg();
	if (cfg == NULL)
		return (std::cerr << "[ERROR][post][checkCfgPermission] cannot get ConfigParser*", false);
	const std::vector<IConfig *> &serverNodes = cfg->getServerBlocks();
	if (serverNodes.empty())
		return (std::cerr << "[ERROR][post][checkCfgPermission] error ocheckCfgPermissionn  getServerBlocks",
			false);
	const std::string path = req.getPath();
	serverIndex = req.getServerIndex();

    debug << "[DEBUG][post][checkCfgPermission] serverIndex = " << serverIndex << std::endl;

	return (cfg->isMethodAllowed(serverNodes[serverIndex], path, method));
}
