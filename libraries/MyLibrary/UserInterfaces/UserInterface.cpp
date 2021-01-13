#include "UserInterface.h"
#include "MyLibrary.h"

#include <iostream>
#include <unistd.h>
#include <limits.h>
#include <signal.h>
#include <libgen.h>
#include <termios.h>

#define SMALL_LOGO_WIDTH 120
#define LARGE_LOGO_WIDTH 180
#define LOGO_FILE_TEXT "/logo.txt"

using namespace std;
using namespace MyLibrary;

namespace MyLibrary
{
    // UserInterface
    unsigned short UserInterface::ConsoleHight = 0;
    unsigned short UserInterface::ConsoleWidth = 0;

    winsize UserInterface::getConsoleSize()
    {
        struct winsize _winsize;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &_winsize);
        ConsoleWidth = _winsize.ws_col > 128 ? 128 : _winsize.ws_col;
        ConsoleHight = _winsize.ws_row;
        return _winsize;
    }

    void UserInterface::printHorizontalSeparatedLine()
    {
        getConsoleSize();
        for (unsigned short i = 0; i < ConsoleWidth; ++i)
            cout << '=';
        cout << '\n';
    }

    void UserInterface::printDecov25LogoOnConsole()
    {
        getConsoleSize();
        int result = RESULT_CODE::RESULT_UNKNOWN;
        char getPath[PATH_MAX];
        cout << "\033[1J";
        ssize_t count = readlink("/proc/self/exe", getPath, PATH_MAX);
        const char *path;
        if (count != -1)
            path = dirname(getPath);

        fstream fileStream;
        string filePath(path);
        filePath.append(LOGO_FILE_TEXT);
        fileStream.open(filePath, fstream::in | fstream::out | fstream::app);
        if (fileStream.fail())
            return;
        string lines[16];
        int i = 0;
        while (!fileStream.eof() && i <= 10)
        {
            getline(fileStream, lines[i++]);
        }
        printHorizontalSeparatedLine();
        if (ConsoleWidth > SMALL_LOGO_WIDTH)
        {
            cout << endl
                 << BOLDMAGENTA;
            unsigned short leftAlign = (ConsoleWidth - 60) / 2;
            for (int i = 0; i < 6; i++)
            {
                for (int j = 0; j < leftAlign; j++)
                    cout << " ";
                cout << lines[i] << endl;
            }
        }
        else
        {
            cout << BOLDMAGENTA;
            unsigned short leftAlign = (ConsoleWidth - 44) / 2;
            for (int i = 6; i < 11; i++)
            {
                for (int j = 0; j < leftAlign; j++)
                    cout << " ";
                cout << lines[i] << endl;
            }
        }
        cout << RESET;
        printHorizontalSeparatedLine();

        fileStream.close();
    }

    void UserInterface::printContentCentered(string str, char alignChar)
    {
        getConsoleSize();
        unsigned short leftAlign = (ConsoleWidth - str.length()) / 2;
        for (int j = 0; j < leftAlign; j++)
            cout << alignChar;
        cout << str;
        for (int j = 0; j < leftAlign; j++)
            cout << alignChar;
        cout << endl;
    }

    void UserInterface::printMenuHeaderWithTitle(string username, string title)
    {
        int sys = system("clear");
        cout << BOLDGREEN;
        UserInterface::printContentCentered(
            "Authenticated as " + username, ' ');
        cout << RESET << endl;
        string tempTitle = " [" + title + "] ";
        UserInterface::printContentCentered(tempTitle, '=');
    }

    int UserInterface::inputChoice(int from, int to)
    {
        cout << BOLDYELLOW << "\nChoose(#): " << RESET;
        int choose = -1;
        cout << "\x1b[s";
        cin >> choose;
        while (choose < from || to < choose || cin.fail())
        {
            if (choose == 0)
            {
                cin.clear();
                cin.ignore();
                cout << "\x1b[u\x1b[K\x1B[F";
                cout << "Enter incorrect number. Please try again!       " << endl;
                cout << "\x1b[u";
            }
            else
            {
                cout << "\x1b[u\x1b[K\x1B[F";
                cout << "We do not have this choice. Please choose again!" << endl;
                cout << "\x1b[u";
            }
            cin >> choose;
        }
        return choose;
    }

    string UserInterface::inputPassword()
    {
        string password;
        int _getch = getch();
        _getch = getch();
        while (_getch != 10)
        {
            if (_getch != 127)
            {
                password.push_back(char(_getch));
                cout << "*";
            }
            else if (password.size() > 0)
            {
                password.pop_back();
                cout << "\033[1D";
                cout << " ";
                cout << "\033[1D";
            }
            _getch = getch();
        }
        cout << endl;
        return password;
    }

    int getch()
    {
        int ch;
        struct termios t_old, t_new;

        tcgetattr(STDIN_FILENO, &t_old);
        t_new = t_old;
        t_new.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &t_new);

        ch = getchar();

        tcsetattr(STDIN_FILENO, TCSANOW, &t_old);
        return ch;
    }

    // InterruptHandler
    condition_variable InterruptHandler::_condition;
    mutex InterruptHandler::_mutex;

    void InterruptHandler::hookSIGINT()
    {
        signal(SIGINT, handleUserInterrupt);
    }
    void InterruptHandler::handleUserInterrupt(int signal)
    {
        if (signal == SIGINT)
        {
            _condition.notify_one();
            cout << endl;
            WARN_LOG("User interrupted");
        }
        cout << endl;
    }
    void InterruptHandler::waitForUserInterrupt()
    {
        INFO_LOG("Waiting user interrupt...\n");
        std::unique_lock<std::mutex> lock{_mutex};
        _condition.wait(lock);
        lock.unlock();
    }
} // namespace MyLibrary