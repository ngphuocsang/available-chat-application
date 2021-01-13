#pragma once

#include <cpprest/http_msg.h>

using namespace web::http;

namespace Client::RestAPI
{
    class BaseRestAPI
    {
    protected:
        static http_request makeHttpRequest(method method);
    };
} // namespace Client::RestAPI