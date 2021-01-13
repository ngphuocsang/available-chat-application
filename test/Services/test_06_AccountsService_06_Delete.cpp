#include <gtest/gtest.h>

#include "helper.h"
#include "AccountsService.h"


using namespace Server::RestServer::Services;

TEST(AccountsService_Delete, Unauthentication)
{
    DeleteAccountRequest req("");
    RESULT_CODE res = AccountsService::postDeleteAccount(req);
    EXPECT_EQ(RESULT_CODE::ACCOUNT_UNAUTHENTICATION, res);
}

TEST(AccountsService_Delete, Unauthorization)
{
    AuthenAccountRequest req1("test_user", generateSHA1("123456789"));
    AuthenAccountResponse res1 = AccountsService::postAuthenAccount(req1);
    DeleteAccountRequest req2(res1.AuthKey);
    req2.Username = "test_user3";
    RESULT_CODE res2 = AccountsService::postDeleteAccount(req2);
    EXPECT_EQ(RESULT_CODE::ACCOUNT_UNAUTHORIZATION, res2);
    LogoutAccountRequest req3("test_user");
    RESULT_CODE res3 = AccountsService::postLogoutAccount(req3);
}

TEST(AccountsService_Delete, InvalidUsername)
{
    AuthenAccountRequest req1("test_admin", generateSHA1("000000000"));
    AuthenAccountResponse res1 = AccountsService::postAuthenAccount(req1);
    DeleteAccountRequest req2(res1.AuthKey);
    req2.Username = "__________";
    RESULT_CODE res2 = AccountsService::postDeleteAccount(req2);
    EXPECT_EQ(RESULT_CODE::DELETE_ACCOUNT_INVALID_USERNAME, res2);
    LogoutAccountRequest req3("test_admin");
    RESULT_CODE res3 = AccountsService::postLogoutAccount(req3);
}

TEST(AccountsService_Delete, ValidDeleteAccount)
{
    AuthenAccountRequest req1("test_admin", generateSHA1("000000000"));
    AuthenAccountResponse res1 = AccountsService::postAuthenAccount(req1);
    DeleteAccountRequest req2(res1.AuthKey);
    req2.Username = "test_user3";
    RESULT_CODE res2 = AccountsService::postDeleteAccount(req2);
    EXPECT_EQ(RESULT_CODE::GENERAL_RESULT_SUCCESSFUL, res2);
    LogoutAccountRequest req3("test_admin");
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