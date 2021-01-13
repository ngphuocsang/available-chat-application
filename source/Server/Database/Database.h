#ifndef _DATABASE_CLASS_H
#define _DATABASE_CLASS_H
#include <iostream>
#include <stdlib.h>
#include <mysql/mysql.h>
#include <string>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <iomanip>
#include "Account.h"
#include "Groupchat.h"

using namespace std;

using namespace MyLibrary::Models;

class Database
{
public:
   // functions
   bool createAccount(Account *account);
   bool deleteAccount(string username);
   vector<Account> readAllAccounts();
   bool updateRole(string username, string role);
   bool updatePassword(string username, string password);
   vector<string> getRoomUser(int room_id);
   int createChatRoom(vector<string> username);
   Account *readAccount(string username);
   bool addMessage(string msg, int room_id);
   int readRecentMessage(int room_id);
   string readMessage(int room_id);
   // int readMessage(int room_id);
   vector<string> fetchAllUser();
   int insertAuthenRecord(string username, string type);
   vector<Groupchat> listRoom();
   bool addUserToGroupChat(vector<string> usernames, unsigned int room_id);
   bool deleteUserFromGroupChat(string username, unsigned int room_id);
   bool deleteGroupChat(unsigned int group_id);

   Groupchat *readGroupchat(unsigned int group_id);

   bool updateGroupChatName(string name, unsigned int room_id);
   bool updateGroupChatPassword(string pwd, unsigned int group_id);
   bool createGroupChat(string name, bool access, string owner, string password);
   vector<int> getUserGroupList(string username);
   vector<string> getGroupUser(unsigned int group_id);
   int inheritFromOwner(int group_id, string username);
   int UserLoggedIn(string username, string session_key);
   int UserLoggedOut(string username);
   string getUserSessionKey(string username);

   //
   static Database *getInstance();
   int initialize(string host, string user, string pass, string dbname);
   void initialize();
   int shutdown();

private:
   static Database *_instance;
   Database();
   MYSQL *db_conn;
};
#endif
