#pragma once
#include <string>
#include <vector>
#include <bits/stdc++.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include "AccountsAPI.h"
#include "roomClass.h"

class adminInterface
{
private:
    char choose;

public:
    adminInterface();
    int mainMenu();
    int manageAccount();
    int manageGroup();
    //======================
    int viewAccountList();
    int createAccount();
    int changeAccountInfo();
    int removeAccount();

    int viewGroupList();
    int createGroup();
    int updateGroup();       // Change name, change pass, change owner, change room mode
    int modifyGroupMember(); // Add & remove members into/from
    int deleteGroup();

    int addGrMember(int groupid);
    int removeGrMember(int groupid);


private:
    static vector<Account> printAccountsList();
    static vector<roomClass> printGroupsList();
    static int printMainMenu();
    static int printManageAccountsMenu();
    static int printManageRoomsMenu();
    static int printModifyMemberMenu(roomClass group);
};