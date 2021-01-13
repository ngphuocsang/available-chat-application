#include <gtest/gtest.h>
#include "ChatServer.h"
#include "MyLibrary.h"
#include "helper.h"
#include "clientClass.h"
#include <random>
#include "AccountsService.h"
#include <unistd.h>
#include "UserInterface.h"
ChatServer *sv = ChatServer::getInstance();
using namespace std;
using namespace MyLibrary;
using namespace Server::RestServer;
using namespace Server::RestServer::Services;

RestServerController *RestServer = NULL;
Database *MySqlServer = NULL;
ChatServer *ChatServerInstance = NULL;
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

void gen_random(char *s, size_t len)
{
    for (size_t i = 0; i < len; ++i)
    {
        int randomChar = rand() % (26 + 26 + 10);
        if (randomChar < 26)
            s[i] = 'a' + randomChar;
        else if (randomChar < 26 + 26)
            s[i] = 'A' + randomChar - 26;
        else
            s[i] = '0' + randomChar - 26 - 26;
    }
    s[len] = 0;
}
void *initSpamer(void *par)
{
    clientClass *client = clientClass::getInstance();
    client->initialize();
    client->Username = "Meep";
    client->GroupID = 138;
    client->color = 0;
    client->connectToServer();
    int step = 1;
    bool done = false;
    while (step <= 1000)
    {
        // string ss = random_string();
        // string ss = "Hello" + client->Username;
        // // cout << "Sending From Client step: " << step << endl;
        // // void *x = client->sendMessageToServer(ss, TAG::_SEND);
        // // sleep(1);

        // string msg = TAG::_SEND + to_string(clientClass::getInstance()->GroupID) + "%" + to_string(client->color) + "%" + ss;
        char randstr[1024];
        gen_random(randstr, 1024);

        // cmsg = "hello";
        // cout << "Before sending: " << msg << endl;
        int code = send(client->Sock, randstr, 1024, 0);
        if (code <= 0)
        {
            break;
        }
        // if (step % 100 == 0)
        // {
        //     cout << "---------------------------------------------------\n";
        //     cout << "Step: " << step << endl;
        //     cout << "---------------------------------------------------\n";
        // }
        step++;
    }
    cout << "Complete sending 100 message " << step << "\n";
    if (step == 0)
        done = true;

    client->Logout();
    cout << "Exiting~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
    _exit(0);
}
TEST(SpamMessage, 10Users1000Message)
{
    int Spam = 10;
    // cout << "Spammer: ";
    // cin >> Spam;

    for (int i = 0; i < Spam; i++) // loop will run n times (n=5)
    {
        cout << "Create Spammer " << i + 1 << endl;
        if (fork() == 0)
        {
            initSpamer(NULL);
            _exit(0);
        }
    }
    // cout << "Compete" << endl;
    // for (int i = 0; i < Spam; i++) // loop will run n times (n=5)
    // {
    //     cout << "waiting for pid: " << i << endl;
    //     wait(NULL);
    //     cout << "Wait Complete!!!!\n";
    // }
    pid_t child_pid, wpid;
    int status = 0;
    while ((wpid = wait(&status)) > 0)
    {
        if (WIFEXITED(status) == 0)
        {
            EXPECT_TRUE(false);
        }
    };
    EXPECT_TRUE(true);
}

int main(int argc, char **argv)
{
    // TestingServer::shutdown();
    TestingServer::initializerServer();
    // cout << "running\n";
    AppSetting::loadSettingServerFromFile();
    // initiailzeDatabase();
    // initializeChatServer();
    testing::InitGoogleTest(&argc, argv);
    int result = RUN_ALL_TESTS();
    cout << "complete!!!!!!\n";
    // TestingServer::shutdown();
    return 0;
}
