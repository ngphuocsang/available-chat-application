#include "AccountsAPI.h"
#include "UserInterface.h"

#include <cpprest/http_client.h>
#include <pplx/pplxtasks.h>

using namespace web;
using namespace web::http;
using namespace web::http::client;

namespace Client::RestAPI
{
    AuthenAccountResponse AccountsAPI::postAuthenAccount(
        AuthenAccountRequest req)
    {
        AuthenAccountResponse res;
        http_request request = makeHttpRequest(methods::POST);

        string endpoint = "http://";
        endpoint.append(
            AppSetting::SettingValues["ServerIP"] + string(":") +
            AppSetting::SettingValues["RestServPort"]);
        endpoint.append("/api/AuthenAccount");

        http_client client(endpoint);
        json::value body = json::value::object();
        body["username"] = json::value::string(req.Username);
        string sha1 = generateSHA1(req.Password);
        body["password"] = json::value::string(sha1);
        request.set_body(body);

        try
        {
            client.request(request)
                .then([=](http_response response) {
                    return response.extract_json();
                })
                .then([&res](json::value json) {
                    res.Result = (RESULT_CODE)json["result"].as_integer();
                    res.AuthKey = json["authKey"].as_string();
                    res.Role = json["role"].as_string();
                })
                .wait();
        }
        catch (const std::exception &e)
        {
            ERROR_LOG("Can't call API to the endpoint: " + endpoint + "\n");
        }

        return res;
    }
    ActiveAccountsResponse AccountsAPI::getActiveAccounts(
        ActiveAccountsRequest req)
    {
        ActiveAccountsResponse res;
        http_request request = makeHttpRequest(methods::GET);

        string endpoint = "http://";
        endpoint.append(
            AppSetting::SettingValues["ServerIP"] + string(":") +
            AppSetting::SettingValues["RestServPort"]);
        endpoint.append("/api/ActiveAccounts");

        http_client client(endpoint);
        request.headers().add("Authorization", req.AuthKey);

        try
        {
            client.request(request)
                .then([=](http_response response) {
                    return response.extract_json();
                })
                .then([&res](json::value json) {
                    res.Result = (RESULT_CODE)json["result"].as_integer();
                    auto jsonArr = json["usernames"].as_array();
                    for (auto item : jsonArr)
                        res.Usernames.push_back(item.as_string());
                })
                .wait();
        }
        catch (const std::exception &e)
        {
            ERROR_LOG("Can't call API to the endpoint: " + endpoint + "\n");
        }

        return res;
    }

    AccountsResponse AccountsAPI::getAllAccounts(AccountsRequest req)
    {
        AccountsResponse res;
        http_request request = makeHttpRequest(methods::GET);

        string endpoint = "http://";
        endpoint.append(
            AppSetting::SettingValues["ServerIP"] + string(":") +
            AppSetting::SettingValues["RestServPort"]);
        endpoint.append("/api/Accounts");

        http_client client(endpoint);
        request.headers().add("Authorization", req.AuthKey);

        try
        {
            client.request(request)
                .then([=](http_response response) {
                    return response.extract_json();
                })
                .then([&res](json::value json) {
                    res.Result = (RESULT_CODE)json["result"].as_integer();
                    auto jsonArr = json["accounts"].as_array();
                    for (auto item : jsonArr)
                    {
                        Account account("", "", "");
                        account.Username = item["username"].as_string();
                        account.Role = item["role"].as_string();
                        res.Accounts.push_back(account);
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

    RESULT_CODE AccountsAPI::postCreateAccount(CreateAccountRequest req)
    {
        http_request request = makeHttpRequest(methods::POST);
        request.headers().add("Authorization", req.AuthKey);
        RESULT_CODE res = RESULT_CODE::RESULT_UNKNOWN;

        string endpoint = "http://";
        endpoint.append(
            AppSetting::SettingValues["ServerIP"] + string(":") +
            AppSetting::SettingValues["RestServPort"]);
        endpoint.append("/api/CreateAccount");

        http_client client(endpoint);
        json::value body = json::value::object();
        body["username"] = json::value::string(req.Username);
        body["password"] = json::value::string(req.Password);
        body["role"] = json::value::string(req.Role);
        request.set_body(body);

        try
        {
            client.request(request)
                .then([=](http_response response) {
                    return response.extract_json();
                })
                .then([&res](json::value json) {
                    res = (RESULT_CODE)json["result"].as_integer();
                })
                .wait();
        }
        catch (const std::exception &e)
        {
            ERROR_LOG("Can't call API to the endpoint: " + endpoint + "\n");
        }

        return res;
    }
    RESULT_CODE AccountsAPI::postUpdateAccount(UpdateAccountRequest req)
    {
        http_request request = makeHttpRequest(methods::POST);
        request.headers().add("Authorization", req.AuthKey);
        RESULT_CODE res = RESULT_CODE::RESULT_UNKNOWN;

        string endpoint = "http://";
        endpoint.append(
            AppSetting::SettingValues["ServerIP"] + string(":") +
            AppSetting::SettingValues["RestServPort"]);
        endpoint.append("/api/UpdateAccount");

        http_client client(endpoint);
        json::value body = json::value::object();
        body["username"] = json::value::string(req.Username);
        string sha1 = generateSHA1(req.Password);
        body["password"] = json::value::string(req.Password);
        body["role"] = json::value::string(req.Role);
        request.set_body(body);

        try
        {
            client.request(request)
                .then([=](http_response response) {
                    return response.extract_json();
                })
                .then([&res](json::value json) {
                    res = (RESULT_CODE)json["result"].as_integer();
                })
                .wait();
        }
        catch (const std::exception &e)
        {
            ERROR_LOG("Can't call API to the endpoint: " + endpoint + "\n");
        }

        return res;
    }
    RESULT_CODE AccountsAPI::postDeleteAccount(DeleteAccountRequest req)
    {
        http_request request = makeHttpRequest(methods::POST);
        request.headers().add("Authorization", req.AuthKey);
        RESULT_CODE res = RESULT_CODE::RESULT_UNKNOWN;

        string endpoint = "http://";
        endpoint.append(
            AppSetting::SettingValues["ServerIP"] + string(":") +
            AppSetting::SettingValues["RestServPort"]);
        endpoint.append("/api/DeleteAccount");

        http_client client(endpoint);
        json::value body = json::value::object();
        body["username"] = json::value::string(req.Username);
        request.set_body(body);

        try
        {
            client.request(request)
                .then([=](http_response response) {
                    return response.extract_json();
                })
                .then([&res](json::value json) {
                    res = (RESULT_CODE)json["result"].as_integer();
                })
                .wait();
        }
        catch (const std::exception &e)
        {
            ERROR_LOG("Can't call API to the endpoint: " + endpoint + "\n");
        }

        return res;
    }
    RESULT_CODE AccountsAPI::postLogoutAccount(
        LogoutAccountRequest req)
    {
        http_request request = makeHttpRequest(methods::POST);
        RESULT_CODE res = RESULT_CODE::RESULT_UNKNOWN;

        string endpoint = "http://";
        endpoint.append(
            AppSetting::SettingValues["ServerIP"] + string(":") +
            AppSetting::SettingValues["RestServPort"]);
        endpoint.append("/api/LogoutAccount");

        http_client client(endpoint);
        json::value body = json::value::object();
        body["username"] = json::value::string(req.Username);
        request.set_body(body);

        try
        {
            client.request(request)
                .then([=](http_response response) {
                    return response.extract_json();
                })
                .then([&res](json::value json) {
                    res = (RESULT_CODE)json["result"].as_integer();
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