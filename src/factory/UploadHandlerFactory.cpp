#include "../../include/factory/UploadHandlerFactory.hpp"
#include "../../include/handler/UploadHandler.hpp"
#include "../../include/utils/Debug.hpp"
#include <iostream>

UploadHandlerFactory::UploadHandlerFactory(const std::string& path, IResponseBuilder* builder, const ConfigParser& cfg) : _uploadsPath(path), _builder(builder), _cfg(cfg)
{
    (void) _cfg;
    debug << "[DEBUG][UploadHandlerFactory] created with uploads path: " << _uploadsPath << std::endl;
}

UploadHandlerFactory::~UploadHandlerFactory() 
{
    debug << "[DEBUG][UploadHandlerFactory] destroyed" << std::endl;
}

IRequestHandler* UploadHandlerFactory::createHandler() const {
    return new UploadHandler(_uploadsPath, _builder, _cfg);
}
