#include "RoomAPI.h"
#include "UserInterface.h"

#include <cpprest/http_client.h>
#include <pplx/pplxtasks.h>

using namespace web;
using namespace web::http;
using namespace web::http::client;

namespace Client::RestAPI
{
    GetRoomResponse RoomAPI::getRoom(GetRoomRequest req)
    {
        GetRoomResponse res;
        http_request request = makeHttpRequest(methods::GET);

        string endpoint = "http://";
        endpoint.append(
            AppSetting::SettingValues["ServerIP"] + string(":") +
            AppSetting::SettingValues["RestServPort"]);

        endpoint.append("/api/Groups");

        http_client client(endpoint);
        request.headers().add("Authorization", req.AuthKey);

        try
        {
            client.request(request)
                .then([=](http_response response) {
                    return response.extract_json();
                })
                .then([&res](json::value json) {
                    //res.Result = (RESULT_CODE)json["result"].as_integer();
                    auto jsonArr = json["Groups"].as_array();
                    for (auto item : jsonArr)
                    {
                        res.Access.push_back(item.at("Access").as_integer());
                        res.Id.push_back(item.at("Id").as_integer());
                        res.Name.push_back(item.at("Name").as_string());
                        res.Owner.push_back(item.at("Owner").as_string());
                    }
                })
                .wait();
        }
        catch (const std::exception &e)
        {
            ERROR_LOG("Can't call API to the endpoint: " + endpoint + "\n");
        }

        return res;
    }

    JoinRoomResponse RoomAPI::joinRoom(JoinRoomRequest req)
    {
        JoinRoomResponse res;
        http_request request = makeHttpRequest(methods::POST);

        string endpoint = "http://";
        endpoint.append(
            AppSetting::SettingValues["ServerIP"] + string(":") +
            AppSetting::SettingValues["RestServPort"]);
        endpoint.append("/api/JoinGroup");

        http_client client(endpoint);
        json::value body = json::value::object();
        body["groupid"] = json::value::number(req.GroupID);
        body["password"] = json::value::string(req.Password);
        body["clientName"] = json::value::string(req.Name);
        body["Authorization"] = json::value::string(req.AuthKey);
        request.set_body(body);

        try
        {
            client.request(request)
                .then([=](http_response response) {
                    return response.extract_json();
                })
                .then([&res](json::value json) {
                    res.Result = (RESULT_CODE)json["Result"].as_integer();
                    res.Status = json["Status"].as_string();
                })
                .wait();
        }
        catch (const std::exception &e)
        {
            ERROR_LOG("Can't call API to the endpoint: " + endpoint + "\n");
        }

        return res;
    }

    InviteRoomResponse RoomAPI::inviteRoom(InviteRoomRequest req)
    {
        InviteRoomResponse res;
        http_request request = makeHttpRequest(methods::POST);

        string endpoint = "http://";
        endpoint.append(
            AppSetting::SettingValues["ServerIP"] + string(":") +
            AppSetting::SettingValues["RestServPort"]);
        endpoint.append("/api/InviteGroup");

        http_client client(endpoint);
        json::value body = json::value::object();
        body["groupid"] = json::value::number(req.GroupID);
        body["clients"] = json::value::array();
        body["Authorization"] = json::value::string(req.AuthKey);
        int i = 0;
        for (auto item : req.Username)
        {
            body["clients"][i++] = json::value::string(item);
        }

        request.set_body(body);

        try
        {
            client.request(request)
                .then([=](http_response response) {
                    return response.extract_json();
                })
                .then([&res](json::value json) {
                    res.Result = (RESULT_CODE)json["Result"].as_integer();
                    res.Status = json["Status"].as_string();
                })
                .wait();
        }
        catch (const std::exception &e)
        {
            ERROR_LOG("Can't call API to the endpoint: " + endpoint + "\n");
        }

        return res;
    }

    LeaveRoomResponse RoomAPI::leaveRoom(LeaveRoomRequest req)
    {
        LeaveRoomResponse res;
        http_request request = makeHttpRequest(methods::POST);

        string endpoint = "http://";
        endpoint.append(
            AppSetting::SettingValues["ServerIP"] + string(":") +
            AppSetting::SettingValues["RestServPort"]);
        endpoint.append("/api/LeaveGroup");

        http_client client(endpoint);
        json::value body = json::value::object();
        body["groupid"] = json::value::number(req.GroupID);
        body["clientName"] = json::value::string(req.Name);
        body["Authorization"] = json::value::string(req.AuthKey);
        request.set_body(body);

        try
        {
            client.request(request)
                .then([=](http_response response) {
                    return response.extract_json();
                })
                .then([&res](json::value json) {
                    res.Result = (RESULT_CODE)json["Result"].as_integer();
                    res.Status = json["Status"].as_string();
                })
                .wait();
        }
        catch (const std::exception &e)
        {
            ERROR_LOG("Can't call API to the endpoint: " + endpoint + "\n");
        }

        return res;
    }

    CreateRoomResponse RoomAPI::createRoom(CreateRoomRequest req)
    {
        CreateRoomResponse res;
        http_request request = makeHttpRequest(methods::POST);

        string endpoint = "http://";
        endpoint.append(
            AppSetting::SettingValues["ServerIP"] + string(":") +
            AppSetting::SettingValues["RestServPort"]);
        endpoint.append("/api/CreateGroup");

        http_client client(endpoint);
        json::value body = json::value::object();
        body["roomname"] = json::value::string(req.Roomname);
        body["access"] = json::value::number(req.Access);
        body["owner"] = json::value::string(req.Owner);
        body["password"] = json::value::string(req.Password);
        body["Authorization"] = json::value::string(req.AuthKey);
        request.set_body(body);

        try
        {
            client.request(request)
                .then([=](http_response response) {
                    return response.extract_json();
                })
                .then([&res](json::value json) {
                    res.Result = (RESULT_CODE)json["Result"].as_integer();
                    res.Status = json["Status"].as_string();
                })
                .wait();
        }
        catch (const std::exception &e)
        {
            ERROR_LOG("Can't call API to the endpoint: " + endpoint + "\n");
        }

        return res;
    }

    DeleteRoomResponse RoomAPI::deleteRoom(DeleteRoomRequest req)
    {
        DeleteRoomResponse res;
        http_request request = makeHttpRequest(methods::POST);

        string endpoint = "http://";
        endpoint.append(
            AppSetting::SettingValues["ServerIP"] + string(":") +
            AppSetting::SettingValues["RestServPort"]);
        endpoint.append("/api/DeleteGroup");

        http_client client(endpoint);
        json::value body = json::value::object();
        body["groupid"] = json::value::number(req.GroupID);
        body["clientName"] = json::value::string(req.Username);
        body["password"] = json::value::string(req.Password);
        body["Authorization"] = json::value::string(req.AuthKey);
        request.set_body(body);

        try
        {
            client.request(request)
                .then([=](http_response response) {
                    return response.extract_json();
                })
                .then([&res](json::value json) {
                    res.Result = (RESULT_CODE)json["Result"].as_integer();
                    res.Status = json["Status"].as_string();
                })
                .wait();
        }
        catch (const std::exception &e)
        {
            ERROR_LOG("Can't call API to the endpoint: " + endpoint + "\n");
        }

        return res;
    }

    ModifyRoomResponse RoomAPI::modifyRoom(ModifyRoomRequest req)
    {
        ModifyRoomResponse res;
        http_request request = makeHttpRequest(methods::POST);

        string endpoint = "http://";
        endpoint.append(
            AppSetting::SettingValues["ServerIP"] + string(":") +
            AppSetting::SettingValues["RestServPort"]);
        endpoint.append("/api/ModifyGroup");

        http_client client(endpoint);
        json::value body = json::value::object();
        body["groupid"] = json::value::number(req.GroupID);
        body["clientName"] = json::value::string(req.Username);
        body["newname"] = json::value::string(req.Newname);
        body["newpass"] = json::value::string(req.Newpass);
        body["password"] = json::value::string(req.Password);
        body["Authorization"] = json::value::string(req.AuthKey);
        request.set_body(body);

        try
        {
            client.request(request)
                .then([=](http_response response) {
                    return response.extract_json();
                })
                .then([&res](json::value json) {
                    res.Result = (RESULT_CODE)json["Result"].as_integer();
                    res.Status = json["Status"].as_string();
                })
                .wait();
        }
        catch (const std::exception &e)
        {
            ERROR_LOG("Can't call API to the endpoint: " + endpoint + "\n");
        }

        return res;
    }

    RoomMemberResponse RoomAPI::getRoomMember(RoomMemberRequest req)
    {
        RoomMemberResponse res;
        http_request request = makeHttpRequest(methods::GET);
        request.headers().add("groupid", req.GroupID);

        string endpoint = "http://";
        endpoint.append(
            AppSetting::SettingValues["ServerIP"] + string(":") +
            AppSetting::SettingValues["RestServPort"]);
        endpoint.append("/api/readGroup");

        http_client client(endpoint);
        request.headers().add("Authorization", req.AuthKey);
        try
        {
            client.request(request)
                .then([=](http_response response) {
                    return response.extract_json();
                })
                .then([&res](json::value json) {
                    res.Result = (RESULT_CODE)json["Result"].as_integer();
                    res.Status = json["Status"].as_string();
                    
                    auto jsonArr = json["usernames"].as_array();
                    for (auto item : jsonArr)
                        res.Username.push_back(item.as_string());
                })
                .wait();
        }
        catch (const std::exception &e)
        {
            ERROR_LOG("Can't call API to the endpoint: " + endpoint + "\n");
        }

        return res;
    }
} // namespace Client::RestAPI
