#pragma once

#include <sys/ioctl.h>
#include <condition_variable>
#include <mutex>

//the following are UBUNTU/LINUX, and MacOS ONLY terminal color codes.
#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

#define ERROR_LOG(str) cout << BOLDRED << "[ERROR]: " << RESET << str
#define INFO_LOG(str) cout << BOLDBLUE << "[INFO]: " << RESET << str 
#define SUCCESS_LOG(str) cout << BOLDGREEN << "[SUCCESS]: " << RESET << str
#define WARN_LOG(str) cout << BOLDYELLOW << "[WARN]: " << RESET << str 

using namespace std;

namespace MyLibrary
{
    int getch();
    class UserInterface
    {
    public:
        static unsigned short ConsoleWidth;
        static unsigned short ConsoleHight;

        static winsize getConsoleSize();
        static void printHorizontalSeparatedLine();
        static void printDecov25LogoOnConsole();
        static void printContentCentered(string str, char alignChar);
        static void printMenuHeaderWithTitle(string username, string title);
        static void printChooseAgain(){}
        static int inputChoice(int from, int to);
        static string inputPassword();
    };

    class InterruptHandler
    {
    private:
        static condition_variable _condition;
        static mutex _mutex;

    public:
        static void hookSIGINT();
        static void handleUserInterrupt(int signal);
        static void waitForUserInterrupt();
    };
} // namespace MyLibrary
