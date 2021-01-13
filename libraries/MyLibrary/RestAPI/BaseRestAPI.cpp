#include "BaseRestAPI.h"

using namespace web::http;

namespace Client::RestAPI
{
    http_request BaseRestAPI::makeHttpRequest(method method)
    {
        http_request req(method);

        return req;
    }
} // namespace Client::RestAPI