#include <gtest/gtest.h>
#include "ChatServer.h"
#include "MyLibrary.h"
#include "UserInterface.h"
#include "Database.h"
#include "RestServerController.h"
#include "RequestsAndResponses.h"
#include "AccountsService.h"
#include "GroupchatService.h"
#include "helper.h"
ChatServer *sv = ChatServer::getInstance();
using namespace std;
using namespace MyLibrary;
using namespace Server::RestServer;
using namespace MyLibrary::Models::Requests;
using namespace MyLibrary::Models::Responses;
using namespace Server::RestServer::Services;
RestServerController *RestServer = NULL;
string authenKey = generateSHA1("123456789");
Database *MySqlServer = NULL;
ChatServer *ChatServerInstance = NULL;

TEST(InviteGroup, ValidCase)
{
    vector<string> clients;
    clients.push_back("hai");
    InviteGroupRequest req(authenKey, 1, clients);
    InviteGroupResponse res = GroupchatService::InviteGroup(req);
    EXPECT_EQ(RESULT_CODE::RESULT_SUCCESS, res.Result);
    EXPECT_EQ("Success", res.Status);
}

TEST(JoinGroup, InvalidGroup)
{
    JoinGroupRequest req(authenKey, 0, "1234", "hai");
    JoinGroupResponse res = GroupchatService::JoinGroup(req);
    EXPECT_EQ(RESULT_CODE::GROUP_INEXIST, res.Result);
    EXPECT_EQ("Group inexist", res.Status);
}

TEST(JoinPublicGroup, ValidCase)
{
    JoinGroupResponse res;
    JoinGroupRequest req(authenKey, 1, "1234", "hai");
    res = GroupchatService::JoinGroup(req);
    EXPECT_EQ(res.Result, 0);
    EXPECT_EQ("Success", res.Status);
}

TEST(JoinPrivateGroup, ValidCase)
{
    JoinGroupResponse res;
    JoinGroupRequest req(authenKey, 11, "1234", "SevenEleven");
    res = GroupchatService::JoinGroup(req);
    EXPECT_EQ(res.Result, 0);
}

TEST(JoinGroup, InvalidPassword)
{
    JoinGroupRequest req(authenKey, 11, "1235", "hai");
    JoinGroupResponse res = GroupchatService::JoinGroup(req);
    EXPECT_EQ(RESULT_CODE::RESULT_UNKNOWN, res.Result);
    EXPECT_EQ("Unsuccess", res.Status);
}
TEST(JoinGroup, Unauthen)
{
    JoinGroupRequest req("NULL", 11, "1235", "hai");
    JoinGroupResponse res = GroupchatService::JoinGroup(req);
    EXPECT_EQ(RESULT_CODE::UNAUTHENTICATION, res.Result);
    EXPECT_EQ("Unauthenticated", res.Status);
}
TEST(LeaveGroup, ValidCase)
{
    LeaveGroupRequest req(authenKey, 1, "khai");
    LeaveGroupResponse res = GroupchatService::LeaveGroup(req);
    EXPECT_EQ(RESULT_CODE::RESULT_SUCCESS, res.Result);
    EXPECT_EQ("Success", res.Status);
}
TEST(LeaveGroup, Unauthen)
{
    LeaveGroupRequest req("NULL", 1, "khai");
    LeaveGroupResponse res = GroupchatService::LeaveGroup(req);
    EXPECT_EQ(RESULT_CODE::UNAUTHENTICATION, res.Result);
    EXPECT_EQ("Unauthenticated", res.Status);
}
TEST(CreateGroup, InvalidCase)
{
    CreateGroupRequest req(authenKey, "test", 1, "khuong", "");
    CreateGroupResponse res = GroupchatService::CreateGroup(req);
    EXPECT_EQ(RESULT_CODE::GROUP_PASSWORD_NULL, res.Result);
    EXPECT_EQ("Password is NULL", res.Status);
}
TEST(CreateGroup, Unauthen)
{
    CreateGroupRequest req("NULL", "test", 1, "khuong", "");
    CreateGroupResponse res = GroupchatService::CreateGroup(req);
    EXPECT_EQ(RESULT_CODE::UNAUTHENTICATION, res.Result);
    EXPECT_EQ("Unauthenticated", res.Status);
}
TEST(DeleteGroup, InvalidGroup)
{
    DeleteGroupRequest req(authenKey, 0, "khuong", "1234");
    DeleteGroupResponse res = GroupchatService::DeleteGroup(req);
    EXPECT_EQ(RESULT_CODE::GROUP_INEXIST, res.Result);
    EXPECT_EQ("Group inexist", res.Status);
}

TEST(DeleteGroup, InvalidUsername)
{
    DeleteGroupRequest req(authenKey, 1, "khai", "1234");
    DeleteGroupResponse res = GroupchatService::DeleteGroup(req);
    EXPECT_EQ(RESULT_CODE::GROUP_ACCOUNT_INVALID, res.Result);
    EXPECT_EQ("Read Account Invalid", res.Status);
}

TEST(DeleteGroup, NeitherOwnerNorAdmin)
{
    DeleteGroupRequest req(authenKey, 1, "hai", "1234");
    DeleteGroupResponse res = GroupchatService::DeleteGroup(req);
    EXPECT_EQ(RESULT_CODE::GROUP_OWNER_OR_PASSWORD_INVALID, res.Result);
    EXPECT_EQ("Owner or Password Invalid", res.Status);
}

TEST(DeleteGroup, InvalidPassword)
{
    DeleteGroupRequest req(authenKey, 1, "khuong", "1235");
    DeleteGroupResponse res = GroupchatService::DeleteGroup(req);
    EXPECT_EQ(RESULT_CODE::GROUP_OWNER_OR_PASSWORD_INVALID, res.Result);
    EXPECT_EQ("Owner or Password Invalid", res.Status);
}
TEST(DeleteGroup, Unauthen)
{
    DeleteGroupRequest req("NULL", 1, "khuong", "1235");
    DeleteGroupResponse res = GroupchatService::DeleteGroup(req);
    EXPECT_EQ(RESULT_CODE::UNAUTHENTICATION, res.Result);
    EXPECT_EQ("Unauthenticated", res.Status);
}
TEST(ModifyGroup, InvalidGroup)
{
    ModifyGroupRequest req(authenKey, 0, "bao", "hoi yeu quy", "1234", "4321");
    ModifyGroupResponse res = GroupchatService::ModifyGroup(req);
    EXPECT_EQ(RESULT_CODE::GROUP_INEXIST, res.Result);
    EXPECT_EQ("Group inexist", res.Status);
}

TEST(ModifyGroup, InvalidUsername)
{
    ModifyGroupRequest req(authenKey, 1, "khoi", "hoi yeu quy", "1234", "4321");
    ModifyGroupResponse res = GroupchatService::ModifyGroup(req);
    EXPECT_EQ(RESULT_CODE::GROUP_ACCOUNT_INVALID, res.Result);
    EXPECT_EQ("Read Account Invalid", res.Status);
}

TEST(ModifyGroup, AdminCase)
{
    ModifyGroupRequest req(authenKey, 11, "admin", "Axis party", "1234", "1234");
    ModifyGroupResponse res = GroupchatService::ModifyGroup(req);
    EXPECT_EQ(RESULT_CODE::RESULT_SUCCESS, res.Result);
    EXPECT_EQ("Success", res.Status);
}

TEST(ModifyGroup, Unauthen)
{
    ModifyGroupRequest req("NULL", 11, "admin", "Axis party", "1234", "1234");
    ModifyGroupResponse res = GroupchatService::ModifyGroup(req);
    EXPECT_EQ(RESULT_CODE::UNAUTHENTICATION, res.Result);
    EXPECT_EQ("Unauthenticated", res.Status);
}

TEST(ModifyPrivateGroup, ValidCase)
{
    ModifyGroupRequest req(authenKey, 11, "khuong", "Axis party", "1234", "1234");
    ModifyGroupResponse res = GroupchatService::ModifyGroup(req);
    EXPECT_EQ(RESULT_CODE::RESULT_SUCCESS, res.Result);
    EXPECT_EQ("Success", res.Status);
}

TEST(ModifyPrivateGroup, EmptyGroupName)
{
    ModifyGroupRequest req(authenKey, 11, "khuong", "", "1234", "1234");
    ModifyGroupResponse res = GroupchatService::ModifyGroup(req);
    EXPECT_EQ(RESULT_CODE::RESULT_SUCCESS, res.Result);
    EXPECT_EQ("Success", res.Status);
}

TEST(ModifyPrivateGroup, InvalidPassword)
{
    ModifyGroupRequest req(authenKey, 11, "khuong", "Axis party", "1236", "1235");
    ModifyGroupResponse res = GroupchatService::ModifyGroup(req);
    EXPECT_EQ(RESULT_CODE::GROUP_OWNER_OR_PASSWORD_INVALID, res.Result);
    EXPECT_EQ("Group Owner or Password is Invalid", res.Status);
}

TEST(ModifyPrivateGroup, InvalidOwner)
{
    ModifyGroupRequest req(authenKey, 11, "dat", "Axis party", "1234", "1234");
    ModifyGroupResponse res = GroupchatService::ModifyGroup(req);
    EXPECT_EQ(RESULT_CODE::GROUP_OWNER_OR_PASSWORD_INVALID, res.Result);
    EXPECT_EQ("Group Owner or Password is Invalid", res.Status);
}

TEST(ModifyPrivateGroup, Unauthen)
{
    ModifyGroupRequest req("NULL", 11, "dat", "Axis party", "1234", "1234");
    ModifyGroupResponse res = GroupchatService::ModifyGroup(req);
    EXPECT_EQ(RESULT_CODE::UNAUTHENTICATION, res.Result);
    EXPECT_EQ("Unauthenticated", res.Status);
}

TEST(ModifyPublicGroup, EmptyGroupName)
{
    ModifyGroupRequest req(authenKey, 1, "khuong", "", "1234", "");
    ModifyGroupResponse res = GroupchatService::ModifyGroup(req);
    EXPECT_EQ(RESULT_CODE::GROUP_NAME_EMPTY, res.Result);
    EXPECT_EQ("Group name empty", res.Status);
}

TEST(ModifyPublicGroup, InvalidPassword)
{
    ModifyGroupRequest req(authenKey, 1, "khuong", "hoi diet quy", "1235", "1234");
    ModifyGroupResponse res = GroupchatService::ModifyGroup(req);
    EXPECT_EQ(RESULT_CODE::GROUP_OWNER_OR_PASSWORD_INVALID, res.Result);
    EXPECT_EQ("Group Owner or Password is Invalid", res.Status);
}

TEST(ModifyPublicGroup, InvalidOwner)
{
    ModifyGroupRequest req(authenKey, 1, "bao", "hoi diet quy", "1234", "1234");
    ModifyGroupResponse res = GroupchatService::ModifyGroup(req);
    EXPECT_EQ(RESULT_CODE::GROUP_OWNER_OR_PASSWORD_INVALID, res.Result);
    EXPECT_EQ("Group Owner or Password is Invalid", res.Status);
}

TEST(ModifyPublicGroup, ValidCase)
{
    ModifyGroupRequest req(authenKey, 1, "khuong", "hoi diet quy", "1234", "");
    ModifyGroupResponse res = GroupchatService::ModifyGroup(req);
    EXPECT_EQ(RESULT_CODE::RESULT_SUCCESS, res.Result);
    EXPECT_EQ("Success", res.Status);
}

TEST(ModifyPublicGroup, Unauthen)
{
    ModifyGroupRequest req("NULL", 1, "khuong", "hoi diet quy", "1234", "");
    ModifyGroupResponse res = GroupchatService::ModifyGroup(req);
    EXPECT_EQ(RESULT_CODE::UNAUTHENTICATION, res.Result);
    EXPECT_EQ("Unauthenticated", res.Status);
}

TEST(ReadGroup, InvalidGroup)
{
    ReadGroupUserRequest req(authenKey, 0);
    ReadGroupUserResponse res = GroupchatService::ReadGroup(req);
    EXPECT_EQ(RESULT_CODE::GROUP_UNFOUND, res.Result);
    EXPECT_EQ("Group Unfound", res.Status);
}

TEST(ReadGroup, Unauthen)
{
    ReadGroupUserRequest req("NULL", 0);
    ReadGroupUserResponse res = GroupchatService::ReadGroup(req);
    EXPECT_EQ(RESULT_CODE::UNAUTHENTICATION, res.Result);
    EXPECT_EQ("Unauthenticated", res.Status);
}

TEST(ReadGroup, ValidCase)
{
    ReadGroupUserRequest req(authenKey, 1);
    ReadGroupUserResponse res = GroupchatService::ReadGroup(req);
    EXPECT_EQ(RESULT_CODE::RESULT_SUCCESS, res.Result);
    EXPECT_EQ("Success", res.Status);
    vector<string> users;
    users.push_back("hai");
    users.push_back("hoang");
    EXPECT_EQ(users, res.users);
}

int main(int argc, char **argv)
{
    using namespace Server::RestServer::Services;
    TestingServer::initializerServer();

    testing::InitGoogleTest(&argc, argv);
    AuthenAccountRequest req("admin", generateSHA1("000000000"));
    AuthenAccountResponse res = AccountsService::postAuthenAccount(req);

    cout << "res AuthKey:" << res.AuthKey << endl;
    cout << "res Result: " << res.Result << endl;

    authenKey = res.AuthKey;
    int result = RUN_ALL_TESTS();
    TestingServer::shutdown();
    return 0;
}