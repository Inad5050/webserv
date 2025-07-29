#include "factory/CGIHandlerFactory.hpp"
#include "handler/CGIHandler.hpp"
#include "response/IResponseBuilder.hpp"
#include "utils/Debug.hpp"

CGIHandlerFactory::CGIHandlerFactory(const std::string& cgiRoot, IResponseBuilder*  builder, const ConfigParser& cfg): _cgiRoot(cgiRoot), _builder(builder), _cfg(cfg)
{
    (void) _cfg;
    debug << std::cout << "[DEBUG][CGIHandlerFactory] created with uploads path: " << _cgiRoot << "\n";
}

CGIHandlerFactory::~CGIHandlerFactory()
{
    debug << std::cout << "[DEBUG][CGIHandlerFactory] destroyed\n";
}

IRequestHandler* CGIHandlerFactory::createHandler() const
{
    return new CGIHandler(_cgiRoot, _builder, _cfg);
}
