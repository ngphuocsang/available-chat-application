#include "AccountsService.h"
#include "AccountFunctions.h"

using namespace Server::MySQL::Functions;

namespace Server::RestServer::Services
{
    ActiveAccountsResponse AccountsService::getActiveAccounts(
        ActiveAccountsRequest req)
    {
        ActiveAccountsResponse res;
        Account *account = AccountFunctions::ReadAccountByKey(req.AuthKey);
        if (!account)
        {
            res.Result = RESULT_CODE::ACCOUNT_UNAUTHENTICATION;
            delete account;
            return res;
        }

        vector<string> usernames;
        vector<Account *> accounts = AccountFunctions::ReadAllAccounts();
        for (auto item : accounts)
        {
            if (item->AuthKey != "NULL" && item->Role != "0")
                usernames.push_back(item->Username);
            delete item;
        }

        res.Result = RESULT_CODE::GENERAL_RESULT_SUCCESSFUL;
        res.Usernames = usernames;
        delete account;
        return res;
    }

    AuthenAccountResponse AccountsService::postAuthenAccount(AuthenAccountRequest req)
    {
        AuthenAccountResponse res;

        Account *account = AccountFunctions::ReadAccountByUsername(req.Username);
        if (!account)
        {
            res.Result = RESULT_CODE::AUTHEN_ACCOUNT_INVALID_USERNAME;
            delete account;
            return res;
        }
        if (generateSHA1(account->Password) != req.Password)
        {
            res.Result = RESULT_CODE::AUTHEN_ACCOUNT_INVALID_PASSWORD;
            delete account;
            return res;
        }
        if (account->AuthKey != "NULL" && account->Role != "0")
        {
            res.Result = RESULT_CODE::AUTHEN_ACCOUNT_AUTHENTICATED;
            delete account;
            return res;
        }
        res.Result = RESULT_CODE::GENERAL_RESULT_SUCCESSFUL;
        string authkey = generateAuthenKey();
        if (AccountFunctions::UpdateAuthKey(req.Username, authkey))
        {
            res.Result = RESULT_CODE::GENERAL_RESULT_UNSUCCESSFUL;
            delete account;
            return res;
        }

        res.AuthKey = authkey;
        res.Role = account->Role;
        delete account;
        return res;
    }

    AccountsResponse AccountsService::getAllAccounts(AccountsRequest req)
    {
        AccountsResponse res;
        Account *account = AccountFunctions::ReadAccountByKey(req.AuthKey);
        if (!account)
        {
            res.Result = RESULT_CODE::ACCOUNT_UNAUTHENTICATION;
            delete account;
            return res;
        }
        vector<Account *> accounts = AccountFunctions::ReadAllAccounts();
        for (auto item : accounts)
        {
            item->AuthKey = "";
            item->Password = "";
            res.Accounts.push_back(*item);
            delete item;
        }
        res.Result = RESULT_CODE::GENERAL_RESULT_SUCCESSFUL;
        delete account;
        return res;
    }

    RESULT_CODE AccountsService::postCreateAccount(CreateAccountRequest req)
    {
        Account *account = AccountFunctions::ReadAccountByKey(req.AuthKey);
        if (!account)
        {
            delete account;
            return RESULT_CODE::ACCOUNT_UNAUTHENTICATION;
        }
        if (account->Role != "0")
        {
            delete account;
            return RESULT_CODE::ACCOUNT_UNAUTHORIZATION;
        }
        Account *newAccount = new Account(req.Username, req.Password, req.Role);
        account = AccountFunctions::ReadAccountByUsername(newAccount->Username);
        if (!account)
            account = new Account("", "", "", "");
        if (newAccount->Username.length() == 0 ||
            newAccount->Username.length() > 16 ||
            !is_valid_username(newAccount->Username) ||
            account->Username == newAccount->Username)
        {
            delete account;
            delete newAccount;
            return RESULT_CODE::CREATE_ACCOUNT_INVALID_USERNAME;
        }
        if (newAccount->Role != "0" && newAccount->Role != "1")
        {
            delete account;
            delete newAccount;
            return RESULT_CODE::CREATE_ACCOUNT_INVALID_ROLE;
        }
        if (!is_digits(newAccount->Password) ||
            newAccount->Password.length() == 0 ||
            newAccount->Password.length() > 9)
        {
            delete account;
            delete newAccount;
            return RESULT_CODE::CREATE_ACCOUNT_INVALID_PASSWORD;
        }
        if (AccountFunctions::CreateNewAccount(*newAccount))
        {
            delete account;
            delete newAccount;
            return RESULT_CODE::GENERAL_RESULT_UNSUCCESSFUL;
        }
        delete account;
        delete newAccount;
        return RESULT_CODE::GENERAL_RESULT_SUCCESSFUL;
    }

    RESULT_CODE AccountsService::postUpdateAccount(UpdateAccountRequest req)
    {
        Account *currentAccount = AccountFunctions::ReadAccountByKey(req.AuthKey);
        if (!currentAccount)
        {
            delete currentAccount;
            return RESULT_CODE::ACCOUNT_UNAUTHENTICATION;
        }
        Account *changingAccount = AccountFunctions::ReadAccountByUsername(req.Username);

        if (!changingAccount)
        {
            delete currentAccount;
            delete changingAccount;
            return RESULT_CODE::UPDATE_ACCOUNT_INVALID_USERNAME;
        }

        if (req.Role != "")
        {
            if (currentAccount->Role != "0")
            {
                delete currentAccount;
                delete changingAccount;
                return RESULT_CODE::ACCOUNT_UNAUTHORIZATION;
            }
            if (req.Role != "0" && req.Role != "1")
            {
                delete currentAccount;
                delete changingAccount;
                return RESULT_CODE::UPDATE_ACCOUNT_INVALID_ROLE;
            }
            changingAccount->Role = req.Role;
            if (AccountFunctions::UpdateExistAccount(*changingAccount))
            {
                delete currentAccount;
                delete changingAccount;
                return RESULT_CODE::GENERAL_RESULT_UNSUCCESSFUL;
            }
        }
        if (req.Password != "")
        {
            if (currentAccount->Username != changingAccount->Username)
            {
                delete currentAccount;
                delete changingAccount;
                return RESULT_CODE::ACCOUNT_UNAUTHORIZATION;
            }
            if (!is_digits(req.Password) ||
                req.Password.length() == 0 ||
                req.Password.length() > 9)
            {
                delete currentAccount;
                delete changingAccount;
                return RESULT_CODE::UPDATE_ACCOUNT_INVALID_PASSWORD;
            }
            changingAccount->Password = req.Password;
            if (AccountFunctions::UpdateExistAccount(*changingAccount))
            {
                delete currentAccount;
                delete changingAccount;
                return RESULT_CODE::GENERAL_RESULT_UNSUCCESSFUL;
            }
        }
        return RESULT_CODE::GENERAL_RESULT_SUCCESSFUL;
    }

    RESULT_CODE AccountsService::postDeleteAccount(DeleteAccountRequest req)
    {
        Account *account = AccountFunctions::ReadAccountByKey(req.AuthKey);
        if (!account)
        {
            delete account;
            return RESULT_CODE::ACCOUNT_UNAUTHENTICATION;
        }
        if (account->Role != "0")
        {
            delete account;
            return RESULT_CODE::ACCOUNT_UNAUTHORIZATION;
        }

        Account *deletingAccount = AccountFunctions::ReadAccountByUsername(req.Username);
        if (!deletingAccount)
        {
            delete account;
            delete deletingAccount;
            return RESULT_CODE::DELETE_ACCOUNT_INVALID_USERNAME;
        }
        if (AccountFunctions::DeleteExistAccount(req.Username))
        {
            delete account;
            delete deletingAccount;
            return RESULT_CODE::GENERAL_RESULT_UNSUCCESSFUL;
        }
        return RESULT_CODE::GENERAL_RESULT_SUCCESSFUL;
    }

    RESULT_CODE AccountsService::postLogoutAccount(LogoutAccountRequest req)
    {
        if (AccountFunctions::UpdateAuthKey(req.Username, "NULL"))
            return RESULT_CODE::GENERAL_RESULT_UNSUCCESSFUL;
        return RESULT_CODE::GENERAL_RESULT_SUCCESSFUL;
    }
} // namespace Server::RestServer::Services
