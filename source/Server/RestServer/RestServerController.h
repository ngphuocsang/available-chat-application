#pragma once

#include "BaseController.h"

using namespace Server::RestServer::Controllers;

namespace Server::RestServer
{
    class RestServerController : public BaseController, HttpMethodController
    {
    public:
        RestServerController();
        ~RestServerController();

        void handleGet(http_request message) override;
        void handlePut(http_request message) override;
        void handlePost(http_request message) override;
        void handleDelete(http_request message) override;
        void initRestOpHandlers() override;

    private:
        static json::value responseNotImplement(const http::method &method);
    };
} // namespace Server::RestServer
