#ifndef CGIHANDLERFACTORY_HPP
#define CGIHANDLERFACTORY_HPP

#include "libraries.hpp"
#include "factory/CGIHandlerFactory.hpp"
#include "factory/IHandlerFactory.hpp"
#include "config/ConfigParser.hpp"
#include "handler/IRequestHandler.hpp"

class IResponseBuilder;

class CGIHandlerFactory : public IHandlerFactory 
{
    private:
        std::string _cgiRoot;
        IResponseBuilder* _builder;
        const ConfigParser& _cfg;

    public:
        CGIHandlerFactory(const std::string& cgiRoot, IResponseBuilder* builder, const ConfigParser& cfg);
        virtual	~CGIHandlerFactory();
        virtual	IRequestHandler* createHandler() const;
};

#endif
