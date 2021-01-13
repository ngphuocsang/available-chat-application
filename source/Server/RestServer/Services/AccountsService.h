#pragma once

#include "Account.h"
#include "RequestsAndResponses.h"

#include <utility>

using namespace std;
using namespace MyLibrary::Models;
using namespace MyLibrary::Models::Responses;
using namespace MyLibrary::Models::Requests;

namespace Server::RestServer::Services
{
    class AccountsService
    {
    public:
        static ActiveAccountsResponse getActiveAccounts(ActiveAccountsRequest req);
        static AuthenAccountResponse postAuthenAccount(AuthenAccountRequest req);
        static AccountsResponse getAllAccounts(AccountsRequest req);
        static RESULT_CODE postCreateAccount(CreateAccountRequest req);
        static RESULT_CODE postUpdateAccount(UpdateAccountRequest req);
        static RESULT_CODE postDeleteAccount(DeleteAccountRequest req);
        static RESULT_CODE postLogoutAccount(LogoutAccountRequest req);
    };
} // namespace Server::RestServer::Services