#include "../../include/handler/CGIHandler.hpp"
#include "../../include/handler/StaticFileHandler.hpp"
#include "../../include/libraries.hpp"
#include "../../include/utils/ErrorPageHandler.hpp"
#include "../../include/utils/Utils.hpp"
#include "../../include/utils/AutoIndex.hpp"
#include "../../include/core/Request.hpp"
#include "../../include/utils/Debug.hpp"
#include <iostream>

int CGIHandler::identifyScriptType(const Request &req)
{
	debug << "[DEBUG][CGI][identifyScriptType] START" << std::endl;

	std::string uri = req.getURI();
	std::string path_part;
	std::string::size_type query_pos = uri.find('?');
	if (query_pos != std::string::npos)
		path_part = uri.substr(0, query_pos);
	else
		path_part = uri;
	if (path_part.length() > 3 && path_part.substr(path_part.length() - 3) == ".py")
    {
		debug << "[DEBUG][CGI][identifyScriptType]: python" << std::endl;
        return (2);
    }
	if (path_part.length() > 3 && path_part.substr(path_part.length() - 3) == ".sh")
    {
		debug << "[DEBUG][CGI][identifyScriptType]: shell" << std::endl;
        return (4);
    }
	
    debug << "[DEBUG][CGI][identifyScriptType] not a valid CGI" << std::endl;
    return (0);
}

int CGIHandler::identifyMethod(const Request &req)
{
	debug << "[DEBUG][CGI][identifyMethod] START" << std::endl;
	
	std::string method = req.getMethod();

	try
	{
		if (method == "GET")
		{
			debug << "[DEBUG][CGI][identifyMethod]: GET" << std::endl;
			checkCfgPermission(req, "GET");
			return (1);
		}
		else if (method == "POST")
		{   
            debug << "[DEBUG][CGI][identifyMethod]: POST" << std::endl;
			checkCfgPermission(req, "POST");
			return (2);
		}
	}
	catch (const std::exception& e)
	{
        debug << "[ERROR][CGI][identifyMethod]: " << e.what() << std::endl;
		return (0);
	}
	return (0);
}

Response CGIHandler::autoindexCGIAux(const Request &req)
{
	debug << "[DEBUG][CGI][autoindexCGIAux] START" << std::endl;
	
	try
	{
		checkCfgPermission(req, req.getMethod());
	}
	catch (const std::exception& e)
	{
		debug << "[ERROR][CGI][autoindexCGIAux][autoindexCGIAux]: " << e.what() << std::endl;
		return (CGIerror(req, 403, " Forbidden", "text/html"));
	}

	std::string uri = req.getURI();
	std::string fullPath = _cgiRoot;
	std::string pathPart = req.getPath();
	if (pathPart.length() > std::string("/cgi-bin").length())
		fullPath += pathPart.substr(std::string("/cgi-bin").length());

	bool autoindexFlag = false;
	Response res = AutoIndex::autoindex(autoindexFlag, uri, fullPath, req, _builder);
	if (autoindexFlag)
		return (res);

	debug << "[DEBUG][CGI][autoindexCGIAux] staticHandler called" << std::endl;

	std::string cgiPrefix = "/cgi-bin";
    std::string relativePath = req.getPath();

	if (relativePath.rfind(cgiPrefix, 0) == 0)
		relativePath = relativePath.substr(cgiPrefix.length());

	if (relativePath.empty() || relativePath[0] != '/')
		relativePath = "/" + relativePath;

	Request modifiedRequest = req;
	modifiedRequest.setPath(relativePath);
	modifiedRequest.setRedirection(true);

	StaticFileHandler staticHandler(_cgiRoot, _builder, _cfg);
	res = staticHandler.handleRequest(modifiedRequest);
	return (res);
}

void CGIHandler::checkCfgPermission(const Request &req, std::string method)
{
    debug << "[DEBUG][CGI][checkCfgPermission] START" << std::endl;
	ConfigParser *cfg = req.getCfg();
	if (cfg == NULL)
		throw (std::runtime_error("cannot get ConfigParser*"));
	
	const std::vector<IConfig*>& serverNodes = cfg->getServerBlocks();
	if (serverNodes.empty())
		throw (std::runtime_error("error on getServerBlocks"));

	const std::string path = req.getPath();

	size_t serverIndex = req.getServerIndex();
    debug << "[DEBUG][CGI][checkCfgPermission] serverIndex = " << serverIndex << std::endl;
	bool allowed = cfg->isMethodAllowed(serverNodes[serverIndex], path, method);
	if (!allowed)
		throw (std::runtime_error(method + " not allowed"));
	return;
}

Response CGIHandler::getScript(const Request &req, std::map<std::string, std::string> &map)
{
    debug << "[DEBUG][CGI][getScript] START" << std::endl;
	std::string uri = req.getURI();
	if (uri.empty())
		return (std::cerr << "[ERROR][getURI] CGI couldn't get uri" << std::endl, \
		CGIerror(req ,404, "Bad Request", "text/html"));
		
	std::string scriptName = getScriptName(uri);
	if (scriptName.empty())
		return (std::cerr << "[ERROR][getScriptName] CGI couldn't get ScriptName" << std::endl, \
		CGIerror(req ,404, "Bad Request", "text/html"));

	if (checkScriptAccess(_cgiRoot, scriptName))
		return (CGIerror(req ,404, "Bad Request", "text/html"));

	map["dir"] = _cgiRoot + "/";
	map["name"] = scriptName;
	map["path"] = map["dir"] + map["name"];
	map["queryString"] = getScriptQuery(uri);
    debug << "[DEBUG][CGI][getScript] path = " << map["path"] << std::endl;
	debug << "[DEBUG][CGI][getScript] query = " << map["queryString"] << std::endl;
	return (_resDefault);
}

std::string CGIHandler::getScriptName(const std::string &uri)
{
	debug << "[DEBUG][CGI][getScriptName] START" << std::endl;

	std::string path;
	std::string::size_type query_pos = uri.find('?');
	if (query_pos != std::string::npos)
		path = uri.substr(0, query_pos);
	else
		path = uri;

	std::string::size_type pos_name = path.rfind("/");
	if (pos_name == std::string::npos)
		return (path);
	else if (pos_name == path.length() - 1)
		return ("");

	return (path.substr(pos_name + 1));
}

std::string CGIHandler::getScriptQuery(const std::string &uri)
{
    debug << "[DEBUG][CGI][getScriptQuery] START" << std::endl;
	std::string::size_type pos_query = uri.find("?");
	if (pos_query == std::string::npos)
		return ("");
	return (uri.substr(pos_query + 1));
}

int CGIHandler::checkScriptAccess(std::string &dir, std::string &scriptName)
{
    debug << "[DEBUG][CGI][checkScriptAccess] START" << std::endl;
    std::string fullPath = dir + "/" + scriptName;

	debug << "[DEBUG][CGI][checkScriptAccess] fullPath = " << fullPath << std::endl;

    if (access(fullPath.c_str(), F_OK) == -1) 
        return (std::cerr << "[ERROR][CGI][checkScriptAccess] couldn't find script: " << \
		fullPath << " — " << strerror(errno) << std::endl, 1);

    if (access(fullPath.c_str(), X_OK) == -1)
		return (std::cerr << "[ERROR][CGI][checkScriptAccess] can't execute script: " << \
		fullPath << " — " << strerror(errno) << std::endl, 1);
    return (0);
}

bool CGIHandler::getEnviroment(std::vector<std::string> &env, std::string method, std::string path, std::string queryString, const Request &req)
{
    debug << "[DEBUG][CGI][enviromentGET] START" << std::endl;
	env.push_back("PATH_INFO=" + path);
	env.push_back("QUERY_STRING=" + queryString);
	env.push_back("SCRIPT_NAME=" + path);
	env.push_back("SERVER_PROTOCOL=HTTP/1.1");

	if (method == "GET")
	{
		env.push_back("REQUEST_METHOD=GET");
		env.push_back("CONTENT_LENGTH=0");
	}
	else if (method == "POST")
	{
		env.push_back("REQUEST_METHOD=POST");
		std::stringstream body_lenght;
		body_lenght << req.getBody().length();
		env.push_back("CONTENT_LENGTH=" + body_lenght.str());
		env.push_back("CONTENT_TYPE=" + req.getHeader("Content-Type"));
	}
	else
	{
		std::cerr << "[ERROR][CGI][getEnviroment] invalid method: " << method << std::endl;
		return (false);
	}
	return (true);
}
