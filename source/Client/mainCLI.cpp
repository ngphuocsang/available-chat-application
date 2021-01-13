#include "MyLibrary.h"
#include "AccountsAPI.h"
#include "clientClass.h"
#include "adminInterface.h"
#include "UserInterface.h"

#include <iostream>

using namespace std;
using namespace MyLibrary;
using namespace Client::RestAPI;
using namespace MyLibrary::Models::Requests;
using namespace MyLibrary::Models::Responses;

tagClass tagmain;

int comeToLogin(clientClass *&clientUser, menuClass &menu);

int main(int argc, char *argv[])
{
    int result = RESULT_CODE::RESULT_UNKNOWN;
    if ((result = AppSetting::loadSettingClientFromFile()))
        return result;
    clientClass *clientUser = clientClass::getInstance();
    menuClass menu;
    adminInterface adminMenu;
    comeToLogin(clientUser, menu);

    int mainMenuReturn = -1;
    while (1)
    {
        if (clientUser->Role == "1")
        {
            mainMenuReturn = menu.mainMenu();
            if (mainMenuReturn == 5)
            {
                send(clientUser->Sock, (void *)tagmain.logout.c_str(), 5, 0);
                cout << "Logging out..." << endl;
                sleep(1);
                clientClass::getInstance()->resetClient();
                comeToLogin(clientUser, menu);
                continue;
            }
            if (mainMenuReturn == 0)
            {
                send(clientUser->Sock, (void *)tagmain.logout.c_str(), 5, 0);
                cout << "Goodbye" << endl;
                break;
            }
        }
        else if (clientUser->Role == "0")
        {
            mainMenuReturn = adminMenu.mainMenu();
            if (mainMenuReturn == 3)
            {
                //send(clientUser->Sock, (void *)tagmain.logout.c_str(), 5, 0);
                cout << "ADMIN is logging out..." << endl;
                sleep(1);
                clientClass::getInstance()->resetClient();
                comeToLogin(clientUser, menu);
                continue;
            }
            if (mainMenuReturn == 0)
            {
                //send(clientUser->Sock, (void *)tagmain.logout.c_str(), 5, 0);
                cout << "Goodbye ADMIN" << endl;
                break;
            }
        }
        else
        {
            break;
        }
    }
    return RESULT_CODE::RESULT_SUCCESS;
}

int comeToLogin(clientClass *&clientUser, menuClass &menu)
{
    int result = RESULT_CODE::RESULT_UNKNOWN;

    for (int i = 0; i < 5; i++)
    {
        result = (RESULT_CODE)menu.loginForm();
        string username = clientClass::getInstance()->Username;
        switch (result)
        {
        case RESULT_CODE::RESULT_UNKNOWN:
            exit(0);
        case RESULT_CODE::RESULT_SUCCESS:
            SUCCESS_LOG("Authentication successfull as ");
            cout << BOLDGREEN << username << endl << RESET;
            sleep(1);
            return result;
            break;
         case RESULT_CODE::ACCOUNT_AUTHENTICATED:
            WARN_LOG("Authentication unsuccessfull: ");
            cout << BOLDYELLOW << "Username " << username << " have been logged in!\n" << RESET;
            break;
        case RESULT_CODE::USERNAME_NOT_FOUND:
            WARN_LOG("Authentication unsuccessfull: ");
            cout << BOLDYELLOW << "Username " << username << " not found!\n" << RESET;
            break;
        case RESULT_CODE::PASSWORD_INVALID:
            WARN_LOG("Authentication unsuccessfull: ");
            cout << BOLDYELLOW << "Incorrect password!" << endl << RESET;
            break;
        default:
            break;
        }
        sleep(1);
    }

    return result;
}
