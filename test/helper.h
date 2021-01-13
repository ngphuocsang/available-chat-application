#include "RestServerController.h"
#include "MyLibrary.h"
#include "Database.h"
#include "ChatServer.h"

using namespace MyLibrary;
using namespace Server::RestServer;

class TestingServer
{
private:
    static Database *MySqlServer;
    static RestServerController *RestServer;
    static ChatServer *ChatServerInstance;

public:
    static int initializerServer()
    {
        AppSetting::loadSettingServerFromFile();

        MySqlServer = Database::getInstance();
        MySqlServer->initialize(
            AppSetting::SettingValues["MySqlServIP"],
            AppSetting::SettingValues["MySqlServUser"],
            AppSetting::SettingValues["MySqlServPass"],
            AppSetting::SettingValues["MySqlServDbname"]);

        string endpoint = "http://";
        endpoint.append(AppSetting::SettingValues["ServerIP"]);
        endpoint.append(string(":"));
        endpoint.append(AppSetting::SettingValues["RestServPort"]);
        endpoint.append("/api");
        RestServer = new RestServerController();
        RestServer->initializeController(endpoint);

        ChatServerInstance = ChatServer::getInstance();
        ChatServerInstance->initialize();
    }

    static int shutdown()
    {
        if (RestServer != NULL)
        {
            RestServer->shutdown().wait();
            delete RestServer;
        }
        if (MySqlServer != NULL)
        {
            MySqlServer->shutdown();
            delete MySqlServer;
        }
        if (ChatServerInstance != NULL)
        {
            delete ChatServerInstance;
        }
    }
};

Database *TestingServer::MySqlServer = NULL;
RestServerController *TestingServer::RestServer = NULL;
ChatServer *TestingServer::ChatServerInstance = NULL;