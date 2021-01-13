#include <gtest/gtest.h>

#include "helper.h"
#include "AccountsService.h"


using namespace MyLibrary;
using namespace Server::RestServer::Services;
using namespace MyLibrary::Models::Responses;
using namespace MyLibrary::Models::Requests;

int isWaiting = 0;

TEST(AccountsService_Active, Unauthentication)
{
    ActiveAccountsRequest req("");
    ActiveAccountsResponse res = AccountsService::getActiveAccounts(req);
    EXPECT_EQ(RESULT_CODE::ACCOUNT_UNAUTHENTICATION, res.Result);
    EXPECT_EQ(0, res.Usernames.size());
}

TEST(AccountsService_Active, ValidGetActives_Empty)
{
    while(isWaiting);
    isWaiting = 1;
    AuthenAccountRequest req1("test_admin", generateSHA1("000000000"));
    AuthenAccountResponse res1 = AccountsService::postAuthenAccount(req1);
    ActiveAccountsRequest req2(res1.AuthKey);
    ActiveAccountsResponse res2 = AccountsService::getActiveAccounts(req2);
    EXPECT_EQ(RESULT_CODE::GENERAL_RESULT_SUCCESSFUL, res2.Result);
    EXPECT_EQ(0, res2.Usernames.size());
    LogoutAccountRequest req3("test_admin");
    RESULT_CODE res3 = AccountsService::postLogoutAccount(req3);
    isWaiting = 0;
}

TEST(AccountsService_Active, ValidGetActives)
{
    while(isWaiting);
    isWaiting = 1;
    AuthenAccountRequest req("test_admin", generateSHA1("000000000"));
    AuthenAccountResponse res = AccountsService::postAuthenAccount(req);
    AuthenAccountRequest req1("test_user1", generateSHA1("123456789"));
    AuthenAccountResponse res1 = AccountsService::postAuthenAccount(req1);
    AuthenAccountRequest req2("test_user2", generateSHA1("123456789"));
    AuthenAccountResponse res2 = AccountsService::postAuthenAccount(req2);
    ActiveAccountsRequest req3(res.AuthKey);
    ActiveAccountsResponse res3 = AccountsService::getActiveAccounts(req3);
    EXPECT_EQ(RESULT_CODE::GENERAL_RESULT_SUCCESSFUL, res3.Result);
    EXPECT_EQ(2, res3.Usernames.size());
    if (2 == res3.Usernames.size())
    {
        EXPECT_EQ(res3.Usernames[0], "test_user1");
        EXPECT_EQ(res3.Usernames[1], "test_user2");
    }
    LogoutAccountRequest req4("test_admin");
    RESULT_CODE res4 = AccountsService::postLogoutAccount(req4);
    LogoutAccountRequest req5("test_user1");
    RESULT_CODE res5 = AccountsService::postLogoutAccount(req5);
    LogoutAccountRequest req6("test_user2");
    RESULT_CODE res6 = AccountsService::postLogoutAccount(req6);
    isWaiting = 0;
}

int main(int argc, char **argv)
{
    TestingServer::initializerServer();
    testing::InitGoogleTest(&argc, argv);
    int result = RUN_ALL_TESTS();
    TestingServer::shutdown();
    return 0;
}