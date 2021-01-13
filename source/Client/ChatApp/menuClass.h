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
#include "AccountsAPI.h"
#include "RoomAPI.h"
#include "roomClass.h"

class menuClass{
    private:
        int choose;
        char* username;       
    public:
        menuClass();
        int loginForm();
        int colorMenu();
        int mainMenu();
        int activeMemMenu();
        int groupChat();
        int entireMember();

        int chatRoom(int isChatGroup);
        int showListRoom();
        int createRoomForm();
        int manageRoomForm();        
};