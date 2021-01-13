#pragma once

#include "RequestsAndResponses.h"
#include "BaseRestAPI.h"

using namespace MyLibrary::Models::Requests;
using namespace MyLibrary::Models::Responses;

namespace Client::RestAPI
{
    class AccountsAPI : public BaseRestAPI
    {
    public:
        static AuthenAccountResponse postAuthenAccount(AuthenAccountRequest req);
        static ActiveAccountsResponse getActiveAccounts(ActiveAccountsRequest req);
        static AccountsResponse getAllAccounts(AccountsRequest req);
        static RESULT_CODE postCreateAccount(CreateAccountRequest req);
        static RESULT_CODE postUpdateAccount(UpdateAccountRequest req);
        static RESULT_CODE postDeleteAccount(DeleteAccountRequest req);
        static RESULT_CODE postLogoutAccount(LogoutAccountRequest req);
    };
} // namespace Client::RestAPI
