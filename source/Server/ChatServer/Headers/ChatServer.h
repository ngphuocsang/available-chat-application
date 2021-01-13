#ifndef _CHAT_SERVER_H
#define _CHAT_SERVER_H

#include "ClientSock.h"
#include "ChatRoom.h"
#include <vector>
#include <unordered_map>
#include <string>
#include <string.h>

void *recvMessageFromClient(void *param);
void *acceptClientLooping(void *param);

using namespace MyLibrary::Models;

class ChatServer
{
private:
    static ChatServer *s_instance;

public:
    int Sock = -1;
    void *sendMessageToClient(std::string msg, int sender_sock, int receiver_sock);

    std::vector<ClientSock *> Clients;
    std::unordered_map<std::string, ClientSock *> ClientSocks;
    std::vector<ChatRoom *> Rooms;

    static ChatServer *getInstance()
    {
        if (!s_instance)
            s_instance = new ChatServer;
        return s_instance;
    }

    int addClient(ClientSock *visitor);
    std::string detectIP();
    void removeActive(std::vector<ClientSock *> &clients, std::string name);
    void set_Socket(int s);
    int get_Socket();

    int initialize();
    int forwardRoomMsg(int roomID, std::string cur_name, std::string msg, std::string color);
    int forwardGroupMsg(unsigned int groupID, std::string cur_name, std::string msg, std::string color);

    friend void *acceptClientLooping(void *param);
};

#endif