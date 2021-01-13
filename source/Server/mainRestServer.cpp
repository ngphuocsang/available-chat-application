#include "MyLibrary.h"
#include "UserInterface.h"
#include "RestServerController.h"
#include "Database.h"

using namespace std;
using namespace MyLibrary;
using namespace Server::RestServer;

RestServerController *RestServer = NULL;
Database *MySqlServer = NULL;

int initiailzeDatabase();
int initializeRestServer();
void shutdownAllServer();

int main(int argc, const char *argv[])
{
    InterruptHandler::hookSIGINT();

    int result = RESULT_CODE::RESULT_UNKNOWN;
    if ((result = AppSetting::loadSettingServerFromFile()))
        return result;

    if ((result = initiailzeDatabase()))
        return result;
    if ((result = initializeRestServer()))
        return result;

    InterruptHandler::waitForUserInterrupt();

    shutdownAllServer();

    return RESULT_CODE::RESULT_SUCCESS;
}

int initiailzeDatabase()
{
    UserInterface::printHorizontalSeparatedLine();
    INFO_LOG("Connection to MySQL database now initializing ...\n");
    INFO_LOG("Connecting to ");
    cout << AppSetting::SettingValues["MySqlServIP"] << " with User: "
         << AppSetting::SettingValues["MySqlServUser"] << " on Database: "
         << AppSetting::SettingValues["MySqlServDbname"] << endl;

    MySqlServer = Database::getInstance();

    int result = RESULT_CODE::DB_SERV_INIT_ERR;

    result = MySqlServer->initialize(
        AppSetting::SettingValues["MySqlServIP"],
        AppSetting::SettingValues["MySqlServUser"],
        AppSetting::SettingValues["MySqlServPass"],
        AppSetting::SettingValues["MySqlServDbname"]);

    if (result)
        ERROR_LOG("Failed to create connection to MySQL database!!!\n");
    else
    {
        SUCCESS_LOG("Connection to MySQL database is established\n");
        result = RESULT_CODE::RESULT_SUCCESS;
    }
    UserInterface::printHorizontalSeparatedLine();

    return result;
}

int initializeRestServer()
{
    string endpoint = "http://";
    endpoint.append(AppSetting::SettingValues["ServerIP"]);
    endpoint.append(string(":"));
    endpoint.append(AppSetting::SettingValues["RestServPort"]);
    endpoint.append("/api");
    RestServer = new RestServerController();
    return RestServer->initializeController(endpoint);
}

void shutdownAllServer()
{
    if (RestServer != NULL)
    {
        SUCCESS_LOG("Modern C++ Rest API server shutdown\n");
        RestServer->shutdown().wait();
        delete RestServer;
    }
    if (MySqlServer != NULL)
    {
        SUCCESS_LOG("Connection to MySQL database server shutdown\n");
        MySqlServer->shutdown();
        delete MySqlServer;
    }

    UserInterface::printHorizontalSeparatedLine();
}
