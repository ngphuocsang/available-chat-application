#pragma once

#include <mysql/mysql.h>
#include <vector>

#include "Account.h"
#include "MyLibrary.h"

using namespace MyLibrary;
using namespace MyLibrary::Models;

namespace Server::MySQL::Functions
{
    class AccountFunctions
    {
    private:
        static MYSQL *_connection;
    public:
        static RESULT_CODE setDatabaseConnection(MYSQL *connection);
        static RESULT_CODE CreateNewAccount(Account account);
        static RESULT_CODE UpdateExistAccount(Account account);
        static RESULT_CODE DeleteExistAccount(string username);
        static RESULT_CODE UpdateAuthKey(string username, string authkey);
        static Account *ReadAccountByKey(string authkey);
        static Account *ReadAccountByUsername(string username);
        static vector<Account *> ReadAllAccounts();
    };
} // namespace Server::MySQL::Functions
