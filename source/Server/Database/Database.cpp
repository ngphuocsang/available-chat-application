
#include "AccountFunctions.h"
#include "Database.h"
#include "MyLibrary.h"
#include <unordered_map>
#include <fstream>
#include <ctime>

using namespace MyLibrary;
using namespace Server::MySQL::Functions;

Database *Database::_instance = NULL;

Database::Database() {}

Database *Database::getInstance()
{
    if (_instance == NULL)
        _instance = new Database();
    return _instance;
}

int Database::initialize(string host, string user, string pass, string dbname)
{
    db_conn = mysql_init(NULL);
    bool reconnect = 1;
    mysql_options(db_conn, MYSQL_OPT_RECONNECT, &reconnect);
    if (!db_conn)
        return RESULT_CODE::DB_SERV_INIT_ERR;

    db_conn = mysql_real_connect(
        db_conn, host.c_str(), user.c_str(), pass.c_str(), dbname.c_str(), 3306, NULL, 0);

    if (!db_conn)
        return RESULT_CODE::DB_SERV_INIT_ERR;
    AccountFunctions::setDatabaseConnection(db_conn);
    return RESULT_CODE::RESULT_SUCCESS;
}

int Database::shutdown()
{
    mysql_close(db_conn);
    return RESULT_CODE::RESULT_SUCCESS;
}

bool Database::updateRole(string username, string role)
{
    stringstream ss;
    if (role != "0" && role != "1")
    {
        printf("Invalid role!\n");
        return 0;
    }
    ss << "UPDATE Account SET ROLES = '" << role << "'WHERE USERNAME =  '" << username << "'";
    if (!mysql_query(db_conn, ss.str().c_str()))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

bool Database::updatePassword(string username, string password)
{
    stringstream ss;
    ss << "UPDATE Account SET PASSWORD = '" << password << "'WHERE USERNAME =  '" << username << "'";
    if (!mysql_query(db_conn, ss.str().c_str()))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

vector<Account> Database::readAllAccounts()
{
    vector<Account> accounts;
    MYSQL_RES *rset;
    MYSQL_ROW rows;
    string sql = "SELECT * FROM Account";
    if (mysql_query(db_conn, sql.c_str()))
    {
        printf("Error printing all accounts!\n");
        return accounts;
    }
    rset = mysql_use_result(db_conn);

    if (rset)
    {
        while ((rows = mysql_fetch_row(rset)))
        {
            Account *a = new Account(string(rows[1]), string(rows[2]), string(rows[3]));
            accounts.push_back(*a);
        }
    }
    mysql_free_result(rset);
    return accounts;
}

bool Database::createAccount(Account *account)
{
    stringstream ss;
    ss << "INSERT INTO Account "
       << "VALUES (0,'" << account->Username << "', '"
       << account->Password << "','"
       << account->Role << "')";
    if (mysql_query(db_conn, ss.str().c_str()))
        return 0;
    else
        return 1;
}

bool Database::deleteAccount(string username)
{
    stringstream ss;
    ss << "DELETE FROM Account WHERE username='"
       << username << "'";

    if (mysql_query(db_conn, ss.str().c_str()))
        return 0;
    else
        return 1;
}

vector<string> Database::getRoomUser(int room_id)
{
    vector<string> result;
    MYSQL_RES *rset;
    // MYSQL_COLS column;
    MYSQL_ROW rows;
    vector<string> fail(1);
    fail[0] = "FAIL\n";

    stringstream ss;
    ss << "SELECT * FROM chat_room_account WHERE ROOM_ID=" << room_id << ";";
    if (mysql_query(db_conn, ss.str().c_str()))
    {
        return fail;
    }
    rset = mysql_use_result(db_conn);
    if (rset)
    {
        while ((rows = mysql_fetch_row(rset)))
        {
            result.push_back(rows[1]);
        }
    }
    mysql_free_result(rset);
    return result;
}

int Database::createChatRoom(vector<string> username)
{
    MYSQL_RES *res;
    MYSQL_ROW rows;
    stringstream ss;
    string sql;
    unordered_map<int, int> chat_room_count;

    if (username.empty())
        return -1;

    //store number of user in each chat room
    sql = "SELECT ROOM_ID, COUNT(*) FROM chat_room_account GROUP BY ROOM_ID";
    if (int error = mysql_query(db_conn, sql.c_str()))
    {
        cout << mysql_error(db_conn) << endl;

        cout << "error code: " << error << endl;

        return -2;
    }
    res = mysql_use_result(db_conn);
    while ((rows = mysql_fetch_row(res)))
    {
        chat_room_count[atoi(rows[0])] = atoi(rows[1]);
    }
    mysql_free_result(res);

    //filter out user not in username list
    ss << "SELECT ROOM_ID, COUNT(*) FROM chat_room_account WHERE ACC_USERNAME = '" << username[0];
    for (int i = 1; i < username.size(); i++)
    {
        ss << "' OR ACC_USERNAME = '" << username[i];
    }
    ss << "' GROUP BY ROOM_ID";
    if (mysql_query(db_conn, ss.str().c_str()))
    {
        printf("Error\n");
        return -3;
    }
    res = mysql_use_result(db_conn);
    //if exist number of user before and after filtering should be equal username list length
    int a;
    bool flag = false;
    while ((rows = mysql_fetch_row(res)))
    {
        if (atoi(rows[1]) == username.size() && chat_room_count[atoi(rows[0])] == username.size())
        {
            a = atoi(rows[0]);
            flag = true;
        }
    }

    //if not exist create new chat room
    mysql_free_result(res);
    if (flag)
        return a;
    sql = "INSERT INTO chat_room VALUES (0,NULL)";
    if (mysql_query(db_conn, sql.c_str()))
    {
        return -4;
    }
    sql = "SELECT LAST_INSERT_ID()";
    if (mysql_query(db_conn, sql.c_str()))
    {
        return -5;
    }

    res = mysql_use_result(db_conn);
    int room_id;
    while ((rows = mysql_fetch_row(res)))
    {
        room_id = atoi(rows[0]);
    }
    mysql_free_result(res);
    stringstream s;
    s << "UPDATE chat_room SET message_history = './chat_history/" << room_id << ".txt' WHERE ROOM_ID = " << room_id;
    if (mysql_query(db_conn, s.str().c_str()))
    {
        return -6;
    }
    for (int i = 0; i < username.size(); i++)
    {
        stringstream str;
        str << "INSERT INTO chat_room_account VALUES (" << room_id << ",'" << username[i] << "');";
        if (mysql_query(db_conn, str.str().c_str()))
        {
            printf("Error1\n");
            return -7;
        }
    }
    return room_id;
}

Account *Database::readAccount(string username)
{
    Account *result = NULL;
    MYSQL_RES *rset;
    MYSQL_ROW rows;

    stringstream ss;
    ss << "SELECT * FROM Account WHERE USERNAME='" << username << "';";
    if (mysql_query(db_conn, ss.str().c_str()))
    {
        return NULL;
    }
    rset = mysql_use_result(db_conn);
    if (rset)
    {
        while ((rows = mysql_fetch_row(rset)))
        {
            Account *a = new Account(string(rows[1]), string(rows[2]), string(rows[3]));
            result = a;
        }
        mysql_free_result(rset);
        return result;
    }
}

bool Database::addMessage(string msg, int room_id)
{
    stringstream ss;
    ss << "SELECT message_history FROM chat_room WHERE ROOM_ID = " << room_id;
    if (mysql_query(db_conn, ss.str().c_str()))
    {
        return 0;
    }
    MYSQL_RES *res = mysql_use_result(db_conn);
    MYSQL_ROW rows;
    string directory;
    if (res)
    {
        while ((rows = mysql_fetch_row(res)))
        {
            directory = string(rows[0]);
        }
    }
    mysql_free_result(res);
    fstream outfile;
    outfile.open(directory, ios::app);
    if (!outfile)
    {
        outfile.open(directory, ios::trunc);
        outfile.close();
    }
    else
    {
        time_t now = time(0);
        tm *ltm = localtime(&now);
        char buffer[80];
        strftime(buffer, 80, "[%d/%m/%Y %H:%M:%S]", ltm);
        outfile << buffer << msg << endl;
        outfile.close();
    }
    return 1;
}

// int Database::readMessage(int room_id)
// {
//     stringstream ss;
//     ss << "SELECT message_history FROM chat_room WHERE ROOM_ID = " << room_id;
//     if (mysql_query(db_conn, ss.str().c_str()))
//     {
//         return 0;
//     }
//     MYSQL_RES *res = mysql_use_result(db_conn);
//     MYSQL_ROW rows;
//     string directory;
//     if (res)
//     {
//         while ((rows = mysql_fetch_row(res)))
//         {
//             directory = string(rows[0]);
//         }
//     }
//     mysql_free_result(res);

//     ifstream infile;
//     string c;
//     infile.open(directory);
//     if (!infile.is_open())
//     {
//         return -1;
//     }
//     else
//     {
//         while (!infile.eof())
//         {
//             getline(infile, c);
//             cout << c << endl;
//         }
//     }
//     infile.close();
//     return 1;
// }

string Database::readMessage(int room_id)
{
    stringstream ss;
    ss << "SELECT message_history FROM chat_room WHERE ROOM_ID = " << room_id;
    if (mysql_query(db_conn, ss.str().c_str()))
    {
        return "0";
    }
    MYSQL_RES *res = mysql_use_result(db_conn);
    MYSQL_ROW rows;
    string directory;
    if (res)
    {
        while ((rows = mysql_fetch_row(res)))
        {
            directory = string(rows[0]);
        }
    }
    mysql_free_result(res);

    return directory;
}

int Database::readRecentMessage(int room_id)
{
    stringstream ss;
    ss << "SELECT message_history FROM chat_room WHERE ROOM_ID = " << room_id;
    if (mysql_query(db_conn, ss.str().c_str()))
    {
        return 0;
    }
    MYSQL_RES *res = mysql_use_result(db_conn);
    MYSQL_ROW rows;
    string directory;
    if (res)
    {
        while ((rows = mysql_fetch_row(res)))
        {
            directory = string(rows[0]);
        }
    }
    mysql_free_result(res);
    ifstream infile;
    string c;
    infile.open(directory);
    if (!infile.is_open())
    {
        return -1;
    }
    else
    {
        while (!infile.eof())
        {
            getline(infile, c);
            string temp = c.substr(0, c.find("]"));
            struct tm ltm;
            strptime(temp.c_str(), "[%d/%m/%Y %H:%M:%S", &ltm);
            time_t t1 = mktime(&ltm);
            time_t now = time(0);
            if (difftime(now, t1) <= 86400)
                cout << c << endl;
        }
    }
    infile.close();
    return 1;
}

vector<string> Database::fetchAllUser()
{
    if (db_conn)
    {
        vector<string> result;
        MYSQL_RES *rset;
        MYSQL_ROW rows;
        vector<string> fail(1);
        fail[0] = "FAIL TO FETCH USER\n";

        stringstream ss;
        ss << "SELECT * FROM Account;";
        if (mysql_query(db_conn, ss.str().c_str()))
        {
            return fail;
        }
        rset = mysql_use_result(db_conn);
        if (rset)
        {
            while ((rows = mysql_fetch_row(rset)))
            {
                result.push_back(rows[1]);
            }
        }
        mysql_free_result(rset);
        return result;
    }
    else
    {
        // initialize();
        fetchAllUser();
    }
}

int Database::insertAuthenRecord(string username, string type)
{
    if (type != "0" || type != "1")
    {
        return -2;
    }
    stringstream ss;
    ss << "INSERT INTO Authentication "
       << "VALUES ('" << username << "', '"
       << type << "',CURRENT_TIME());";

    if (mysql_query(db_conn, ss.str().c_str()))
        return 0;
    else
        return 1;
}

vector<Groupchat> Database::listRoom()
{
    vector<Groupchat> listroom;
    MYSQL_RES *rset;
    MYSQL_ROW rows;
    string sql = "SELECT * FROM group_chat";
    if (mysql_query(db_conn, sql.c_str()))
    {
        printf("Error printing all room!\n");
        return listroom;
    }
    rset = mysql_use_result(db_conn);

    if (rset)
    {
        while ((rows = mysql_fetch_row(rset)))
        {
            Groupchat *a = new Groupchat(atoi(rows[0]), string(rows[1]), bool(atoi(rows[2])), string(rows[3]));
            listroom.push_back(*a);
        }
    }
    mysql_free_result(rset);
    return listroom;
}

bool Database::addUserToGroupChat(vector<string> usernames, unsigned int room_id)
{
    for (auto username : usernames)
    {
        stringstream ss;
        ss << "INSERT INTO group_chat_account "
           << "VALUES (" << room_id << ", '"
           << username << "')";
        if (mysql_query(db_conn, ss.str().c_str()))
        {
            cout << "Can't add " << username << endl;
            continue;
        }
    }
    return 1;
}

bool Database::deleteUserFromGroupChat(string username, unsigned int room_id)
{
    stringstream ss;
    ss << "DELETE FROM group_chat_account WHERE GROUP_ID="
       << room_id << " AND ACC_USERNAME = '" << username << "'";

    if (mysql_query(db_conn, ss.str().c_str()))
        return 0;
    else
        return 1;
}

bool Database::deleteGroupChat(unsigned int group_id)
{
    stringstream ss;
    cout << "group_id: " << group_id << endl;
    ss << "DELETE FROM group_chat WHERE GROUP_ID="
       << group_id << "; ";
    if (mysql_query(db_conn, ss.str().c_str()))
        return 0;

    stringstream s;
    s << "DELETE FROM group_chat_account where GROUP_ID = " << group_id << ";";
    if (mysql_query(db_conn, s.str().c_str()))
        return 0;
}

Groupchat *Database::readGroupchat(unsigned int group_id)
{
    Groupchat *result = NULL;
    MYSQL_RES *rset;
    MYSQL_ROW rows;

    stringstream ss;
    ss << "SELECT * FROM group_chat WHERE GROUP_ID=" << group_id << ";";
    if (mysql_query(db_conn, ss.str().c_str()))
    {
        return NULL;
    }
    rset = mysql_use_result(db_conn);
    if (rset)
    {
        while ((rows = mysql_fetch_row(rset)))
        {
            Groupchat *a;
            if (!bool(atoi(rows[2])))
            {
                a = new Groupchat(string(rows[1]), bool(atoi(rows[2])), string(rows[3]), "");
            }
            else
            {
                a = new Groupchat(string(rows[1]), bool(atoi(rows[2])), string(rows[3]), string(rows[4]));
            }
            result = a;
        }
    }
    mysql_free_result(rset);
    return result;
}

bool Database::createGroupChat(string name, bool access, string owner, string password)
{
    if (access)
    {
        if (password == "")
        {
            printf("Password of private group chat can't be NULL");
            return 0;
        }
        stringstream ss;
        ss << "INSERT INTO group_chat "
           << "VALUES (0,'" << name << "', '"
           << 1 << "','" << owner << "','" << password << "',NULL)";
        if (mysql_query(db_conn, ss.str().c_str()))
            return 0;
    }
    else
    {
        stringstream ss;
        ss << "INSERT INTO group_chat "
           << "VALUES (0,'" << name << "', '"
           << 0 << "','" << owner << "',NULL,NULL)";
        if (mysql_query(db_conn, ss.str().c_str()))
            return 0;
    }
    string sql = "SELECT LAST_INSERT_ID()";
    if (mysql_query(db_conn, sql.c_str()))
    {
        return 0;
    }

    MYSQL_RES *res = mysql_use_result(db_conn);
    MYSQL_ROW rows;
    int room_id;
    while ((rows = mysql_fetch_row(res)))
    {
        room_id = atoi(rows[0]);
    }
    mysql_free_result(res);
    stringstream s;
    s << "UPDATE group_chat SET message_history = './group_chat_history/" << room_id << ".txt' WHERE GROUP_ID = " << room_id;
    if (mysql_query(db_conn, s.str().c_str()))
    {
        return 0;
    }
    stringstream str;
    str << "INSERT INTO group_chat_account VALUES (" << room_id << ",'" << owner << "');";
    if (mysql_query(db_conn, str.str().c_str()))
    {
        return 0;
    }
    return 1;
}

bool Database::updateGroupChatName(string name, unsigned int room_id)
{
    stringstream s;
    s << "UPDATE group_chat SET GROUP_NAME = '" << name << "' WHERE GROUP_ID = " << room_id;
    if (mysql_query(db_conn, s.str().c_str()))
    {
        return 0;
    }
    return 1;
}

bool Database::updateGroupChatPassword(string pwd, unsigned int group_id)
{
    stringstream s;
    s << "UPDATE group_chat SET PASSWORD = '" << pwd << "' WHERE ACCESSIBLE_MODIFIER = '1' AND GROUP_ID = " << group_id;
    if (mysql_query(db_conn, s.str().c_str()))
    {
        return 0;
    }
    return 1;
}

vector<int> Database::getUserGroupList(string username)
{
    vector<int> result;
    MYSQL_RES *rset;
    MYSQL_ROW rows;
    vector<int> fail(1);
    fail[0] = 0;

    stringstream ss;
    ss << "SELECT GROUP_ID FROM group_chat_account WHERE ACC_USERNAME='" << username << "';";
    if (mysql_query(db_conn, ss.str().c_str()))
    {
        return fail;
    }
    rset = mysql_use_result(db_conn);
    if (rset)
    {
        while ((rows = mysql_fetch_row(rset)))
        {
            result.push_back(atoi(rows[0]));
        }
    }
    mysql_free_result(rset);
    return result;
}

vector<string> Database::getGroupUser(unsigned int group_id)
{
    vector<string> result;
    MYSQL_RES *rset;
    MYSQL_ROW rows;
    vector<string> fail(1);
    fail[0] = "FAIL";

    stringstream ss;
    ss << "SELECT * FROM group_chat_account WHERE GROUP_ID=" << group_id << ";";
    if (mysql_query(db_conn, ss.str().c_str()))
    {
        return fail;
    }
    rset = mysql_use_result(db_conn);
    if (rset)
    {
        while ((rows = mysql_fetch_row(rset)))
        {
            result.push_back(rows[1]);
        }
    }

    mysql_free_result(rset);
    return result;
}

int Database::inheritFromOwner(int group_id, string username)
{
    stringstream ss;
    ss << "UPDATE group_chat SET owner='" << username << "' WHERE group_id='" << group_id << "';";
    if (mysql_query(db_conn, ss.str().c_str()))
        return 0;
    else
        return 1;
}

int Database::UserLoggedIn(string username, string session_key)
{
    stringstream ss;
    ss << "update Authentication set session = 1 where username='" << username << "' and session_key = '" << session_key << "';";
    if (mysql_query(db_conn, ss.str().c_str()))
        return 0;
    else
        return 1;
}

int Database::UserLoggedOut(string username)
{
    stringstream ss;
    stringstream s;
    ss << "update Authentication set session = 0 where username='" << username << "';";
    s << "update Authentication set session_key=NULL where username= '" << username << "';";
    if (mysql_query(db_conn, ss.str().c_str()))
        return 0;
    else
        return 1;
}

string Database::getUserSessionKey(string username)
{
    string result = "NULL";
    MYSQL_RES *rset;
    MYSQL_ROW rows;

    stringstream ss;
    ss << "SELECT * FROM Authentication WHERE username='" << username << "';";
    if (mysql_query(db_conn, ss.str().c_str()))
    {
        return result;
    }
    rset = mysql_use_result(db_conn);
    if (rset)
    {
        while ((rows = mysql_fetch_row(rset)))
        {
            result = rows[3];
        }
    }
    mysql_free_result(rset);
    return result;
}