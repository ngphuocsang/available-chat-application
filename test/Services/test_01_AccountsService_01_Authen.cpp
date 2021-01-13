#include <gtest/gtest.h>

#include "helper.h"
#include "AccountsService.h"


using namespace Server::RestServer::Services;

TEST(AccountsService_Authen, UsernameNotFound)
{
    AuthenAccountRequest req("_____", "_____");
    AuthenAccountResponse res = AccountsService::postAuthenAccount(req);
    EXPECT_EQ(RESULT_CODE::AUTHEN_ACCOUNT_INVALID_USERNAME, res.Result);
    EXPECT_EQ("", res.AuthKey);
    EXPECT_EQ("", res.Role);
}

TEST(AccountsService_Authen, InvalidPassword)
{
    AuthenAccountRequest req("test_admin", "_____");
    AuthenAccountResponse res = AccountsService::postAuthenAccount(req);
    EXPECT_EQ(RESULT_CODE::AUTHEN_ACCOUNT_INVALID_PASSWORD, res.Result);
    EXPECT_EQ("", res.AuthKey);
    EXPECT_EQ("", res.Role);
}

TEST(AccountsService_Authen, ValidAuthen_Admin)
{
    AuthenAccountRequest req("test_admin", generateSHA1("000000000"));
    AuthenAccountResponse res = AccountsService::postAuthenAccount(req);
    EXPECT_EQ(RESULT_CODE::GENERAL_RESULT_SUCCESSFUL, res.Result);
    EXPECT_NE("", res.AuthKey);
    EXPECT_EQ("0", res.Role);
    LogoutAccountRequest req1("test_admin");
    RESULT_CODE res1 = AccountsService::postLogoutAccount(req1);
}

TEST(AccountsService_Authen, ValidAuthen_User)
{
    AuthenAccountRequest req("test_user", generateSHA1("123456789"));
    AuthenAccountResponse res = AccountsService::postAuthenAccount(req);
    EXPECT_EQ(RESULT_CODE::GENERAL_RESULT_SUCCESSFUL, res.Result);
    EXPECT_NE("", res.AuthKey);
    EXPECT_EQ("1", res.Role);
    LogoutAccountRequest req1("test_user");
    RESULT_CODE res1 = AccountsService::postLogoutAccount(req1);
}

TEST(AccountsService_Authen, Authenticated_User)
{
    AuthenAccountRequest req1("test_user", generateSHA1("123456789"));
    AuthenAccountResponse res1 = AccountsService::postAuthenAccount(req1);
    AuthenAccountRequest req2("test_user", generateSHA1("123456789"));
    AuthenAccountResponse res2 = AccountsService::postAuthenAccount(req2);
    EXPECT_EQ(RESULT_CODE::AUTHEN_ACCOUNT_AUTHENTICATED, res2.Result);
    EXPECT_EQ("", res2.AuthKey);
    EXPECT_EQ("", res2.Role);
    LogoutAccountRequest req3("test_user");
    RESULT_CODE res3 = AccountsService::postLogoutAccount(req3);
}

int main(int argc, char **argv)
{
    TestingServer::initializerServer();
    testing::InitGoogleTest(&argc, argv);
    int result = RUN_ALL_TESTS();
    TestingServer::shutdown();
    return 0;
}