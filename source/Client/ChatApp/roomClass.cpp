#include "roomClass.h"
#include "RoomAPI.h"

using namespace Client::RestAPI;

vector<roomClass> roomClass::getListRoom(){
    vector<roomClass> listRoom;
    GetRoomResponse res = RoomAPI::getRoom(clientClass::getInstance()->AuthKey);
    int index = 0;
    for(int index = 0;index < res.Name.size() ;index++){
        listRoom.push_back(roomClass(res.Id.at(index),res.Access.at(index),res.Name.at(index),res.Owner.at(index)));
    }
    return listRoom;
}

roomClass::roomClass(int ids, int typeofroom, string name, string owner){
    IDs = ids;
    TypeOfRoom = typeofroom;
    Name = name;
    Owner = owner;
}

int roomClass::createRoom(string roomName, bool type){
    
    return 0;
}

int roomClass::deleteRoom(){
    
    return 0;
}