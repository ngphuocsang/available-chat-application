#include <gtest/gtest.h>

#include "helper.h"
#include "AccountsService.h"


using namespace Server::RestServer::Services;

TEST(AccountsService_Create, Unauthentication)
{
    CreateAccountRequest req("");
    RESULT_CODE res = AccountsService::postCreateAccount(req);
    EXPECT_EQ(RESULT_CODE::ACCOUNT_UNAUTHENTICATION, res);
}

TEST(AccountsService_Create, Unauthorization)
{
    AuthenAccountRequest req1("test_user", generateSHA1("123456789"));
    AuthenAccountResponse res1 = AccountsService::postAuthenAccount(req1);
    CreateAccountRequest req2(res1.AuthKey);
    RESULT_CODE res2 = AccountsService::postCreateAccount(req2);
    EXPECT_EQ(RESULT_CODE::ACCOUNT_UNAUTHORIZATION, res2);
    LogoutAccountRequest req3("test_user");
    RESULT_CODE res3 = AccountsService::postLogoutAccount(req3);
}

TEST(AccountsService_Create, InvalidUsername_Duplicated)
{
    AuthenAccountRequest req1("test_admin", generateSHA1("000000000"));
    AuthenAccountResponse res1 = AccountsService::postAuthenAccount(req1);
    CreateAccountRequest req2(res1.AuthKey);
    req2.Username = "test_user";
    req2.Role = "1";
    req2.Password = "123456789";
    RESULT_CODE res2 = AccountsService::postCreateAccount(req2);
    EXPECT_EQ(RESULT_CODE::CREATE_ACCOUNT_INVALID_USERNAME, res2);
    LogoutAccountRequest req3("test_admin");
    RESULT_CODE res3 = AccountsService::postLogoutAccount(req3);
}

TEST(AccountsService_Create, InvalidUsername_Character)
{
    AuthenAccountRequest req1("test_admin", generateSHA1("000000000"));
    AuthenAccountResponse res1 = AccountsService::postAuthenAccount(req1);
    CreateAccountRequest req2(res1.AuthKey);
    req2.Username = "aafsdf/-_.-4";
    req2.Role = "1";
    req2.Password = "123456789";
    RESULT_CODE res2 = AccountsService::postCreateAccount(req2);
    EXPECT_EQ(RESULT_CODE::CREATE_ACCOUNT_INVALID_USERNAME, res2);
    LogoutAccountRequest req3("test_admin");
    RESULT_CODE res3 = AccountsService::postLogoutAccount(req3);
}

TEST(AccountsService_Create, InvalidUsername_Length)
{
    AuthenAccountRequest req1("test_admin", generateSHA1("000000000"));
    AuthenAccountResponse res1 = AccountsService::postAuthenAccount(req1);
    CreateAccountRequest req2(res1.AuthKey);
    req2.Username = "___________________________";
    req2.Role = "1";
    req2.Password = "123456789";
    RESULT_CODE res2 = AccountsService::postCreateAccount(req2);
    EXPECT_EQ(RESULT_CODE::CREATE_ACCOUNT_INVALID_USERNAME, res2);
    LogoutAccountRequest req3("test_admin");
    RESULT_CODE res3 = AccountsService::postLogoutAccount(req3);
}

TEST(AccountsService_Create, InvalidRole)
{
    AuthenAccountRequest req1("test_admin", generateSHA1("000000000"));
    AuthenAccountResponse res1 = AccountsService::postAuthenAccount(req1);
    CreateAccountRequest req2(res1.AuthKey);
    req2.Username = "test_user3";
    req2.Role = "3";
    req2.Password = "123456789";
    RESULT_CODE res2 = AccountsService::postCreateAccount(req2);
    EXPECT_EQ(RESULT_CODE::CREATE_ACCOUNT_INVALID_ROLE, res2);
    LogoutAccountRequest req3("test_admin");
    RESULT_CODE res3 = AccountsService::postLogoutAccount(req3);
}

TEST(AccountsService_Create, InvalidPassword_Character)
{
    AuthenAccountRequest req1("test_admin", generateSHA1("000000000"));
    AuthenAccountResponse res1 = AccountsService::postAuthenAccount(req1);
    CreateAccountRequest req2(res1.AuthKey);
    req2.Username = "test_user3";
    req2.Role = "1";
    req2.Password = "alskjd";
    RESULT_CODE res2 = AccountsService::postCreateAccount(req2);
    EXPECT_EQ(RESULT_CODE::CREATE_ACCOUNT_INVALID_PASSWORD, res2);
    LogoutAccountRequest req3("test_admin");
    RESULT_CODE res3 = AccountsService::postLogoutAccount(req3);
}

TEST(AccountsService_Create, InvalidPassword_Length)
{
    AuthenAccountRequest req1("test_admin", generateSHA1("000000000"));
    AuthenAccountResponse res1 = AccountsService::postAuthenAccount(req1);
    CreateAccountRequest req2(res1.AuthKey);
    req2.Username = "test_user3";
    req2.Role = "1";
    req2.Password = "1234567890";
    RESULT_CODE res2 = AccountsService::postCreateAccount(req2);
    EXPECT_EQ(RESULT_CODE::CREATE_ACCOUNT_INVALID_PASSWORD, res2);
    LogoutAccountRequest req3("test_admin");
    RESULT_CODE res3 = AccountsService::postLogoutAccount(req3);
}

TEST(AccountsService_Create, ValidCreateAccount)
{
    AuthenAccountRequest req1("test_admin", generateSHA1("000000000"));
    AuthenAccountResponse res1 = AccountsService::postAuthenAccount(req1);
    CreateAccountRequest req2(res1.AuthKey);
    req2.Username = "test_user3";
    req2.Role = "1";
    req2.Password = "123456789";
    RESULT_CODE res2 = AccountsService::postCreateAccount(req2);
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