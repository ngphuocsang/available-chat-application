#include <iostream>
#include <string>
#include <termios.h>
#include <unistd.h>
#include <iomanip>
#include "adminInterface.h"
#include "clientClass.h"
#include "Text_Color.h"
#include "text_align.h"
#include "roomClass.h"
#include <iomanip>
#include "UserInterface.h"

using namespace std;
using namespace Client::RestAPI;
using namespace text_align;
using namespace MyLibrary;

adminInterface::adminInterface()
{
    choose = '0';
}

vector<Account> adminInterface::printAccountsList()
{
    AccountsRequest req(clientClass::getInstance()->AuthKey);
    vector<Account> accounts = AccountsAPI::getAllAccounts(req).Accounts;
    UserInterface::printContentCentered(" [ALL ACCOUNTS LIST] ", '-');
    cout << '\t' << left << BOLDBLUE << setw(8) << "No";
    cout << setw(32) << "Username";
    cout << setw(8) << "Role" << RESET << endl;
    int i = 1;
    for (auto item : accounts)
    {
        cout << '\t' << left << setw(8) << i++ << RESET;
        cout << setw(32) << item.Username;
        auto temp = item.Role == "0" ? "admin" : "user";
        cout << setw(8) << temp << endl;
    }
    return accounts;
}

vector<roomClass> adminInterface::printGroupsList()
{
    vector<roomClass> rooms = roomClass::getListRoom();
    UserInterface::printContentCentered(" [ALL GROUPS LIST] ", '-');
    unsigned short width = UserInterface::getConsoleSize().ws_col - 24;
    cout << '\t' << left << BOLDBLUE << setw(8) << "No";
    cout << left << BOLDBLUE << setw(8) << "ID";
    cout << setw(3 * width / 5) << "Room's name";
    cout << setw(width / 5) << "Owner";
    cout << setw(width / 5) << "IsPrivate" << RESET << endl;
    int i = 1;
    for (auto item : rooms)
    {
        cout << '\t' << left << setw(8) << i++ << RESET;
        cout << left << setw(8) << item.IDs << RESET;
        cout << setw(3 * width / 5) << item.Name;
        cout << setw(width / 5) << item.Owner;
        auto temp = item.TypeOfRoom == 0 ? "public" : "private";
        cout << setw(width / 5) << temp << endl;
    }
    return rooms;
}
int adminInterface::mainMenu()
{
    int choose = printMainMenu();
    switch (choose)
    {
    case 1:
        this->manageAccount();
        break;
    case 2:
        this->manageGroup();
        break;
    }
    return choose;
}

int adminInterface::manageAccount()
{
    while (1)
    {
        int choose = printManageAccountsMenu();
        switch (choose)
        {
        case 1:
            this->viewAccountList();
            break;
        case 2:
            this->createAccount();
            break;
        case 3:
            this->changeAccountInfo();
            break;
        case 4:
            this->removeAccount();
            break;
        case 0:
            return choose;
        }
    }
}

int adminInterface::manageGroup()
{
    while (1)
    {

        int choose = printManageRoomsMenu();
        switch (choose)
        {
        case 1:
            this->viewGroupList();
            break;
        case 2:
            this->createGroup();
            break;
        case 3:
            this->updateGroup();
            break;
        case 4:
            this->deleteGroup();
            break;
        case 5:
            this->modifyGroupMember();
            break;
        case 0:
            return choose;
        }
    }
}

int adminInterface::modifyGroupMember()
{
    while (1)
    {
        UserInterface::printMenuHeaderWithTitle(
            clientClass::getInstance()->Username, "MODIFY GROUP'S MEMBERS");
        auto groups = printGroupsList();
        cout << BOLDYELLOW << "\n0:\tBack" << RESET << endl;
        UserInterface::printHorizontalSeparatedLine();
        int choice = UserInterface::inputChoice(0, groups.size());
        if (choice == 0)
            break;
        int choose = printModifyMemberMenu(groups[choice - 1]);
        switch (choose)
        {
        case 1:
            this->addGrMember(groups[choice - 1].IDs);
            break;
        case 2:
            this->removeGrMember(groups[choice - 1].IDs);
            break;
        }
    }
    return 0;
}

int adminInterface::viewAccountList()
{
    AccountsRequest req(clientClass::getInstance()->AuthKey);
    vector<Account> accounts = AccountsAPI::getAllAccounts(req).Accounts;
    UserInterface::printMenuHeaderWithTitle(
        clientClass::getInstance()->Username, "ACCOUNTS LIST");
    printAccountsList();
    cout << BOLDYELLOW << "\n0:\tBack" << RESET << endl;
    UserInterface::printHorizontalSeparatedLine();
    return UserInterface::inputChoice(0, 3);
}

int adminInterface::createAccount()
{
    RESULT_CODE result;
    UserInterface::printMenuHeaderWithTitle(
        clientClass::getInstance()->Username, "CREATE NEW ACCOUNT");
    CreateAccountRequest req(clientClass::getInstance()->AuthKey);
    cout << BOLDWHITE << "\tUsername: " << RESET;
    cin >> req.Username;
    if (req.Username == "/exit")
        return 0;
    cout << BOLDWHITE << "\tPassword: " << RESET;
    req.Password = UserInterface::inputPassword();
    cout << BOLDWHITE << "\tRole    : " << RESET;
    cin >> req.Role;
    if (req.Role != "0" || req.Role != "1")
        req.Role = "1";
    UserInterface::printHorizontalSeparatedLine();
    result = AccountsAPI::postCreateAccount(req);
    if (result == RESULT_SUCCESS)
        cout << "Creating account succeeded. Go back now" << endl;
    else
        cout << "Creating new account failed. Try again later" << endl;
    sleep(1);
    return result;
}

int adminInterface::changeAccountInfo()
{
    RESULT_CODE result;
    UserInterface::printMenuHeaderWithTitle(
        clientClass::getInstance()->Username, "UPDATE EXIST ACCOUNT");
    auto accounts = printAccountsList();
    UpdateAccountRequest req(clientClass::getInstance()->AuthKey);
    cout << BOLDYELLOW << "Leave it empty if you don't want to change\n"
         << RESET;
    cout << BOLDWHITE << "\tUsername(/exit to back): " << RESET;
    cin >> req.Username;
    if (req.Username == "/exit")
        return 0;
    cout << BOLDWHITE << "\tPassword(9 digit chars): " << RESET;
    req.Password = UserInterface::inputPassword();
    cout << BOLDWHITE << "\tRole(1: user, 0: admin): " << RESET;
    cin >> req.Role;
    if (req.Role != "0" && req.Role != "1")
        req.Role = "";
    UserInterface::printHorizontalSeparatedLine();
    result = AccountsAPI::postUpdateAccount(req);
    if (result == RESULT_SUCCESS)
        cout << "Update account successfully. Go back now" << endl;
    else if (result == USERNAME_NOT_FOUND)
        cout << "Username " << req.Username << " not found. Go back now" << endl;
    else
        cout << "Update exist account failed. Try again later" << endl;
    sleep(1);
    return result;
}

int adminInterface::removeAccount()
{
    RESULT_CODE result = RESULT_SUCCESS;
    UserInterface::printMenuHeaderWithTitle(
        clientClass::getInstance()->Username, "DELETE EXIST ACCOUNT");
    auto accounts = printAccountsList();
    DeleteAccountRequest req(clientClass::getInstance()->AuthKey);
    cout << BOLDYELLOW << "\n0:\tBack" << RESET << endl;
    UserInterface::printHorizontalSeparatedLine();
    int choice = UserInterface::inputChoice(0, accounts.size());
    if (choice != 0)
    {
        req.Username = accounts[choice - 1].Username;
        result = AccountsAPI::postDeleteAccount(req);
        if (result == RESULT_SUCCESS)
            cout << "Delete exist account successfully. Go back now" << endl;
        else if (result == USERNAME_NOT_FOUND)
            cout << "Username " << req.Username << " not found. Go back now" << endl;
        else
            cout << "Delete exist account failed. Try again later" << endl;
        sleep(1);
    }
    return result;
}

int adminInterface::viewGroupList()
{
    UserInterface::printMenuHeaderWithTitle(
        clientClass::getInstance()->Username, "GROUPS LIST");
    vector<roomClass> rooms = roomClass::getListRoom();
    printGroupsList();
    cout << BOLDYELLOW << "\n0: back" << RESET << endl;
    UserInterface::printHorizontalSeparatedLine();
    return UserInterface::inputChoice(0, 3);
}

int adminInterface::createGroup()
{
    UserInterface::printMenuHeaderWithTitle(
        clientClass::getInstance()->Username, "CREATE NEW GROUP");
    auto accounts = printAccountsList();
    UserInterface::printHorizontalSeparatedLine();
    CreateRoomRequest req("", 0, "", "", clientClass::getInstance()->AuthKey);
    cout << BOLDWHITE << "\tGroup's name: " << RESET;
    cin.clear();
    cin.ignore();
    getline(cin, req.Roomname);
    if (req.Roomname == "/exit")
        return 0;
    cout << BOLDWHITE << "\tOwner's name: " << RESET;
    cin >> req.Owner;
    cout << BOLDWHITE << "\tIsPrivate: " << RESET;
    cin >> req.Access;
    if (req.Access != 0 || req.Access != 1)
        req.Access = 0;
    if (req.Access == 1)
    {
        cout << BOLDWHITE << "\tPassword: " << RESET;
        req.Password = UserInterface::inputPassword();
    }
    CreateRoomResponse res = RoomAPI::createRoom(req);
    UserInterface::printHorizontalSeparatedLine();
    if (res.Result == RESULT_SUCCESS)
        cout << "Create new room chat successfully. Go back now!" << endl;
    else if (res.Result == GROUP_OWNER_OR_PASSWORD_INVALID)
        cout << "Group owner or password invalid. Try again later." << endl;
    else if (res.Result == GROUP_NAME_EMPTY)
        cout << "Please enter your group name at the next try!" << endl;
    else
        cout << "Create new group failed. Try again later" << endl;
    sleep(1);
    return res.Result;
}

int adminInterface::updateGroup()
{
    UserInterface::printMenuHeaderWithTitle(
        clientClass::getInstance()->Username, "UPDATE EXIST GROUP");
    auto groups = printGroupsList();
    UserInterface::printHorizontalSeparatedLine();
    ModifyRoomRequest req(0, clientClass::getInstance()->Username, "", "", "" , clientClass::getInstance()->AuthKey);
    cout << BOLDWHITE << "\tGroup's ID (0: Back): " << RESET;
    cin >> req.GroupID;
    if (req.GroupID == 0)
        return 0;
    cout << BOLDWHITE << "\tGroup's new name: " << RESET;
    cin.clear();
    cin.ignore();
    getline(cin, req.Newname);
    cout << BOLDWHITE << "\tGroup's old passwd: " << RESET;
    req.Password = UserInterface::inputPassword();
    cout << BOLDWHITE << "\tGroup's new passwd: " << RESET;
    req.Newpass = UserInterface::inputPassword();
    UserInterface::printHorizontalSeparatedLine();
    ModifyRoomResponse res = RoomAPI::modifyRoom(req);

    if (res.Result == RESULT_SUCCESS)
        cout << "Update group info successfully. Go back now!" << endl;
    else if (res.Result == GROUP_OWNER_OR_PASSWORD_INVALID)
        cout << "Group owner or password invalid. Try again later." << endl;
    else if (res.Result == GROUP_NAME_EMPTY)
        cout << "Please enter your group name at the next try!" << endl;
    else
        cout << "Update exist group failed. Try again later" << endl;
    sleep(1);
    return res.Result;
}

int adminInterface::deleteGroup()
{
    UserInterface::printMenuHeaderWithTitle(
        clientClass::getInstance()->Username, "DELETE EXIST GROUP");
    auto groups = printGroupsList();
    DeleteRoomRequest req(0, clientClass::getInstance()->Username, "", clientClass::getInstance()->AuthKey);
    cout << BOLDYELLOW << "\n0:\tBack" << RESET << endl;
    UserInterface::printHorizontalSeparatedLine();
    int choice = UserInterface::inputChoice(0, groups.size());
    if (choice != 0)
    {
        req.GroupID = groups[choice - 1].IDs;
        DeleteRoomResponse res = RoomAPI::deleteRoom(req);
        if (res.Result == RESULT_SUCCESS)
            cout << "Delete exist group successfully. Go back now!" << endl;
        else if (res.Result == GROUP_INEXIST)
            cout << "Group ID does not exist. Try again later!" << endl;
        else
            cout << "Delete exist group failed. Try again later" << endl;
        sleep(1);
        return res.Result;
    }
    return 0;
}

int adminInterface::addGrMember(int groupid)
{
    UserInterface::printMenuHeaderWithTitle(
        clientClass::getInstance()->Username, "ADD NEW MEMBER TO GROUP");
    RoomMemberResponse members = RoomAPI::getRoomMember(RoomMemberRequest(groupid, clientClass::getInstance()->AuthKey));
    UserInterface::printContentCentered(" [ALL MEMBERS LIST] ", '-');
    cout << '\t' << left << BOLDBLUE << setw(8) << "No";
    cout << setw(32) << "Username" << RESET << endl;
    int i = 1;
    for (auto item : members.Username)
    {
        cout << '\t' << left << setw(8) << i++ << RESET;
        cout << setw(32) << item << endl;
    }
    cout << '\n';
    auto accounts = printAccountsList();
    UserInterface::printHorizontalSeparatedLine();
    vector<string> listUser;
    InviteRoomRequest req(groupid, listUser, clientClass::getInstance()->AuthKey);
    string member;
    vector<string> listMember;
    cout << BOLDWHITE << "\tList users: " << RESET;
    cin.clear();
    cin.ignore();
    getline(cin, member);
    if (member == "/exit")
        return 0;
    listMember = align::divide_message_into_word(member);
    int idx;
    for (int i = 0; i < listMember.size(); i++)
    {
        stringstream ssMember(listMember.at(i));
        ssMember >> idx;
        listUser.push_back(accounts.at(idx - 1).Username);
    }
    req.Username = listUser;
    InviteRoomResponse res = RoomAPI::inviteRoom(req);
    if (res.Result == RESULT_SUCCESS)
        cout << "Add member into group successfully. Go back now!" << endl;
    else
        cout << "Add member into group failed. Try again later" << endl;
    sleep(1);
    return res.Result;
}

int adminInterface::removeGrMember(int groupid)
{
    UserInterface::printMenuHeaderWithTitle(
        clientClass::getInstance()->Username, "REMOVE MEMBER IN GROUP");
    RoomMemberResponse members = RoomAPI::getRoomMember(RoomMemberRequest(groupid, clientClass::getInstance()->AuthKey));
    UserInterface::printContentCentered(" [ALL MEMBERS LIST] ", '-');
    cout << '\t' << left << BOLDBLUE << setw(8) << "No";
    cout << setw(32) << "Username" << RESET << endl;
    int i = 1;
    for (auto item : members.Username)
    {
        cout << '\t' << left << setw(8) << i++ << RESET;
        cout << setw(32) << item << endl;
    }
    cout << BOLDYELLOW << "\n0:\tBack" << RESET << endl;
    UserInterface::printHorizontalSeparatedLine();
    int choice = UserInterface::inputChoice(0, members.Username.size());
    if (choice == 0)
        return 0;
    string userRemoved = members.Username[choice - 1];
    UserInterface::printHorizontalSeparatedLine();
    LeaveRoomResponse res = RoomAPI::leaveRoom(
        LeaveRoomRequest(groupid, userRemoved, clientClass::getInstance()->AuthKey));
    if (res.Result == RESULT_SUCCESS)
        cout << "Remove member in group successfully. Go back now!" << endl;
    else
        cout << "Remove member in group failed. Try again later" << endl;
    sleep(1);
    return res.Result;
}

int adminInterface::printMainMenu()
{
    UserInterface::printMenuHeaderWithTitle(
        clientClass::getInstance()->Username, "MAIN MENU");
    cout << BOLDYELLOW << "\t1:\t" << RESET << "Manage accounts" << endl;
    cout << BOLDYELLOW << "\t2:\t" << RESET << "Manage groups" << endl;
    cout << BOLDYELLOW << "\t3:\t" << RESET << "Logout" << endl;
    cout << BOLDYELLOW << "\t0:\t" << RESET << "Exit" << endl;
    UserInterface::printHorizontalSeparatedLine();
    return UserInterface::inputChoice(0, 3);
}

int adminInterface::printManageAccountsMenu()
{
    int sys = system("clear");
    cout << BOLDGREEN;
    UserInterface::printContentCentered(
        "Authenticated as " + clientClass::getInstance()->Username, ' ');
    cout << RESET << endl;
    UserInterface::printContentCentered(" [MANAGE ACCOUNTS] ", '=');
    cout << BOLDYELLOW << "\t1:\t" << RESET << "View accounts list" << endl;
    cout << BOLDYELLOW << "\t2:\t" << RESET << "Create new account" << endl;
    cout << BOLDYELLOW << "\t3:\t" << RESET << "Update exist account" << endl;
    cout << BOLDYELLOW << "\t4:\t" << RESET << "Remove exist account" << endl;
    cout << BOLDYELLOW << "\t0:\t" << RESET << "Back" << endl;
    UserInterface::printHorizontalSeparatedLine();
    return UserInterface::inputChoice(0, 4);
}

int adminInterface::printManageRoomsMenu()
{
    UserInterface::printMenuHeaderWithTitle(
        clientClass::getInstance()->Username, "MANAGE GROUPS");
    cout << BOLDYELLOW << "\t1:\t" << RESET << "View groups list" << endl;
    cout << BOLDYELLOW << "\t2:\t" << RESET << "Create new group" << endl;
    cout << BOLDYELLOW << "\t3:\t" << RESET << "Update exist group" << endl;
    cout << BOLDYELLOW << "\t4:\t" << RESET << "Delete exist group" << endl;
    cout << BOLDYELLOW << "\t5:\t" << RESET << "Modify group's members" << endl;
    cout << BOLDYELLOW << "\t0:\t" << RESET << "Back" << endl;
    UserInterface::printHorizontalSeparatedLine();
    return UserInterface::inputChoice(0, 5);
}

int adminInterface::printModifyMemberMenu(roomClass group)
{
    UserInterface::printMenuHeaderWithTitle(
        clientClass::getInstance()->Username, "MODIFY GROUP'S MEMBERS");
    UserInterface::printContentCentered(to_string(group.IDs) + " " + group.Name, '-');
    cout << BOLDYELLOW << "\t1:\t" << RESET << "Add new member" << endl;
    cout << BOLDYELLOW << "\t2:\t" << RESET << "Remove member" << endl;
    cout << BOLDYELLOW << "\t0:\t" << RESET << "Back" << endl;
    UserInterface::printHorizontalSeparatedLine();
    return UserInterface::inputChoice(0, 2);
}