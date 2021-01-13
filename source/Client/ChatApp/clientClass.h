#pragma once
#include <string>
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
#include "menuClass.h"
#include "tagClass.h"
#include <cstring>
#include <fstream>
#include <ctime>
#include "text_align.h"
#include <cctype>
#include <libgen.h>

using namespace std;

class clientClass{
    private:
        static clientClass *_instance;
        clientClass(){
            _instance = this;
        };       
    public:
        //ofstream History;
        int Sock;
        struct sockaddr_in svIPaddr;
        string Username;
        //string senderName = "asd123";
        char sendmsg[1024];
        int color;
        string msg;
        char recivemsg[1024] = {0};
        //int IDs;
        string AuthKey;
        string Role = "-1";
        int GroupID = -1;
        //vector<string> ActiveClients;
    public:
        static clientClass* getInstance();
        int initialize();
        int connectToServer();
        void* sendMessageToServer(string str, string tag);
        void CtoC();
        void CtoG();
        int Authenticate();
        int Logout();
        int resetClient();
        int chatHistory(string msgHistory,string name, int ID, int grtype);
        int printChatHistory(int ID);
        string getExecutionPath();
};