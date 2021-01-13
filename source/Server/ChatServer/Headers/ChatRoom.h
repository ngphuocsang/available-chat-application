#ifndef _CHAT_ROOM_H
#define _CHAT_ROOM_H

#include <vector>
#include <string>
#include <string.h>

class ChatRoom
{
public:
    int id;
    std::vector<std::string> Insider;
    ChatRoom(int roomID) { id = roomID; }
    ChatRoom() {}
};
#endif