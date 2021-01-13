#include "ClientSock.h"
namespace MyLibrary::Models
{
    ClientSock::ClientSock(std::string Name, int Sock, sockaddr_in Addr)
    {
        this->Username = Name;
        this->Socket = Sock;
        this->SocketAddr = Addr;
        this->no_mess=0;
    }
} // namespace Server::ChatServer
