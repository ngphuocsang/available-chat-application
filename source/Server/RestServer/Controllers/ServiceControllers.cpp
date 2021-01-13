#include "ServiceControllers.h"
#include "RequestsAndResponses.h"
#include "AccountsService.h"
#include "GroupchatService.h"

#include <string>

using namespace MyLibrary::Models::Responses;
using namespace MyLibrary::Models::Requests;
using namespace Server::RestServer::Services;

namespace Server::RestServer::Controllers
{
    json::value AccountsController::getActiveAccounts(
        http_request *message)
    {
        string key = message->headers()["Authorization"];
        ActiveAccountsRequest req(key);
        ActiveAccountsResponse res = AccountsService::getActiveAccounts(req);
        json::value jsonObj = json::value::object();
        jsonObj["result"] = json::value::number(res.Result);
        jsonObj["usernames"] = json::value::array();
        int i = 0;
        for (auto item : res.Usernames)
            jsonObj["usernames"][i++] = json::value::string(item);
        return jsonObj;
    }
    json::value AccountsController::postAuthenAccount(
        http_request *message)
    {
        json::value jsonObj = json::value::object();
        message->extract_json()
            .then([&jsonObj](json::value json) {
                string username = json["username"].as_string();
                string password = json["password"].as_string();
                AuthenAccountRequest req(username, password);
                auto res = AccountsService::postAuthenAccount(req);
                jsonObj["result"] = json::value::number(res.Result);
                jsonObj["authKey"] = json::value::string(res.AuthKey);
                jsonObj["role"] = json::value::string(res.Role);
            })
            .wait();
        return jsonObj;
    }
    json::value AccountsController::postLogoutAccount(
        http_request *message)
    {
        json::value jsonObj = json::value::object();
        message->extract_json()
            .then([&jsonObj](json::value json) {
                string username = json["username"].as_string();
                LogoutAccountRequest req(username);
                auto res = AccountsService::postLogoutAccount(req);
                jsonObj["result"] = json::value::number(res);
            })
            .wait();
        return jsonObj;
    }

    json::value AccountsController::getAllAccounts(
        http_request *message)
    {
        string key = message->headers()["Authorization"];
        AccountsRequest req(key);
        AccountsResponse res = AccountsService::getAllAccounts(req);
        json::value jsonObj = json::value::object();
        jsonObj["result"] = json::value::number(res.Result);
        jsonObj["accounts"] = json::value::array();
        int i = 0;
        for (auto item : res.Accounts)
        {
            jsonObj["accounts"][i]["username"] = json::value::string(item.Username);
            jsonObj["accounts"][i++]["role"] = json::value::string(item.Role);
        }
        return jsonObj;
    }

    json::value AccountsController::postCreateAccount(http_request *message)
    {
        json::value jsonObj = json::value::object();
        string key = message->headers()["Authorization"];

        message->extract_json()
            .then([&jsonObj, &key](json::value json) {
                CreateAccountRequest req(key);
                req.Username = json["username"].as_string();
                req.Password = json["password"].as_string();
                req.Role = json["role"].as_string();
                auto res = AccountsService::postCreateAccount(req);
                jsonObj["result"] = json::value::number(res);
            })
            .wait();

        return jsonObj;
    }
    json::value AccountsController::postUpdateAccount(http_request *message)
    {
        json::value jsonObj = json::value::object();
        string key = message->headers()["Authorization"];

        message->extract_json()
            .then([&jsonObj, &key](json::value json) {
                UpdateAccountRequest req(key);
                req.Username = json["username"].as_string();
                req.Password = json["password"].as_string();
                req.Role = json["role"].as_string();
                auto res = AccountsService::postUpdateAccount(req);
                jsonObj["result"] = json::value::number(res);
            })
            .wait();

        return jsonObj;
    }
    json::value AccountsController::postDeleteAccount(http_request *message)
    {
        json::value jsonObj = json::value::object();
        string key = message->headers()["Authorization"];

        message->extract_json()
            .then([&jsonObj, &key](json::value json) {
                DeleteAccountRequest req(key);
                req.Username = json["username"].as_string();
                auto res = AccountsService::postDeleteAccount(req);
                jsonObj["result"] = json::value::number(res);
            })
            .wait();

        return jsonObj;
    }

    json::value GroupchatController::getAllGroups(http_request *message)
    {
        string key = message->headers()["Authorization"];
        GroupsRequest req(key);

        GroupsResponse res = GroupchatService::getAllGroups(req);

        json::value jsonObj = json::value::object();
        jsonObj["Result"] = json::value::number(res.Result);
        jsonObj["Groups"] = json::value::array();
        int k = 0;
        if (res.Groups.size() > 0)
        {
            for (auto i : res.Groups)
            {
                json::value singleGroup;
                singleGroup["Id"] = json::value::number(i.Id);
                singleGroup["Name"] = json::value::string(i.Name);
                singleGroup["Access"] = json::value::number(i.Access);
                singleGroup["Owner"] = json::value::string(i.Owner);

                jsonObj["Groups"][k++] = singleGroup;
            }
        }

        return jsonObj;
        // GroupsResponse res= Groustatic json::value getAllGroups(http_request *message);
    }

    json::value GroupchatController::postJoinGroup(http_request *message)
    {
        json::value jsonObj = json::value::object();
        message->extract_json()
            .then([&jsonObj](json::value json) {
                unsigned int groupID = json["groupid"].as_integer();
                string Password = json["password"].as_string();
                string ClientName = json["clientName"].as_string();
                string key = json["Authorization"].as_string();
                JoinGroupRequest req(key, groupID, Password, ClientName);
                auto res = GroupchatService::JoinGroup(req);
                jsonObj["Result"] = json::value::number(res.Result);
                jsonObj["Status"] = json::value::string(res.Status);
            })
            .wait();
        return jsonObj;
    }
    json::value GroupchatController::postInviteGroup(http_request *message)
    {
        json::value jsonObj = json::value::object();
        message->extract_json()
            .then([&jsonObj](json::value json) {
                unsigned int groupID = json["groupid"].as_integer();
                json::array Jsonclients = json["clients"].as_array();
                string key = json["Authorization"].as_string();
                vector<string> clients;
                for (auto x : Jsonclients)
                {
                    clients.push_back(x.as_string());
                }
                InviteGroupRequest req(key, groupID, clients);

                auto res = GroupchatService::InviteGroup(req);

                jsonObj["Result"] = json::value::number(res.Result);
                jsonObj["Status"] = json::value::string(res.Status);
                cout << "Status: " << res.Status << endl;
            })
            .wait();
        return jsonObj;
    }

    json::value GroupchatController::postLeaveGroup(http_request *message)
    {
        json::value jsonObj = json::value::object();
        message->extract_json()
            .then([&jsonObj](json::value json) {
                unsigned int groupID = json["groupid"].as_integer();
                string ClientName = json["clientName"].as_string();
                string key = json["Authorization"].as_string();
                LeaveGroupRequest req(key, groupID, ClientName);
                auto res = GroupchatService::LeaveGroup(req);
                jsonObj["Result"] = json::value::number(res.Result);
                jsonObj["Status"] = json::value::string(res.Status);
            })
            .wait();
        return jsonObj;
    }
    json::value GroupchatController::postCreateGroup(http_request *message)
    {
        json::value jsonObj = json::value::object();
        message->extract_json()
            .then([&jsonObj](json::value json) {
                string roomName = json["roomname"].as_string();
                bool access = json["access"].as_integer();
                string owner = json["owner"].as_string();
                string pass = json["password"].as_string();
                string key = json["Authorization"].as_string();
                CreateGroupRequest req(key, roomName, access, owner, pass);
                auto res = GroupchatService::CreateGroup(req);
                jsonObj["Result"] = json::value::number(res.Result);
                jsonObj["Status"] = json::value::string(res.Status);
            })
            .wait();

        return jsonObj;
    }
    json::value GroupchatController::postDeleteGroup(http_request *message)
    {
        json::value jsonObj = json::value::object();
        message->extract_json()
            .then([&jsonObj](json::value json) {
                unsigned int id = json["groupid"].as_integer();
                string clientname = json["clientName"].as_string();
                string pass = json["password"].as_string();
                string key = json["Authorization"].as_string();
                DeleteGroupRequest req(key, id, clientname, pass);
                auto res = GroupchatService::DeleteGroup(req);
                jsonObj["Result"] = json::value::number(res.Result);
                jsonObj["Status"] = json::value::string(res.Status);
            })
            .wait();

        return jsonObj;
    }
    json::value GroupchatController::postModifyGroup(http_request *message)
    {
        json::value jsonObj = json::value::object();
        message->extract_json()
            .then([&jsonObj](json::value json) {
                unsigned int id = json["groupid"].as_integer();
                string clientname = json["clientName"].as_string();
                string pass = json["password"].as_string();
                string newname = json["newname"].as_string();
                string newpass = json["newpass"].as_string();
                string key = json["Authorization"].as_string();
                ModifyGroupRequest req(key, id, clientname, newname, newpass, pass);
                auto res = GroupchatService::ModifyGroup(req);
                jsonObj["Result"] = json::value::number(res.Result);
                jsonObj["Status"] = json::value::string(res.Status);
            })
            .wait();

        return jsonObj;
    }

    json::value GroupchatController::getReadGroup(
        http_request *message)
    {
        string authenKey = message->headers()["Authorization"];
        string key = message->headers()["groupid"];
        unsigned int groupid;
        if (key.empty() || key.compare("") == 0 || key.size() < 0)
            groupid = -1;
        else
            groupid = stoi(key);
        ReadGroupUserRequest req(authenKey, groupid);
        auto res = GroupchatService::ReadGroup(req);
        json::value jsonObj = json::value::object();
        jsonObj["Result"] = json::value::number(res.Result);
        jsonObj["Status"] = json::value::string(res.Status);
        jsonObj["usernames"] = json::value::array();

        if (res.Result == RESULT_CODE::RESULT_SUCCESS)
        {
            cout << "Successs\n";
            int i = 0;
            for (auto item : res.users)
                jsonObj["usernames"][i++] = json::value::string(item);
        }

        return jsonObj;
    }
} // namespace Server::RestServer::Controllers
