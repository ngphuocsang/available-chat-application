#include <gtest/gtest.h>
// #include "example.h"
#include "ChatServer.h"
#include "MyLibrary.h"
#include "UserInterface.h"
#include "Database.h"
#include "RestServerController.h"
ChatServer *sv = ChatServer::getInstance();
using namespace std;
using namespace MyLibrary;
using namespace Server::RestServer;
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////// ADDCLIENT 27/08/2020
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RESETCHATSERVER()
{
    sv->Sock = -1;
    sv->ClientSocks.clear();
    sv->Clients.clear();
}
int initiailzeDatabase()
{
    UserInterface::printHorizontalSeparatedLine();
    INFO_LOG("Connection to MySQL database now initializing ...\n");
    INFO_LOG("Connecting to ");
    cout << AppSetting::SettingValues["MySqlServIP"] << " with User: "
         << AppSetting::SettingValues["MySqlServUser"] << " on Database: "
         << AppSetting::SettingValues["MySqlServDbname"] << endl;

    Database *MySqlServer = Database::getInstance();

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
TEST(addClient, ValidParameter)
{
    string expectName = "Olympic";
    sockaddr_in Addr;
    ClientSock *client = new ClientSock(expectName, 2020, Addr);
    sv->addClient(client);
    string retName = sv->Clients[0]->Username;
    sv->Clients.clear();
    EXPECT_EQ(expectName, retName);
}
TEST(addClient, NULLParameter)
{
    sockaddr_in Addr;
    ClientSock *client = NULL;
    int code = sv->addClient(client);
    sv->Clients.clear();
    EXPECT_EQ(-1, code);
}
TEST(addClient, MultipleAdd)
{
    vector<string> ListName = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j"};
    sockaddr_in Addr;
    ClientSock *client = NULL;
    for (int i = 0; i < 10; i++)
    {
        client = new ClientSock(ListName[i], 20, Addr);
        sv->addClient(client);
    }
    bool error = false;
    for (int i = 0; i < 10; i++)
    {
        if (ListName[i].compare(sv->Clients[i]->Username) != 0)
        {
            error = true;
            break;
        }
    }
    sv->Clients.clear();
    EXPECT_EQ(false, error);
}
TEST(addClient, SpecialCharParameter)
{
    string expectName = "@#!@Olympic";
    sockaddr_in Addr;
    ClientSock *client = new ClientSock(expectName, 2020, Addr);
    sv->addClient(client);
    string retName = sv->Clients[0]->Username;
    sv->Clients.clear();
    EXPECT_EQ(expectName, retName);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////// REMOVE ACTIVE USERS 27/08/2020
////////////////////////////////////////////////////////////////////////////////////////////////////////////
vector<string> Defaultclients = {"Elsowrd", "Rena", "Aisha", "Chung", "Elesis", "Add", "Laby", "Rose", "Eve"};
void resetClient()
{

    RESETCHATSERVER();
    ClientSock *cli;
    sockaddr_in add;
    for (auto i : Defaultclients)
    {
        cli = new ClientSock(i, 0, add);
        sv->Clients.push_back(cli);
        sv->ClientSocks[i] = cli;
    }
}
TEST(RemoveActive, multipleRemove)
{
    resetClient();
    vector<string> remove = {"ChUng", "Aisha", "Add"};
    for (auto i : remove)
    {
        sv->removeActive(sv->Clients, i);
    }
    bool err = false;
    for (auto i : remove)
    {
        for (auto j : sv->Clients)
        {
            if (i.compare(j->Username) == 0)
            {
                err = true;
                break;
            }
        }
    }
    EXPECT_FALSE(err);
}
TEST(RemoveActive, CaseSensitive)
{
    resetClient();
    string remove = "eLsWorD";

    sv->removeActive(sv->Clients, remove);

    bool err = false;

    for (auto j : sv->Clients)
    {
        if (j->Username.compare("Elsword") == 0)
        {
            err = true;
            break;
        }
    }
    EXPECT_FALSE(err);
}
TEST(RemoveActive, ValidParameter)
{
    resetClient();
    string mover = "Rose";
    sv->removeActive(sv->Clients, mover);
    bool error = false;
    if (sv->ClientSocks.find(mover) != sv->ClientSocks.end())
    {
        error = true;
    }
    if (error == false)
        for (auto i : sv->Clients)
        {
            string name = i->Username;
            if (name.compare(mover) == 0)
            {
                error = true;
                break;
            }
        }
    ASSERT_FALSE(error);
}
TEST(RemoveActive, InValidParameter)
{
    resetClient();
    string mover = "Ran";
    sv->removeActive(sv->Clients, mover);
    bool error = false;

    for (int i = 0; i < Defaultclients.size(); i++)
    {
        string name = sv->Clients[i]->Username;
        if (name.compare(Defaultclients[i]) != 0)
        {
            error = true;
            break;
        }
    }
    ASSERT_FALSE(error);
}
TEST(RemoveActive, EmptyParameter)
{
    resetClient();
    string mover = "";
    sv->removeActive(sv->Clients, mover);
    bool error = false;

    for (int i = 0; i < Defaultclients.size(); i++)
    {
        string name = sv->Clients[i]->Username;
        if (name.compare(Defaultclients[i]) != 0)
        {
            error = true;
            break;
        }
    }
    ASSERT_FALSE(error);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////// Server initialize  27/08/2020
////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(initialize, ValidCase)
{
    using namespace MyLibrary;
    int result = sv->initialize();
    int expectResult = RESULT_CODE::RESULT_SUCCESS;
    EXPECT_EQ(expectResult, result);
}
TEST(initialize, ServerIPAfterInit)
{
    int result = sv->initialize();

    struct sockaddr_in server_addr;
    socklen_t namelen = sizeof(server_addr);
    getsockname(sv->Sock, (struct sockaddr *)&server_addr, &namelen);
    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(server_addr.sin_addr), ip, INET_ADDRSTRLEN);
    std::string serverIP(ip);
    string nexpectIP = "127.0.0.1";
    int expectport = 8888;
    bool error = false;

    if (nexpectIP.compare(serverIP) == 0 || htons(expectport) != (server_addr.sin_port))
    {
        error = true;
    }

    EXPECT_FALSE(error);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////// Server forwardRoomMessage  27/08/2020
////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(forwardRoomMsg, NormalAccValidCase)
{
    int expect = DATABASE_RETURN_CODE::SUCCESS;
    int actual = sv->forwardRoomMsg(90, "bao", "Hello", "1");
    EXPECT_EQ(expect, actual);
}
TEST(forwardRoomMsg, AdValidCase)
{
    int expect = DATABASE_RETURN_CODE::SUCCESS;
    int actual = sv->forwardRoomMsg(90, "admin", "Hello", "1");
    EXPECT_EQ(expect, actual);
}
TEST(forwardRoomMsg, InvalidNegativeRoomID)
{
    // initiailzeDatabase();
    int expect = DATABASE_RETURN_CODE::INVALID_ROOMID;
    int actual = sv->forwardRoomMsg(-1, "bao", "Hello", "1");
    EXPECT_EQ(expect, actual);
}
TEST(forwardRoomMsg, AdInvalidNegativeRoomID)
{
    // initiailzeDatabase();
    int expect = DATABASE_RETURN_CODE::INVALID_ROOMID;
    int actual = sv->forwardRoomMsg(-1, "admin", "Hello", "1");
    EXPECT_EQ(expect, actual);
}
TEST(forwardRoomMsg, InvalidRoomInexist)
{
    // initiailzeDatabase();
    int expect = DATABASE_RETURN_CODE::UNFOUND_ROOM_USERS;
    int actual = sv->forwardRoomMsg(1000, "bao", "Hello", "1");
    EXPECT_EQ(expect, actual);
}
TEST(forwardRoomMsg, AdInvalidRoomInexist)
{
    // initiailzeDatabase();
    int expect = DATABASE_RETURN_CODE::UNFOUND_ROOM_USERS;
    int actual = sv->forwardRoomMsg(1000, "admin", "Hello", "1");
    EXPECT_EQ(expect, actual);
}
TEST(forwardRoomMsg, NonLatinMsg)
{
    int expect = DATABASE_RETURN_CODE::SUCCESS;
    int actual = sv->forwardRoomMsg(90, "bao", "見ないで", "1");
    EXPECT_EQ(expect, actual);
}
TEST(forwardRoomMsg, AdNonLatinMsg)
{
    int expect = DATABASE_RETURN_CODE::SUCCESS;
    int actual = sv->forwardRoomMsg(90, "admin", "見ないで", "1");
    EXPECT_EQ(expect, actual);
}
TEST(forwardRoomMsg, SpecialCaseMsg)
{
    int expect = DATABASE_RETURN_CODE::SUCCESS;
    int actual = sv->forwardRoomMsg(90, "bao", "!@#!#$$^^", "1");
    EXPECT_EQ(expect, actual);
}
TEST(forwardRoomMsg, AdSpecialCaseMsg)
{
    int expect = DATABASE_RETURN_CODE::SUCCESS;
    int actual = sv->forwardRoomMsg(90, "admin", "!@#!#$$^^", "1");
    EXPECT_EQ(expect, actual);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////// Server forwardGroupMessage  27/08/2020
////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(forwardGroupMsg, ValidCase)
{
    int expect = DATABASE_RETURN_CODE::SUCCESS;
    int actual = sv->forwardGroupMsg(7, "bao", "Meep Guardian", "1");
    EXPECT_EQ(expect, actual);
}
TEST(forwardGroupMsg, AdValidCase)
{
    int expect = DATABASE_RETURN_CODE::SUCCESS;
    int actual = sv->forwardGroupMsg(7, "admin", "Meep Guardian", "1");
    EXPECT_EQ(expect, actual);
}
TEST(forwardGroupMsg, InvalidNegativeGroupID)
{
    // initiailzeDatabase();
    int expect = DATABASE_RETURN_CODE::INVALID_ROOMID;
    int actual = sv->forwardGroupMsg(-1, "bao", "Hello", "1");
    EXPECT_EQ(expect, actual);
}
TEST(forwardGroupMsg, AdInvalidNegativeGroupID)
{
    // initiailzeDatabase();
    int expect = DATABASE_RETURN_CODE::INVALID_ROOMID;
    int actual = sv->forwardGroupMsg(-1, "admin", "Hello", "1");
    EXPECT_EQ(expect, actual);
}
TEST(forwardGroupMsg, InvalidGroupInexist)
{
    // initiailzeDatabase();
    int expect = DATABASE_RETURN_CODE::UNFOUND_ROOM_USERS;
    int actual = sv->forwardGroupMsg(1000, "bao", "Hello", "1");
    EXPECT_EQ(expect, actual);
}
TEST(forwardGroupMsg, AdInvalidGroupInexist)
{
    // initiailzeDatabase();
    int expect = DATABASE_RETURN_CODE::UNFOUND_ROOM_USERS;
    int actual = sv->forwardGroupMsg(1000, "admin", "Hello", "1");
    EXPECT_EQ(expect, actual);
}

TEST(forwardGroupMsg, NonLatinMsg)
{
    int expect = DATABASE_RETURN_CODE::SUCCESS;
    int actual = sv->forwardGroupMsg(7, "bao", "見ないで", "1");
    EXPECT_EQ(expect, actual);
}
TEST(forwardGroupMsg, AdNonLatinMsg)
{
    int expect = DATABASE_RETURN_CODE::SUCCESS;
    int actual = sv->forwardGroupMsg(7, "admin", "見ないで", "1");
    EXPECT_EQ(expect, actual);
}
TEST(forwardGroupMsg, SpecialCaseMsg)
{
    int expect = DATABASE_RETURN_CODE::SUCCESS;
    int actual = sv->forwardGroupMsg(7, "bao", "!@#!#$$^^", "1");
    EXPECT_EQ(expect, actual);
}
TEST(forwardGroupMsg, AdSpecialCaseMsg)
{
    int expect = DATABASE_RETURN_CODE::SUCCESS;
    int actual = sv->forwardGroupMsg(7, "admin", "!@#!#$$^^", "1");
    EXPECT_EQ(expect, actual);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
    int result = RESULT_CODE::RESULT_UNKNOWN;
    if ((result = AppSetting::loadSettingServerFromFile()))
        return result;
    initiailzeDatabase();

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
