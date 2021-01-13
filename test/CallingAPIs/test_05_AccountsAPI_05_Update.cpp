#include <gtest/gtest.h>

#include "AccountsAPI.h"

#include "helper.h"

using namespace std;
using namespace Server::RestServer;
using namespace MyLibrary;
using namespace Client::RestAPI;
using namespace MyLibrary::Models::Requests;
using namespace MyLibrary::Models::Responses;

TEST(AccountsAPI_Update, Unauthentication)
{
    UpdateAccountRequest req("");
    RESULT_CODE res = AccountsAPI::postUpdateAccount(req);
    EXPECT_EQ(RESULT_CODE::ACCOUNT_UNAUTHENTICATION, res);
}

TEST(AccountsAPI_Update, ChangePassword_Unauthorization)
{
    AuthenAccountRequest req1("test_admin", "000000000");
    AuthenAccountResponse res1 = AccountsAPI::postAuthenAccount(req1);
    UpdateAccountRequest req2(res1.AuthKey);
    req2.Username = "test_user3";
    req2.Password = "000000000";
    req2.Role = "";
    RESULT_CODE res2 = AccountsAPI::postUpdateAccount(req2);
    EXPECT_EQ(RESULT_CODE::ACCOUNT_UNAUTHORIZATION, res2);
    LogoutAccountRequest req3("test_admin");
    RESULT_CODE res3 = AccountsAPI::postLogoutAccount(req3);
}

TEST(AccountsAPI_Update, ChangePassword_InvalidLength)
{
    AuthenAccountRequest req1("test_user3", "123456789");
    AuthenAccountResponse res1 = AccountsAPI::postAuthenAccount(req1);
    UpdateAccountRequest req2(res1.AuthKey);
    req2.Username = "test_user3";
    req2.Role = "";
    req2.Password = "1234567890";
    RESULT_CODE res2 = AccountsAPI::postUpdateAccount(req2);
    EXPECT_EQ(RESULT_CODE::UPDATE_ACCOUNT_INVALID_PASSWORD, res2);
    LogoutAccountRequest req3("test_user3");
    RESULT_CODE res3 = AccountsAPI::postLogoutAccount(req3);
}

TEST(AccountsAPI_Update, ChangePassword_InvalidCharacter)
{
    AuthenAccountRequest req1("test_user3", "123456789");
    AuthenAccountResponse res1 = AccountsAPI::postAuthenAccount(req1);
    UpdateAccountRequest req2(res1.AuthKey);
    req2.Username = "test_user3";
    req2.Role = "";
    req2.Password = "12345678_";
    RESULT_CODE res2 = AccountsAPI::postUpdateAccount(req2);
    EXPECT_EQ(RESULT_CODE::UPDATE_ACCOUNT_INVALID_PASSWORD, res2);
    LogoutAccountRequest req3("test_user3");
    RESULT_CODE res3 = AccountsAPI::postLogoutAccount(req3);
}

TEST(AccountsAPI_Update, ChangePassword_Valid)
{
    AuthenAccountRequest req1("test_user3", "123456789");
    AuthenAccountResponse res1 = AccountsAPI::postAuthenAccount(req1);
    UpdateAccountRequest req2(res1.AuthKey);
    req2.Username = "test_user3";
    req2.Role = "";
    req2.Password = "000000000";
    RESULT_CODE res2 = AccountsAPI::postUpdateAccount(req2);
    EXPECT_EQ(RESULT_CODE::GENERAL_RESULT_SUCCESSFUL, res2);
    LogoutAccountRequest req3("test_user3");
    RESULT_CODE res3 = AccountsAPI::postLogoutAccount(req3);
}

TEST(AccountsAPI_Update, ChangeRole_Unauthorization)
{
    AuthenAccountRequest req1("test_user", "123456789");
    AuthenAccountResponse res1 = AccountsAPI::postAuthenAccount(req1);
    UpdateAccountRequest req2(res1.AuthKey);
    req2.Username = "test_user3";
    req2.Password = "";
    req2.Role = "1";
    RESULT_CODE res2 = AccountsAPI::postUpdateAccount(req2);
    EXPECT_EQ(RESULT_CODE::ACCOUNT_UNAUTHORIZATION, res2);
    LogoutAccountRequest req3("test_user");
    RESULT_CODE res3 = AccountsAPI::postLogoutAccount(req3);
}

TEST(AccountsAPI_Update, ChangeRole_InvalidRole)
{
    AuthenAccountRequest req1("test_admin", "000000000");
    AuthenAccountResponse res1 = AccountsAPI::postAuthenAccount(req1);
    UpdateAccountRequest req2(res1.AuthKey);
    req2.Username = "test_user3";
    req2.Role = "3";
    req2.Password = "";
    RESULT_CODE res2 = AccountsAPI::postUpdateAccount(req2);
    EXPECT_EQ(RESULT_CODE::UPDATE_ACCOUNT_INVALID_ROLE, res2);
    LogoutAccountRequest req3("test_user3");
    RESULT_CODE res3 = AccountsAPI::postLogoutAccount(req3);
}

TEST(AccountsAPI_Update, ChangeRole_Valid)
{
    AuthenAccountRequest req1("test_admin", "000000000");
    AuthenAccountResponse res1 = AccountsAPI::postAuthenAccount(req1);
    UpdateAccountRequest req2(res1.AuthKey);
    req2.Username = "test_user3";
    req2.Role = "0";
    req2.Password = "";
    RESULT_CODE res2 = AccountsAPI::postUpdateAccount(req2);
    EXPECT_EQ(RESULT_CODE::GENERAL_RESULT_SUCCESSFUL, res2);
    LogoutAccountRequest req3("test_admin");
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