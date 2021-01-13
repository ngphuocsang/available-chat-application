#include "RestServerController.h"
#include "ServiceControllers.h"
#include "UserInterface.h"

using namespace MyLibrary;
using namespace Server::RestServer::Controllers;

namespace Server::RestServer
{
    RestServerController::RestServerController() {}
    RestServerController::~RestServerController() {}

    void RestServerController::initRestOpHandlers()
    {
        _listener.support(methods::GET, std::bind(&RestServerController::handleGet, this, std::placeholders::_1));
        _listener.support(methods::PUT, std::bind(&RestServerController::handlePut, this, std::placeholders::_1));
        _listener.support(methods::POST, std::bind(&RestServerController::handlePost, this, std::placeholders::_1));
        _listener.support(methods::DEL, std::bind(&RestServerController::handleDelete, this, std::placeholders::_1));
    }

    void RestServerController::handleGet(http_request message)
    {
        auto path = requestPath(message);
        if (!path.empty())
        {
            INFO_LOG("Calling API " + uri::decode(message.relative_uri().path()));
            cout << " from IP: " << message.remote_address() << endl;
            json::value response;
            if (path[0] == "ActiveAccounts")
            {
                response = AccountsController::getActiveAccounts(&message);
                message.reply(status_codes::OK, response);
            }
            if (path[0] == "Accounts")
            {
                response = AccountsController::getAllAccounts(&message);
                message.reply(status_codes::OK, response);
            }

            if (path[0] == "Groups")
            {
                response = GroupchatController::getAllGroups(&message);
                message.reply(status_codes::OK, response);
            }
            if (path[0] == "readGroup")
            {
                response = GroupchatController::getReadGroup(&message);
                message.reply(status_codes::OK, response);
            }
            cout << "[response]: " << response.serialize() << endl;
            return;
        }
        message.reply(status_codes::NotFound);
    }

    void RestServerController::handlePut(http_request message)
    {
        message.reply(status_codes::NotImplemented, responseNotImplement(methods::PUT));
    }

    void RestServerController::handlePost(http_request message)
    {
        auto path = requestPath(message);
        if (!path.empty())
        {
            INFO_LOG("Calling API " + uri::decode(message.relative_uri().path()));
            cout << " from IP: " << message.remote_address() << endl;
            json::value response;
            if (path[0] == "AuthenAccount")
            {
                response = AccountsController::postAuthenAccount(&message);
                message.reply(status_codes::OK, response);
            }
            if (path[0] == "LogoutAccount")
            {
                response = AccountsController::postLogoutAccount(&message);
                message.reply(status_codes::OK, response);
            }
            if (path[0] == "InviteGroup")
            {
                response = GroupchatController::postInviteGroup(&message);
                message.reply(status_codes::OK, response);
            }
            if (path[0] == "JoinGroup")
            {
                response = GroupchatController::postJoinGroup(&message);

                message.reply(status_codes::OK, response);
            }
            if (path[0] == "LeaveGroup")
            {
                response = GroupchatController::postLeaveGroup(&message);
                message.reply(status_codes::OK, response);
            }

            if (path[0] == "CreateGroup")
            {
                response =
                    GroupchatController::postCreateGroup(&message);
                message.reply(status_codes::OK, response);
            }
            if (path[0] == "DeleteGroup")
            {
                response =
                    GroupchatController::postDeleteGroup(&message);
                message.reply(status_codes::OK, response);
            }
            if (path[0] == "ModifyGroup")
            {
                response =
                    GroupchatController::postModifyGroup(&message);
                message.reply(status_codes::OK, response);
            }
            // if (path[0] == "JoinGroup")
            // {
            //     json::value response =
            //         GroupchatController::postJoinGroup(&message);
            //     message.reply(status_codes::OK, response);
            // }

            if (path[0] == "CreateAccount")
            {
                response = AccountsController::postCreateAccount(&message);
                message.reply(status_codes::OK, response);
            }
            if (path[0] == "DeleteAccount")
            {
                response = AccountsController::postDeleteAccount(&message);
                message.reply(status_codes::OK, response);
            }
            if (path[0] == "UpdateAccount")
            {
                response = AccountsController::postUpdateAccount(&message);
                message.reply(status_codes::OK, response);
            }
            cout << "[response]: " << response.serialize() << endl;
            return;
        }
        message.reply(status_codes::NotFound);
    }

    void RestServerController::handleDelete(http_request message)
    {
        message.reply(status_codes::NotImplemented, responseNotImplement(methods::PUT));
    }

    json::value RestServerController::responseNotImplement(const http::method &method)
    {
        auto response = json::value::object();
        response["serviceName"] = json::value::string("C++ Mircroservice Sample");
        response["http_method"] = json::value::string(method);
        return response;
    }
} // namespace Server::RestServer