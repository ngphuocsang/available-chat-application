#include "menuClass.h"
#include "clientClass.h"
#include "Text_Color.h"
#include <string>
#include <pthread.h>
#include "tagClass.h"
#include <unistd.h>
#include <stdio.h>
#include "roomClass.h"
#include "RoomAPI.h"

#include "UserInterface.h"

using namespace std;
using namespace MyLibrary;
using namespace Text_Color;
using namespace Client::RestAPI;

tagClass tagmenu;

inline void displayMenu(int i);
inline void swapVectorElement(string &a, string &b);
inline void displayColorMenu(int i);
inline void displayManageRoomForm();
menuClass::menuClass()
{
    choose = 0;
}

int menuClass::loginForm()
{
    cin.clear();
    int sys = system("clear");
    UserInterface::printDecov25LogoOnConsole();
    cout << BOLDGREEN;
    UserInterface::printContentCentered("[Authentication]", ' ');
    cout << RESET;
    cout << BOLDWHITE << "\tUsername: " << RESET;
    cin >> clientClass::getInstance()->Username;
    cout << BOLDWHITE << "\tPassword: " << RESET;
    string password = UserInterface::inputPassword();
    UserInterface::printHorizontalSeparatedLine();
    AuthenAccountRequest req(clientClass::getInstance()->Username, password);
    AuthenAccountResponse res = AccountsAPI::postAuthenAccount(req);
    clientClass::getInstance()->AuthKey = res.AuthKey;
    clientClass::getInstance()->Role = res.Role;
    return res.Result;
}

int menuClass::mainMenu()
{
    char localChoose;
    while (1)
    {
        int i = 0;
        displayMenu(i);
        cin >> localChoose;
        //cout << "a" << endl;
        switch (localChoose)
        {
        case '1':
            this->activeMemMenu();
            break;
        case '2':
            this->groupChat();
            break;
        case '3':
            this->colorMenu();
            break;
        case '4':
            this->entireMember();
            break;
        case '5':
            break;
        case '0':
            break;
        default:
            i = 1;
            continue;
        }
        break;
    }
    return (int)(localChoose - '0');
}

int menuClass::activeMemMenu()
{
    int localChoose;
    int index = 0;
    int oldIDs = clientClass::getInstance()->GroupID;
    ActiveAccountsRequest req(clientClass::getInstance()->AuthKey);
    vector<string> users = AccountsAPI::getActiveAccounts(req).Usernames;
    int count = 5;
    for (auto i = users.begin(); i != users.end(); ++i)
        if (*i == clientClass::getInstance()->Username)
        {
            users.erase(i);
            break;
        }
    while (1)
    {
        UserInterface::printMenuHeaderWithTitle(
            clientClass::getInstance()->Username, "ACTIVE USERS");
        int i = 1;
        for (auto name : users)
        {
            cout << "\t" << i << ": " << name << endl;
            i++;
        }
        cout << "\t0: back" << endl;
        UserInterface::printHorizontalSeparatedLine();
        cout << "Choose(#): ";
        if (index == 1)
            UserInterface::printChooseAgain();

        cout << "\x1b[s";
        cin >> localChoose;
        while (cin.fail())
        {
            cin.clear();
            cin.ignore();
            cout << "\x1b[u\x1b[s\x1b[K \n";
            cout << "We do not have this choice. Please choose again!" << endl;
            cout << "\x1b[u";
            cin >> localChoose;
        }
        if (localChoose > users.size() || localChoose < 0)
        {
            index = 1;
            continue;
        }
        break;
    }
    if (localChoose == 0)
    {
        return 0;
    }
    string msg = tagmenu.reqsRoom + users.at(localChoose - 1);
    send(clientClass::getInstance()->Sock, (void *)msg.c_str(), 1024, 0);
    this->chatRoom(0);
    return 1;
}

int menuClass::groupChat()
{
    int i = 0;
    int localChoose;
    while (1)
    {
        UserInterface::printMenuHeaderWithTitle(
            clientClass::getInstance()->Username, "ROOMS LIST");
        cout << "\t1. List rooms" << endl;
        cout << "\t2. Create new room" << endl;
        cout << "\t3. Manage your room" << endl;
        cout << "\t0. Back" << endl;
        UserInterface::printHorizontalSeparatedLine();
        cout << "Choose(#): ";
        if (i == 1)
            UserInterface::printChooseAgain();
        cin >> localChoose;
        switch (localChoose)
        {
        case 1:
            this->showListRoom();
            break;
        case 2:
            this->createRoomForm();
            break;
        case 3:
            this->manageRoomForm();
            break;
        default:
            i = 1;
            break;
        }
        break;
    }
}

int menuClass::colorMenu()
{
    MyLibrary::getch();
    while (1)
    {
        int i;
        char localChoose;
        displayColorMenu(i);
        cin >> localChoose;
        //cout << "a" << endl;
        switch (localChoose)
        {
        case '1':
            clientClass::getInstance()->color = 1;
            break;
        case '2':
            clientClass::getInstance()->color = 2;
            break;
        case '3':
            clientClass::getInstance()->color = 3;
            break;
        case '0':
            break;
        default:
            i = 1;
            continue;
        }
        break;
    }
    return 1;
}

int menuClass::entireMember()
{
    int localChoose;
    int index = 0;
    int oldIDs = clientClass::getInstance()->GroupID;
    AccountsRequest req(clientClass::getInstance()->AuthKey);
    vector<Account> accounts = AccountsAPI::getAllAccounts(req).Accounts;
    int count = 5;
    for (auto i = accounts.begin(); i != accounts.end(); ++i)
        if (i->Username == clientClass::getInstance()->Username)
        {
            accounts.erase(i);
            break;
        }
    while (1)
    {
        UserInterface::printMenuHeaderWithTitle(
            clientClass::getInstance()->Username, "ALL USERS");
        int i = 1;
        for (auto item : accounts)
        {
            cout << "\t" << i << ": " << item.Username << endl;
            i++;
        }
        cout << BOLDRED << "\n0: back" << RESET << endl;
        UserInterface::printHorizontalSeparatedLine();
        cout << "Choose(#): ";
        if (index == 1)
            UserInterface::printChooseAgain();

        cout << "\x1b[s";
        cin >> localChoose;
        while (cin.fail())
        {
            cin.clear();
            cin.ignore();
            cout << "\x1b[u\x1b[s\x1b[K \n";
            cout << "We do not have this choice. Please choose again!" << endl;
            cout << "\x1b[u";
            cin >> localChoose;
        }
        if (localChoose > accounts.size() || localChoose < 0)
        {
            index = 1;
            continue;
        }
        break;
    }
    if (localChoose == 0)
    {
        return 0;
    }
    string msg = tagmenu.reqsRoom + accounts.at(localChoose - 1).Username;
    send(clientClass::getInstance()->Sock, (void *)msg.c_str(), 1024, 0);
    this->chatRoom(0);
    return 1;
}

int menuClass::chatRoom(int isChatGroup)
{
    // init socket
    if (clientClass::getInstance()->initialize() != RESULT_SUCCESS)
    {
        cout << "Initializing socket failed.\nGo back now." << endl;
        return CLIENT_SOCK_INIT_ERR;
    }
    if (clientClass::getInstance()->connectToServer() != RESULT_SUCCESS)
    {
        cout << "Go back now." << endl;
        return CLIENT_CONN_FAIL;
    }

    system("clear");
    sleep(1);
    clientClass::getInstance()->printChatHistory(clientClass::getInstance()->GroupID);
    if (isChatGroup == 0)
        clientClass::getInstance()->CtoC();
    else
        clientClass::getInstance()->CtoG();
    return 1;
}

int menuClass::showListRoom()
{
    int localChoose;
    int index = 0;
    vector<roomClass> rooms = roomClass::getListRoom();
    while (1)
    {
        UserInterface::printMenuHeaderWithTitle(
            clientClass::getInstance()->Username, "ROOMS LIST");
        unsigned short width = UserInterface::getConsoleSize().ws_col - 8;
        cout << left << BOLDBLUE << setw(8) << "ID";
        cout << setw(3 * width / 5) << "Room's name";
        cout << setw(width / 5) << "Owner";
        cout << setw(width / 5) << "IsPrivate" << RESET << endl;
        for (auto item : rooms)
        {
            cout << left << setw(8) << BOLDYELLOW << item.IDs << RESET;
            cout << setw(3 * width / 5) << item.Name;
            cout << setw(width / 5) << item.Owner;
            cout << setw(width / 5) << item.TypeOfRoom << endl;
        }
        cout << BOLDYELLOW << "\n0: back" << RESET << endl;
        UserInterface::printHorizontalSeparatedLine();
        cout << BOLDYELLOW << "Choose(#): " << RESET;
        if (index == 1)
            UserInterface::printChooseAgain();

        cout << "\x1b[s";
        cin >> localChoose;
        while (cin.fail())
        {
            cin.clear();
            cin.ignore();
            cout << "\x1b[u\x1b[s\x1b[K \n";
            cout << "We do not have this choice. Please choose again!" << endl;
            cout << "\x1b[u";
            cin >> localChoose;
        }
        int size = rooms.size();
        if (localChoose > rooms.at(size - 1).IDs || localChoose < 0)
        {
            index = 1;
            continue;
        }
        if (localChoose == 0)
        {
            return 0;
        }
        for (int index = 0; index < rooms.size(); index++)
        {
            if (rooms.at(index).IDs == localChoose)
            {
                string password;
                if (rooms.at(index).TypeOfRoom == 1)
                {
                    cout << "Password";
                    cin.ignore();
                    cin.clear();
                    getline(cin, password);
                    sleep(1);
                }
                JoinRoomRequest req(localChoose, password, clientClass::getInstance()->Username, clientClass::getInstance()->AuthKey);
                JoinRoomResponse res = RoomAPI::joinRoom(req);
                cout << res.Status << endl;
                clientClass::getInstance()->GroupID = rooms.at(index).IDs;
                break;
            }
            //continue;
        }

        this->chatRoom(1);
        break;
    }

    //this->c();
    return 1;
}

int menuClass::createRoomForm()
{
    int ID = 1;
    string groupName;
    int grMode = 0;
    string grPw;
    string stMode;
    MyLibrary::RESULT_CODE result = RESULT_UNKNOWN;
    UserInterface::printMenuHeaderWithTitle(
        clientClass::getInstance()->Username, "CREATE YOUR NEW ROOM");
    cout << BOLDWHITE << "\tEnter group name: " << RESET;
    getline(cin, groupName);
    cin.ignore();
    cout << "\tChoose your group mode: ";
    cin >> grMode;
    if (grMode == 0)
    {
        grPw = "";
        stMode = "Public";
    }
    else
    {
        cout << "   Enter your group password: ";
        cin >> grPw;
        stMode = "Private";
    }

    //  Send creating group request to server here
    CreateRoomRequest req(groupName, grMode, clientClass::getInstance()->Username, grPw, clientClass::getInstance()->AuthKey);
    CreateRoomResponse res = RoomAPI::createRoom(req);
    // If Okay
    if (res.Status == "Success")
    {
        UserInterface::printHorizontalSeparatedLine();
        SUCCESS_LOG("Your room is created successfully!");
        cout << "\n\tGrID:       " << ID << endl;
        cout << "\tGroup name: " << groupName << endl;
        cout << "\tMode:       " << stMode << endl;
        cout << "\tPassword:   " << grPw << endl;
        UserInterface::printHorizontalSeparatedLine();
    }
    else
    {
        UserInterface::printHorizontalSeparatedLine();
        ERROR_LOG("Room creating failed. Please try again later!");
        cout << endl;
    }

    return result;
}

int menuClass::manageRoomForm()
{
    displayManageRoomForm();
    char localChoose;
    while (1)
    {
        int i = 0;
        displayMenu(i);
        cin >> localChoose;
        //cout << "a" << endl;
        switch (localChoose)
        {
        case '1':
            //this->changeRoomName();
            break;
        case '2':
            //this->changeRoomMode();
            break;
        case '3':
            //this->addMember();
            break;
        case '4':
            //this->removeMember();
            break;
        case '0':
            break;
        default:
            i = 1;
            continue;
        }
        break;
    }
    return (int)(localChoose - '0');
}

void swapVectorElement(string &a, string &b)
{
    string t = a;
    a = b;
    b = t;
}

void displayColorMenu(int i)
{
    UserInterface::printMenuHeaderWithTitle(
        clientClass::getInstance()->Username, "CHOOSE COLOR");
    cout << color_text::color_Red_Text("\t1:\tRed") << endl;
    cout << color_text::color_Blue_Text("\t2:\tBlue") << endl;
    cout << color_text::color_Green_Text("\t3:\tGreen") << endl;
    cout << BOLDYELLOW << "\t0:\t" << RESET << "Back" << endl;
    UserInterface::printHorizontalSeparatedLine();
    cout << BOLDYELLOW << "Choose(#): " << RESET;
    if (i == 1)
        UserInterface::printChooseAgain();
}

void displayManageRoomForm()
{
    UserInterface::printMenuHeaderWithTitle(
        clientClass::getInstance()->Username, "MANAGE YOUR ROOM");
    cout << BOLDYELLOW << "\t1:\t" << RESET << "Change room name" << endl;
    cout << BOLDYELLOW << "\t2:\t" << RESET << "Change room password" << endl;
    cout << BOLDYELLOW << "\t3:\t" << RESET << "Add member" << endl;
    cout << BOLDYELLOW << "\t4:\t" << RESET << "Remove member" << endl;
    cout << BOLDYELLOW << "\t0:\t" << RESET << "Go back" << endl;
    UserInterface::printHorizontalSeparatedLine();
    cout << BOLDYELLOW << "Choose(#): " << RESET;
}

inline void displayMenu(int i)
{
    UserInterface::printMenuHeaderWithTitle(
        clientClass::getInstance()->Username, "MAIN MENU");
    cout << BOLDYELLOW << "\t1:\t" << RESET << "Active users" << endl;
    cout << BOLDYELLOW << "\t2:\t" << RESET << "Group chat" << endl;
    cout << BOLDYELLOW << "\t3:\t" << RESET << "Choose text color" << endl;
    cout << BOLDYELLOW << "\t4:\t" << RESET << "All users" << endl;
    cout << BOLDYELLOW << "\t5:\t" << RESET << "Logout" << endl;
    cout << BOLDYELLOW << "\t0:\t" << RESET << "Exit" << endl;
    UserInterface::printHorizontalSeparatedLine();
    cout << BOLDYELLOW << "Choose(#): " << RESET;
    if (i == 1)
        UserInterface::printChooseAgain();
}