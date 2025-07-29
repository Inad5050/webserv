#ifndef IREQUESTHANDLER_HPP
#define IREQUESTHANDLER_HPP

#include "libraries.hpp"
#include "core/Response.hpp"
#include "core/Request.hpp"

class IRequestHandler 
{
    public:
        virtual Response handleRequest(const Request& request) = 0;
        virtual ~IRequestHandler() {}
};

#endif
