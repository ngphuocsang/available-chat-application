#include "MyLibrary.h"
#include "AccountsAPI.h"
#include "clientClass.h"
#include "adminInterface.h"
#include "mainwindow.h"
#include <QApplication>

#include <iostream>

using namespace std;
using namespace MyLibrary;
using namespace Client::RestAPI;
using namespace MyLibrary::Models::Requests;
using namespace MyLibrary::Models::Responses;

tagClass tagmain;

//int comeToLogin(clientClass *&clientUser, menuClass &menu);

int main(int argc, char *argv[])
{
    int result = RESULT_CODE::RESULT_UNKNOWN;
    if ((result = AppSetting::loadSettingClientFromFile()))
        return result;
    clientClass *clientUser = clientClass::getInstance();
    //menuClass menu;
    adminInterface adminMenu;
    
    QApplication a(argc, argv);
    MainWindow w;
    chatbox *b = chatbox::getInstance();
    while(1){       
        w.show();
        a.exec();
        //exit
        if(clientClass::getInstance()->Role == "-1"){
            clientClass::getInstance()->Logout();
            return 0;
        }  
        b->show();
        b->init();
        a.exec();
        //logout
        if(clientClass::getInstance()->Role == "-1"){
            continue;
            clientClass::getInstance()->Logout();
        }
        clientClass::getInstance()->Logout();   
        break;
    }
    // Authenticate
    //comeToLogin(clientUser, menu);

    // int mainMenuReturn = -1;
    // while (1)
    // {
    //     if (clientUser->Role == "1")
    //     {
    //         mainMenuReturn = menu.mainMenu();
    //         if (mainMenuReturn == 5)
    //         {
    //             send(clientUser->Sock, (void *)tagmain.logout.c_str(), 5, 0);
    //             cout << "Logging out..." << endl;
    //             sleep(1);
    //             clientClass::getInstance()->resetClient();
    //             comeToLogin(clientUser, menu);
    //             continue;
    //         }
    //         if (mainMenuReturn == 0)
    //         {
    //             send(clientUser->Sock, (void *)tagmain.logout.c_str(), 5, 0);
    //             cout << "Goodbye" << endl;
    //             break;
    //         }
    //     }
    //     else if (clientUser->Role == "0")
    //     {
    //         mainMenuReturn = adminMenu.mainMenu();
    //         if (mainMenuReturn == 3)
    //         {
    //             send(clientUser->Sock, (void *)tagmain.logout.c_str(), 5, 0);
    //             cout << "ADMIN is logging out..." << endl;
    //             sleep(1);
    //             clientClass::getInstance()->resetClient();
    //             comeToLogin(clientUser, menu);
    //             continue;
    //         }
    //         if (mainMenuReturn == 0)
    //         {
    //             send(clientUser->Sock, (void *)tagmain.logout.c_str(), 5, 0);
    //             cout << "Goodbye ADMIN" << endl;
    //             break;
    //         }
    //     }
    //     else
    //     {
    //         break;
    //     }
    // }
    return 0;
}
