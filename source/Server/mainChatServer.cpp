#include "MyLibrary.h"
#include "UserInterface.h"
#include "ChatServer.h"
#include "Database.h"

using namespace std;
using namespace MyLibrary;

Database *MySqlServer = NULL;
ChatServer *ChatServerInstance = NULL;

int initiailzeDatabase();
int initializeRestServer();
int initializeChatServer();
void shutdownAllServer();

int main(int argc, const char *argv[])
{
    InterruptHandler::hookSIGINT();

    int result = RESULT_CODE::RESULT_UNKNOWN;
    if ((result = AppSetting::loadSettingServerFromFile()))
        return result;

    if ((result = initiailzeDatabase()))
        return result;
    if ((result = initializeChatServer()))
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

int initializeChatServer()
{
    UserInterface::printHorizontalSeparatedLine();
    INFO_LOG("Modern C++ Chat server now initializing for connection...\n");

    ChatServerInstance = ChatServer::getInstance();

    int result = RESULT_CODE::DB_SERV_INIT_ERR;

    result = ChatServerInstance->initialize();

    if (result)
        ERROR_LOG("Failed to create C++ Chat server for connection!!!\n");
    else
        result = RESULT_CODE::RESULT_SUCCESS;

    UserInterface::printHorizontalSeparatedLine();

    return result;
}

void shutdownAllServer()
{
    if (MySqlServer != NULL)
    {
        SUCCESS_LOG("Connection to MySQL database server shutdown\n");
        MySqlServer->shutdown();
        delete MySqlServer;
    }
    if (ChatServerInstance != NULL)
    {
        SUCCESS_LOG("Modern C++ Chat server shutdown\n");
        delete ChatServerInstance;
    }

    UserInterface::printHorizontalSeparatedLine();
}
