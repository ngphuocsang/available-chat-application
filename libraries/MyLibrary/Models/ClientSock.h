#pragma once

#include <vector>
#include <unordered_map>
#include <netinet/in.h>

namespace MyLibrary::Models
{
    class ClientSock
    {
    private:
        int Socket;
        sockaddr_in SocketAddr;

    public:
        unsigned int no_mess = 0;
        std::string Username;
        ClientSock(std::string Name, int Sock, sockaddr_in Addr);
        int getSocket() { return Socket; }
    };
} // namespace MyLibrary::Models