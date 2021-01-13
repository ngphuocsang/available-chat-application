#include <gtest/gtest.h>
#include "ChatServer.h"
#include "MyLibrary.h"
#include "helper.h"
#include "clientClass.h"
#include <random>
#include "AccountsService.h"
#include <unistd.h>
#include "UserInterface.h"
#include "AccountsAPI.h"
ChatServer *sv = ChatServer::getInstance();
using namespace std;
using namespace MyLibrary;
using namespace Server::RestServer;
using namespace Server::RestServer::Services;

RestServerController *RestServer = NULL;
Database *MySqlServer = NULL;
ChatServer *ChatServerInstance = NULL;
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
static vector<string> ValidName = {"admin", "hoang", "dat", "sang", "khuong", "nguyen", "hai", "meep", "sang1", "nguyen1"};
void *initSpamer(void *par)
{
    using namespace Client::RestAPI;
    int spammer = *(int *)par;
    string spamName = ValidName[spammer];

    clientClass *client = clientClass::getInstance();
    client->initialize();
    client->Username = "bao";
    cout << "Spammer name: " << spamName << endl;
    client->GroupID = 1;
    client->color = 0;
    int step = 1;
    bool done = false;
    while (step <= 1000)
    {
        AuthenAccountRequest req("bao", generateSHA1("123456789"));
        AuthenAccountResponse res = AccountsService::postAuthenAccount(req);

        cout << "Authen complete\n";
        if (res.Result != RESULT_CODE::RESULT_SUCCESS)
        {
            cout << "Error occur: " << res.Result << endl;
            client->connectToServer();
            client->Logout();
            _exit(1);
        }
        client->connectToServer();
        client->Logout();
        step++;
    }
    cout << "Exiting~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
    _exit(0);
}
TEST(AuthenLogin, MultipleLoginAtTime)
{
    int Spam = 1;
    // cout << "Spammer: ";
    // cin >> Spam;

    for (int i = 0; i < Spam; i++) // loop will run n times (n=5)
    {
        // initSpamer((void *)&i);

        if (fork() == 0)
        {
            cout << "Create Spammer " << i + 1 << endl;
            initSpamer((void *)&i);
            _exit(0);
        }
    }

    pid_t child_pid, wpid;
    int status = 0;
    while ((wpid = wait(&status)) > 0)
    {
        if ((status) != 0)
        {
            cout << "status code: " << status << endl;
            EXPECT_TRUE(false);
        }
    };
    EXPECT_TRUE(true);
}

int main(int argc, char **argv)
{
    // TestingServer::initializerServer();
    // // cout << "running\n";
    // AppSetting::loadSettingServerFromFile();
    // // initiailzeDatabase();
    // // initializeChatServer();
    // testing::InitGoogleTest(&argc, argv);
    // int result = RUN_ALL_TESTS();
    // cout << "complete!!!!!!\n";
    // TestingServer::shutdown();
    return 0;
}
