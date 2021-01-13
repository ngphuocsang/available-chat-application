#include "MyLibrary.h"
#include "UserInterface.h"

#include <iostream>
#include <unistd.h>
#include <random>
#include <unistd.h>
#include <limits.h>
#include <libgen.h>
#include <cryptopp/sha.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <chrono>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

using namespace std;

namespace MyLibrary
{
    string generateSHA1(string value)
    {
        string sha1 = "";
        CryptoPP::SHA1 sha1Crypto;
        auto stringSink = new CryptoPP::StringSink(sha1);
        auto hexEndcoder = new CryptoPP::HexEncoder(stringSink);
        auto hashFilter = new CryptoPP::HashFilter(sha1Crypto, hexEndcoder);
        CryptoPP::StringSource(value, true, hashFilter);
        std::for_each(sha1.begin(), sha1.end(), [](char &c) {
            c = ::tolower(c);
        });
        return sha1;
    }

    string generateAuthenKey()
    {
        using namespace std::chrono;
        milliseconds ms = duration_cast<milliseconds>(
            system_clock::now().time_since_epoch());
        return string(generateSHA1(to_string(ms.count())));
    }

    string getIpAddressInLAN()
    {
        int server = socket(AF_INET, SOCK_STREAM, 0);
        if (server < 0)
            return "127.0.0.1";

        struct sockaddr_in my_addr, peer_addr;
        my_addr.sin_family = AF_INET;
        my_addr.sin_addr.s_addr = inet_addr("8.8.8.8");
        my_addr.sin_port = htons(53); // DNS Port
        if (!bind(server, (struct sockaddr *)&my_addr, sizeof(struct sockaddr_in) == 0))
            return "127.0.0.1";
        if (connect(server, (struct sockaddr *)&my_addr, sizeof my_addr))
            return "127.0.0.1";

        struct sockaddr_in server_addr;
        socklen_t namelen = sizeof(server_addr);
        getsockname(server, (struct sockaddr *)&server_addr, &namelen);
        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(server_addr.sin_addr), ip, INET_ADDRSTRLEN);
        std::string serverIP(ip);
        return serverIP;
    }

    string getExecutionPath()
    {
        char getPath[PATH_MAX];
        ssize_t count = readlink("/proc/self/exe", getPath, PATH_MAX);
        const char *path;
        if (count != -1)
            path = dirname(getPath);
        return string(path);
    }

    bool is_digits(const std::string &str)
    {
        std::size_t found = str.find_first_not_of("0123456789");
        bool result = found == std::string::npos;
        return result;
    }

    bool is_valid_username(const std::string &str)
    {
        std::size_t found = str.find_first_not_of("0123456789abcdefghijklmnopqrstuvwxyz_");
        bool result = found == std::string::npos;
        return result;
    }

    unordered_map<string, string> AppSetting::SettingValues;

    int AppSetting::loadSettingServerFromFile()
    {
        int result = RESULT_CODE::RESULT_UNKNOWN;

        fstream fileStream;
        string filePath = getExecutionPath() + APP_SETTING_FILE;
        fileStream.open(filePath, fstream::in | fstream::out | fstream::app);
        if (fileStream.fail())
            result = rewriteDefaultServerSettingFile(filePath);
        // rewriteDefaultServerSettingFile(filePath);
        string line;
        while (!fileStream.eof())
        {
            getline(fileStream, line);
            int tab_pos = line.find('=');
            if (tab_pos == -1)
            {
                ERROR_LOG("Your 'appSetting.txt' file's format is not correct\n");
                rewriteDefaultServerSettingFile(filePath);
                fileStream.close();
                return RESULT_CODE::CANNOT_READ_SETTING;
            }
            SettingValues[line.substr(0, tab_pos)] =
                line.substr(tab_pos + 1, line.length());
        }

        fileStream.close();
        return RESULT_CODE::RESULT_SUCCESS;
    }

    int AppSetting::loadSettingClientFromFile()
    {
        int result = RESULT_CODE::RESULT_UNKNOWN;
        fstream fileStream;
        string filePath = getExecutionPath() + APP_SETTING_FILE;
        fileStream.open(filePath, fstream::in | fstream::out | fstream::app);
        if (fileStream.fail())
            result = rewriteDefaultClientSettingFile(filePath);
        // rewriteDefaultClientSettingFile(filePath);
        string line;
        while (!fileStream.eof())
        {
            getline(fileStream, line);
            int tab_pos = line.find('=');
            if (tab_pos == -1)
            {
                ERROR_LOG("Your 'appSetting.txt' file's format is not correct\n");
                rewriteDefaultClientSettingFile(filePath);
                fileStream.close();
                return RESULT_CODE::CANNOT_READ_SETTING;
            }
            SettingValues[line.substr(0, tab_pos)] =
                line.substr(tab_pos + 1, line.length());
        }

        fileStream.close();
        return RESULT_CODE::RESULT_SUCCESS;
    }

    int AppSetting::rewriteDefaultServerSettingFile(string filepath)
    {
        fstream fileStream;
        fileStream.open(filepath, fstream::out);
        fileStream << "ServerIP=" << getIpAddressInLAN() << endl;
        fileStream << "RestServPort=8080\n";
        fileStream << "ChatServPort=8888\n";
        fileStream << "DefaultTimeout=300\n";
        fileStream << "MySqlServIP=acadbservice\n";
        fileStream << "MySqlServUser=acauser\n";
        fileStream << "MySqlServDbname=acadb\n";
        fileStream << "MySqlServPass=acapasswd";
        fileStream.close();
        INFO_LOG("New file 'appSetting.txt' has been created in: ");
        cout << filepath << endl;
        INFO_LOG("You should to change value in it to make sure application is run correctly\n");
        return RESULT_CODE::NEED_TO_CHANGE_SETTING;
    }

    int AppSetting::rewriteDefaultClientSettingFile(string filepath)
    {
        fstream fileStream;
        fileStream.open(filepath, fstream::out);
        fileStream << "ServerIP=127.0.0.1\n";
        fileStream << "RestServPort=8080\n";
        fileStream << "ChatServPort=8888";
        fileStream.close();
        INFO_LOG("New file 'appSetting.txt' has been created in: ");
        cout << filepath << endl;
        INFO_LOG("You should to change value in it to make sure application is run correctly\n");
        return RESULT_CODE::NEED_TO_CHANGE_SETTING;
    }
} // namespace MyLibrary
