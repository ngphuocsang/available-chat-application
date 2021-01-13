#pragma once

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
#include <string>

using namespace std;

class tagClass
{
private:
    char *Username;

public:
    tagClass();
    string logout       = "00000"; // 00000
    string send         = "00001"; // 00001 IDs %color %msg
    string reqsRoom     = "00002"; // 00002 recvname
    string x            = "00003"; // 00003
    string recv         = "00004"; // 00004 IDs% sender% textcolor% msg
    string linuxcmd     = "00005"; //
    string annouce      = "00006"; //
    string groupsend    = "00007"; // 00007 groupIDs %color %msg
    string grouprecive  = "00008"; // 00008 gorupIDs% sender% textcolor% msg
    string history      = "00009"; // 00007 IDs %sender %msg
    string alive        = "00010"; // 00010
};