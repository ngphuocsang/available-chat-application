#pragma once

#include <string>
#include <fstream>
#include <unordered_map>

#define APP_SETTING_FILE "/appSetting.txt"

using namespace std;

namespace MyLibrary
{
    string generateAuthenKey();
    string generateSHA1(string value);
    string getIpAddressInLAN();
    string getExecutionPath();
    bool is_digits(const std::string &str);
    bool is_valid_username(const std::string &str);

    class AppSetting
    {
    public:
        static unordered_map<string, string> SettingValues;

        static int loadSettingClientFromFile();
        static int loadSettingServerFromFile();

    private:
        static int rewriteDefaultServerSettingFile(string filepath);
        static int rewriteDefaultClientSettingFile(string filepath);
    };

    enum RESULT_CODE
    {
        RESULT_SUCCESS = 0x0000,
        RESULT_UNKNOWN = 0x0001,
        NEED_TO_CHANGE_SETTING = 0x0002,
        CANNOT_READ_SETTING = 0x0003,
        REST_SERV_INIT_ERR = 0x0010,
        DB_SERV_INIT_ERR = 0x0011,
        CLIENT_SOCK_INIT_ERR = 0x0012,
        IP_ADDRESS_ERR = 0x0013,
        CLIENT_CONN_FAIL = 0x0014,
        CHAT_SERV_INIT_ERR = 0x0015,
        UNAUTHENTICATION = 0x0020,
        USERNAME_NOT_FOUND = 0x0021,
        PASSWORD_INVALID = 0x0022,
        GROUP_PASSWORD_INVALID = 0x0023,
        GROUP_OWNER_INVALID = 0x0024,
        GROUP_INEXIST = 0x0025,
        GROUP_OWNER_OR_PASSWORD_INVALID = 0x0026,
        GROUP_NAME_EMPTY = 0x0027,
        GROUP_ACCOUNT_INVALID = 0x0028,
        GROUP_PASSWORD_NULL = 0x0029,
        GROUP_READ_USER_FAIL = 0x0030,
        GROUP_UNFOUND = 0x0031,
        ACCOUNT_AUTHENTICATED = 0x0032,
        GROUP_UNCHANGE = 0x0033,

        GENERAL_RESULT_SUCCESSFUL = 0x0000,
        GENERAL_RESULT_UNSUCCESSFUL = 0x100C,

        ACCOUNT_UNAUTHENTICATION = 0x1008,
        ACCOUNT_UNAUTHORIZATION = 0x1009,

        AUTHEN_ACCOUNT_INVALID_USERNAME = 0x1006,
        AUTHEN_ACCOUNT_INVALID_PASSWORD = 0x100A,
        AUTHEN_ACCOUNT_AUTHENTICATED = 0x100B,

        CREATE_ACCOUNT_INVALID_USERNAME = 0x1001,
        CREATE_ACCOUNT_INVALID_PASSWORD = 0x1002,
        CREATE_ACCOUNT_INVALID_ROLE = 0x1003,

        UPDATE_ACCOUNT_INVALID_USERNAME = 0x1007,
        UPDATE_ACCOUNT_INVALID_PASSWORD = 0x1004,
        UPDATE_ACCOUNT_INVALID_ROLE = 0x1005,

        DELETE_ACCOUNT_INVALID_USERNAME = 0x100C,
    };
    static unordered_map<MyLibrary::RESULT_CODE, string> RC_converter;
    // RC_converter[RESULT_CODE::RESULT_SUCCESS] = "Success";
    // RC_converter[RESULT_CODE::GROUP_PASSWORD_INVALID] = "Group Password is Invalid";
    // RC_converter[RESULT_CODE::GROUP_OWNER_INVALID] = "Group Owner is Invalid";
    // RC_converter[RESULT_CODE::GROUP_INEXIST] = "Group inexist";
    // RC_converter[RESULT_CODE::GROUP_OWNER_OR_PASSWORD_INVALID] = "Group Owner or Password is Invalid";
    // RC_converter[RESULT_CODE::GROUP_NAME_EMPTY] = "Group name empty";
    // RC_converter[RESULT_CODE::GROUP_ACCOUNT_INVALID] = "Read Account Invalid";
    // RC_converter[RESULT_CODE::GROUP_PASSWORD_NULL] = "Password is NULL";
    // RC_converter[RESULT_CODE::GROUP_READ_USER_FAIL] = "Read Group User Fail";
} // namespace MyLibrary

#define PORT_NO 8080
#define MSG_MAX_LEN 256
#define UNAM_MAX_LEN 32
#define SEND_CNT_MAX_LEN 300

enum ERROR_CODE
{
    AUTH_SUCCESS = 0x0000,
    UNAME_NOT_FOUND = 0x0001,
    PASSWD_INVALID = 0x0002,
    UNKNOW = 0xFFFF,
};

enum DATABASE_RETURN_CODE
{
    SUCCESS = 0x0000,
    INVALID_ROOMID = 0x0001,
    UNFOUND_ROOM_USERS = 0x0002,

    INVALID_GROUPID = 0x0010,
    UNFOUND_GROUP_SERS = 0x0011,

};
enum RECEIVED_MESSAGE_CODE
{
    UNKNOWN_SOCKET = 0x0001,
    SETUP_TIMEOUT_FAILED = 0x0002,
    INVALID_SEND_FORMAT = 0x0003,
    INVALID_REQROOM_FORMAT = 0x0004,
    INVALID_RECEIVE_FORMAT = 0x0005,

};
