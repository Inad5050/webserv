#ifndef IHANDLERFACTORY_HPP
#define IHANDLERFACTORY_HPP

#include "libraries.hpp"
#include "handler/IRequestHandler.hpp"

class IHandlerFactory 
{
    public:
	    virtual IRequestHandler* createHandler() const = 0;
	    virtual ~IHandlerFactory() {}
};

#endif
