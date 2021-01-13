#pragma once
#include <string>
using namespace std;

namespace TAG
{
    string _LOGOUT = "00000";
    string _SEND = "00001"; // 00001 IDs %color %msg
    string _REQROOM = "00002";
    string _TIMEOUT = "00003";
    string _RECEIVE = "00004";
    string _LINUXCMD = "00005";
    string _ANNOUNCE = "00006";
    string _GROUP_SEND = "00007";    // 00007 groupIDs %color %msg
    string _GROUP_RECEIVE = "00008"; //00008 gorupIDs% sender% textcolor% msg
    string _KEEP_A_LIVE = "00009";
} // namespace TAG
