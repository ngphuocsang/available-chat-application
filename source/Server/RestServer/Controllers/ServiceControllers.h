#pragma once

#include <pplx/pplxtasks.h>
#include <cpprest/json.h>
#include <cpprest/http_msg.h>

using namespace web;
using namespace http;

namespace Server::RestServer::Controllers
{
    class AccountsController
    {
    public:
        static json::value getActiveAccounts(http_request *message);
        static json::value postAuthenAccount(http_request *message);
        static json::value getAllAccounts(http_request *message);
        static json::value postCreateAccount(http_request *message);
        static json::value postUpdateAccount(http_request *message);
        static json::value postDeleteAccount(http_request *message);
        static json::value postLogoutAccount(http_request *message);
    };

    class GroupchatController
    {
    public:
        static json::value getAllGroups(http_request *message);
        static json::value postInviteGroup(http_request *message);
        static json::value postJoinGroup(http_request *message);
        static json::value postLeaveGroup(http_request *message);
        static json::value postCreateGroup(http_request *message);
        static json::value postDeleteGroup(http_request *message);
        static json::value postModifyGroup(http_request *message);
        static json::value getReadGroup(http_request *message);

    };
} // namespace Server::RestServer::Controllers
