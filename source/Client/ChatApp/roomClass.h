#pragma once

#include "clientClass.h"

using namespace std;

class roomClass{
    public:
        int IDs;
        string Name;
        string Owner;
        vector<string> Member;
        int TypeOfRoom;
        static int Password;
    public:
        roomClass();
        roomClass(int ids, int typeofroom, string name, string owner);
        int createRoom(string roomName, bool type);
        //int modifyRoom();
        static vector<roomClass> getListRoom();
        int deleteRoom();
};