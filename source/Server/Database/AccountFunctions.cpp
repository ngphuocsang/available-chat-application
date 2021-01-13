#include <sstream>
#include <iostream>
#include <string>

#include "AccountFunctions.h"

using namespace std;
using namespace MyLibrary::Models;

namespace Server::MySQL::Functions
{
    MYSQL *AccountFunctions::_connection = NULL;
    RESULT_CODE AccountFunctions::CreateNewAccount(Account account)
    {
        if (account.Role != "0" && account.Role != "1")
            account.Role = "1";
        stringstream ss;
        ss << "INSERT INTO `Account` (`USERNAME`, `Password`, `ROLES`) "
           << "VALUES ('" << account.Username
           << "', '" << account.Password
           << "', '" << account.Role << "');";
        if (mysql_query(_connection, ss.str().c_str()))
            return RESULT_CODE::RESULT_UNKNOWN;
        return RESULT_CODE::RESULT_SUCCESS;
    }
    RESULT_CODE AccountFunctions::DeleteExistAccount(string username)
    {
        stringstream ss;
        ss << "DELETE FROM `Account` WHERE `USERNAME` = '" << username << "';";
        if (mysql_query(_connection, ss.str().c_str()))
            return RESULT_CODE::RESULT_UNKNOWN;
        return RESULT_CODE::RESULT_SUCCESS;
    }
    RESULT_CODE AccountFunctions::UpdateExistAccount(Account account)
    {
        stringstream ss;
        ss << "UPDATE `Account` SET `ROLES` = '" << account.Role << "'"
           << ", `Password` = '" << account.Password << "'"
           << " WHERE `USERNAME` = '" << account.Username << "';";
        if (mysql_query(_connection, ss.str().c_str()))
            return RESULT_CODE::RESULT_UNKNOWN;
        return RESULT_CODE::RESULT_SUCCESS;
    }
    RESULT_CODE AccountFunctions::setDatabaseConnection(MYSQL *connection)
    {
        _connection = connection;
        return RESULT_CODE::RESULT_SUCCESS;
    }
    RESULT_CODE AccountFunctions::UpdateAuthKey(string username, string authkey)
    {
        stringstream ss;
        if (authkey == "NULL")
            ss << "UPDATE `Account` SET `AUTHEN_KEY` = " << authkey
               << " WHERE `USERNAME` = '" << username << "';";
        else
            ss << "UPDATE `Account` SET `AUTHEN_KEY` = '" << authkey << "'"
               << " WHERE `USERNAME` = '" << username << "';";
        if (mysql_query(_connection, ss.str().c_str()))
            return RESULT_CODE::RESULT_UNKNOWN;
        return RESULT_CODE::RESULT_SUCCESS;
    }

    Account *AccountFunctions::ReadAccountByKey(string authkey)
    {
        Account *account = NULL;
        stringstream ss;
        ss << "SELECT * FROM `Account` WHERE `AUTHEN_KEY` = '" << authkey << "';";
        mysql_query(_connection, ss.str().c_str());
        MYSQL_RES *rset = mysql_use_result(_connection);
        if (rset)
        {
            MYSQL_ROW rows;
            while ((rows = mysql_fetch_row(rset)))
            {
                string username = string(rows[1]);
                string password = string(rows[2]);
                string role = string(rows[3]);
                account = new Account(username, password, role, authkey);
                break;
            }
            mysql_free_result(rset);
        }
        return account;
    }

    Account *AccountFunctions::ReadAccountByUsername(string username)
    {
        Account *account = NULL;
        stringstream ss;
        ss << "SELECT * FROM `Account` WHERE `USERNAME` = '" << username << "';";
        int result = mysql_query(_connection, ss.str().c_str());
        MYSQL_RES *rset = mysql_use_result(_connection);
        if (rset)
        {
            MYSQL_ROW rows;
            while ((rows = mysql_fetch_row(rset)))
            {
                string username = string(rows[1]);
                string password = string(rows[2]);
                string role = string(rows[3]);
                string authkey = string(rows[4] ? rows[4] : "NULL");
                account = new Account(username, password, role, authkey);
                break;
            }
            mysql_free_result(rset);
        }
        return account;
    }

    vector<Account *> AccountFunctions::ReadAllAccounts()
    {
        vector<Account *> accounts;
        stringstream ss;
        ss << "SELECT * FROM `Account`;";
        mysql_query(_connection, ss.str().c_str());
        MYSQL_RES *rset = mysql_use_result(_connection);
        if (rset)
        {
            MYSQL_ROW rows;
            while ((rows = mysql_fetch_row(rset)))
            {
                string username = string(rows[1]);
                string password = string(rows[2]);
                string role = string(rows[3]);
                string authkey = string(rows[4] ? rows[4] : "NULL");
                Account *account = new Account(username, password, role, authkey);
                accounts.push_back(account);
            }
            mysql_free_result(rset);
        }
        return accounts;
    }
} // namespace Server::MySQL::Functions
