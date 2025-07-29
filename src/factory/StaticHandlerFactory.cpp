#include "factory/StaticHandlerFactory.hpp"
#include "handler/StaticFileHandler.hpp"
#include "response/IResponseBuilder.hpp"
#include "utils/Debug.hpp"
#include <iostream>

StaticHandlerFactory::StaticHandlerFactory(const std::string& rootDir, IResponseBuilder* builder, const ConfigParser& cfg): _rootDir(rootDir), _builder(builder), _cfg(cfg)
{
    (void) _cfg;
    debug << "[DEBUG][StaticHandlerFactory] created with rootsDir: " << rootDir << "\n";
}

StaticHandlerFactory::~StaticHandlerFactory()
{
    debug << "[DEBUG][StaticHandlerFactory] destroyed\n";
}

IRequestHandler* StaticHandlerFactory::createHandler() const
{
    return new StaticFileHandler(_rootDir, _builder, _cfg);
}
