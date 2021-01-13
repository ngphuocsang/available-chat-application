#include <gtest/gtest.h>

#include "helper.h"
#include "AccountsService.h"


using namespace MyLibrary;
using namespace Server::RestServer::Services;
using namespace MyLibrary::Models::Responses;
using namespace MyLibrary::Models::Requests;

TEST(AccountsService_Accounts, Unauthentication)
{
    AccountsRequest req("");
    AccountsResponse res = AccountsService::getAllAccounts(req);
    EXPECT_EQ(RESULT_CODE::ACCOUNT_UNAUTHENTICATION, res.Result);
    EXPECT_EQ(0, res.Accounts.size());
}

TEST(AccountsService_Accounts, ValidGetAccounts)
{
    AuthenAccountRequest req1("test_admin", generateSHA1("000000000"));
    AuthenAccountResponse res1 = AccountsService::postAuthenAccount(req1);
    AccountsRequest req2(res1.AuthKey);
    AccountsResponse res2 = AccountsService::getAllAccounts(req2);
    EXPECT_EQ(RESULT_CODE::GENERAL_RESULT_SUCCESSFUL, res2.Result);
    EXPECT_EQ(5, res2.Accounts.size());
    if (res2.Accounts.size() == 5)
    {
        EXPECT_EQ("admin", res2.Accounts[0].Username);
        EXPECT_EQ("0", res2.Accounts[0].Role);
        EXPECT_EQ("test_admin", res2.Accounts[1].Username);
        EXPECT_EQ("0", res2.Accounts[1].Role);
        EXPECT_EQ("test_user", res2.Accounts[2].Username);
        EXPECT_EQ("1", res2.Accounts[2].Role);
        EXPECT_EQ("test_user1", res2.Accounts[3].Username);
        EXPECT_EQ("1", res2.Accounts[3].Role);
        EXPECT_EQ("test_user2", res2.Accounts[4].Username);
        EXPECT_EQ("1", res2.Accounts[4].Role);
    }
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