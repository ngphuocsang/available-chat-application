#include <gtest/gtest.h>

#include "AccountsAPI.h"
#include "helper.h"

using namespace std;
using namespace Server::RestServer;
using namespace MyLibrary;
using namespace Client::RestAPI;
using namespace MyLibrary::Models::Requests;
using namespace MyLibrary::Models::Responses;

TEST(AccountsAPI_Authen, UsernameNotFound)
{
    AuthenAccountRequest req("_____", "_____");
    AuthenAccountResponse res = AccountsAPI::postAuthenAccount(req);
    EXPECT_EQ(RESULT_CODE::AUTHEN_ACCOUNT_INVALID_USERNAME, res.Result);
    EXPECT_EQ("", res.AuthKey);
    EXPECT_EQ("", res.Role); 
}

TEST(AccountsAPI_Authen, InvalidPassword)
{
    AuthenAccountRequest req("test_admin", "_____");
    AuthenAccountResponse res = AccountsAPI::postAuthenAccount(req);
    EXPECT_EQ(RESULT_CODE::AUTHEN_ACCOUNT_INVALID_PASSWORD, res.Result);
    EXPECT_EQ("", res.AuthKey);
    EXPECT_EQ("", res.Role);
}

TEST(AccountsAPI_Authen, ValidAuthen_Admin)
{
    AuthenAccountRequest req("test_admin", "000000000");
    AuthenAccountResponse res = AccountsAPI::postAuthenAccount(req);
    EXPECT_EQ(RESULT_CODE::GENERAL_RESULT_SUCCESSFUL, res.Result);
    EXPECT_NE("", res.AuthKey);
    EXPECT_EQ("0", res.Role);
    LogoutAccountRequest req2("test_admin");
    RESULT_CODE res2 = AccountsAPI::postLogoutAccount(req2);
}

TEST(AccountsAPI_Authen, ValidAuthen_User)
{
    AuthenAccountRequest req("test_user", "123456789");
    AuthenAccountResponse res = AccountsAPI::postAuthenAccount(req);
    EXPECT_EQ(RESULT_CODE::GENERAL_RESULT_SUCCESSFUL, res.Result);
    EXPECT_NE("", res.AuthKey);
    EXPECT_EQ("1", res.Role);
    LogoutAccountRequest req1("test_user");
    RESULT_CODE res1 = AccountsAPI::postLogoutAccount(req1);
}

TEST(AccountsService_Authen, Authenticated_User)
{
    AuthenAccountRequest req1("test_user", "123456789");
    AuthenAccountResponse res1 = AccountsAPI::postAuthenAccount(req1);
    AuthenAccountRequest req2("test_user", "123456789");
    AuthenAccountResponse res2 = AccountsAPI::postAuthenAccount(req2);
    EXPECT_EQ(RESULT_CODE::AUTHEN_ACCOUNT_AUTHENTICATED, res2.Result);
    EXPECT_EQ("", res2.AuthKey);
    EXPECT_EQ("", res2.Role);
    LogoutAccountRequest req3("test_user");
    RESULT_CODE res3 = AccountsAPI::postLogoutAccount(req3);
}

int main(int argc, char **argv)
{
    TestingServer::initializerServer();
    testing::InitGoogleTest(&argc, argv);
    int result = RUN_ALL_TESTS();
    TestingServer::shutdown();
    return 0;
}