#include "ChatServer.h"
#include "TAG.h"
#include "ChatRoom.h"
#include "Database.h"
#include "MyLibrary.h"
#include "UserInterface.h"
#include "AccountFunctions.h"

#include <mysql/mysql.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <bits/stdc++.h>
#include <string.h>
#include <algorithm>
#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <cpprest/json.h>

using namespace MyLibrary;
using namespace MyLibrary::Models;
using namespace Server::MySQL::Functions;

#define c_length 1024
///////////////////////////////////////////////
DATABASE_RETURN_CODE db_error;
RECEIVED_MESSAGE_CODE msg_error;
//////////////////////////////////////////////
ChatServer *ChatServer::s_instance = NULL;
ChatServer *Ghost = ChatServer::getInstance();
// ChatServer *sv = ChatServer::getInstance();
Database *mySQL = Database::getInstance();
///////////////////////////////////////////////

void ChatServer::removeActive(std::vector<ClientSock *> &clients, std::string name)
{
    if (name.empty())
    {
        return;
    }
    clients.erase(
        remove_if(clients.begin(), clients.end(), [&](ClientSock *const &cli) {
            return boost::iequals(cli->Username, name);
            return cli->Username == name;
        }),
        clients.end());
    if (AccountFunctions::UpdateAuthKey(name, "NULL"))
    {
        INFO_LOG("ChatUser have been logged out: ");
        cout << name;
    }
    else
    {
        INFO_LOG("Logout Failed");
    }
    ClientSocks.erase(name);
}

int ChatServer::initialize()
{
    RESULT_CODE result = RESULT_CODE::CHAT_SERV_INIT_ERR;
    this->Sock = socket(AF_INET, SOCK_STREAM, 0); //ChatServer Master_sock for listening

    if (Sock < 0)
    {
        ERROR_LOG("Chat server create Master Socket failed\n");
        return result;
    }
    // set_Socket(Sock);

    int opt = 1;
    if (setsockopt(Sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        ERROR_LOG("Chat server set Master Socket option failed\n");
        return result;
    }

    struct sockaddr_in my_addr, server_addr;
    string serverIP = AppSetting::SettingValues["ServerIP"];
    int port = atoi(AppSetting::SettingValues["ChatServPort"].c_str());
    // Sock = socket(AF_INET, SOCK_STREAM, 0); //ChatServer Master_sock for listening
    my_addr.sin_family = AF_INET;
    my_addr.sin_addr.s_addr = inet_addr(serverIP.c_str());
    my_addr.sin_port = htons(port);

    if (bind(Sock, (struct sockaddr *)&my_addr, sizeof(my_addr)) != 0)
    {
        ERROR_LOG("Chat server failed to bind Master Socket\n");
        return result;
    }

    if (listen(Sock, 5) == 0) // Client receive ECONNREFUSED if queue fulled
    {
        SUCCESS_LOG("Chat server listenning on socket at ");
        cout << serverIP << ":" << port << endl;
        result = RESULT_CODE::RESULT_SUCCESS;
    }
    else
    {
        ERROR_LOG("Chat server cannot listenning on socket at ");
        cout << serverIP << ":" << port << endl;
    }

    pthread_t thread_id;
    pthread_create(&thread_id, NULL, acceptClientLooping, (void *)NULL);

    return result;
}

int ChatServer::forwardRoomMsg(int roomID, std::string cur_name, std::string msg, std::string color)
{
    using namespace std;

    // cout << "Getroom userROOMID: " << roomID << endl;
    if (roomID < 0)
    {
        cout << "-------------------------------------------------------------------\n";
        cout << " INVALID ROOMID IGNORED SENDING MESSAGE" << endl;
        cout << "-------------------------------------------------------------------\n";
        return DATABASE_RETURN_CODE::INVALID_ROOMID;
    }
    vector<string> participant = mySQL->getRoomUser(roomID);
    if (participant.size() > 0)
    {
        if (participant[0].compare("FAIL") == 0)
        {
            cout << "-------------------------------------------------------------------\n";
            cout << "ERROR OCCUR UNABLE TO GET ROOM PARTICIPANT, IGNORE SENDING MESSANGE" << endl;
            cout << "-------------------------------------------------------------------\n";
            return DATABASE_RETURN_CODE::UNFOUND_ROOM_USERS;
        }
    }
    else
    {
        return DATABASE_RETURN_CODE::UNFOUND_ROOM_USERS;
    }

    // mySQL->addMessage(cur_name + ":" + msg, roomID);

    msg = TAG::_RECEIVE + to_string(roomID) + "%" + cur_name + "%" + color + "%" + msg;
    for (auto i : participant)
    {
        if (i.compare(cur_name) != 0)
        {

            if (ClientSocks.find(i) == ClientSocks.end()) // check if this user is online, connected to server
            {
                // cout << "User unfound" << endl;
                // // User is currenly not online
            }
            else
            {
                //User online, connected to server
                cout << "Message before sending: " << msg << endl;
                send(ClientSocks[i]->getSocket(), (void *)msg.c_str(), 1024, 0);
            }
        }
    }
    return DATABASE_RETURN_CODE::SUCCESS;
}

int ChatServer::forwardGroupMsg(unsigned int groupID, std::string cur_name, std::string msg, std::string color)
{
    using namespace std;

    cout << "Getroom userGroupID: " << groupID << endl;

    if (groupID == UINT_MAX)
    {
        cout << "-------------------------------------------------------------------\n";
        cout << " INVALID ROOMID IGNORED SENDING MESSAGE" << endl;
        cout << "-------------------------------------------------------------------\n";
        return DATABASE_RETURN_CODE::INVALID_ROOMID;
    }
    vector<string> participant = mySQL->getGroupUser(groupID);
    if (participant.size() > 0)
    {
        if (participant[0].compare("FAIL") == 0)
        {
            cout << "-------------------------------------------------------------------\n";
            cout << "ERROR OCCUR UNABLE TO GET ROOM PARTICIPANT, IGNORE SENDING MESSANGE" << endl;
            cout << "-------------------------------------------------------------------\n";
            return DATABASE_RETURN_CODE::UNFOUND_ROOM_USERS;
        }
    }
    else
    {
        return DATABASE_RETURN_CODE::UNFOUND_ROOM_USERS;
    }
    for (auto i : participant)
    {
        cout << "User got from ROOM: " << i << "|";
    }
    cout << endl;

    msg = TAG::_GROUP_RECEIVE + to_string(groupID) + "%" + cur_name + "%" + color + "%" + msg;
    for (auto i : participant)
    {
        if (i.compare(cur_name) != 0)
        {

            if (ClientSocks.find(i) == ClientSocks.end()) // check if this user is online, connected to server
            {
                cout << "User " << i << " is not online" << endl;
            }
            else
            {
                //User online, connected to server
                cout << "Message before sending: " << msg << endl;
                int code = send(ClientSocks[i]->getSocket(), (void *)msg.c_str(), 1024, 0);
            }
        }
    }
    return DATABASE_RETURN_CODE::SUCCESS;
}

void *recvMessageFromClient(void *param)
{
    using namespace std;
    ChatServer *sv = ChatServer::getInstance();
    ClientSock *client = (ClientSock *)param;
    ////////////////////////////////////////////////////////////////////////////////////////////////
    int acc = client->getSocket();
    if (acc < 0)
    {
        return (void *)RECEIVED_MESSAGE_CODE::UNKNOWN_SOCKET;
    }
    struct timeval timeout;
    timeout.tv_sec = atoi(AppSetting::SettingValues["DefaultTimeout"].c_str());
    timeout.tv_usec = 0;

    if (setsockopt(acc, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout,
                   sizeof(timeout)) < 0)
    {
        perror("setsockopt failed\n");
        return (void *)RECEIVED_MESSAGE_CODE::SETUP_TIMEOUT_FAILED;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    while (1)
    {

        if (client->Username.empty())
        {
            pthread_exit(NULL);
        }
        char buffer3[1024];
        int recv_code = recv(client->getSocket(), buffer3, 1024, 0);

        if (recv_code == -1) //This maybe because the timeout
        {
            if (errno == EAGAIN)
            {
                cout << "Timeout for user " << client->Username << endl;
                cout << "Closing Socket with " << client->Username << endl;
                string timeoutMsg = TAG::_TIMEOUT;
                send(client->getSocket(), (void *)timeoutMsg.c_str(), timeoutMsg.length(), 0);
                sv->removeActive(sv->Clients, client->Username);
                close(client->getSocket());
                break;
            }
            cout << "errno message: " << errno << endl;
            cout << "Coming Here after TIMEOUT" << endl;
            break;
        }
        if (recv_code == 0)
        {
            cout << "This User has close its socket at the other endpoint!! TERMINATE SOCKET\n";
            sv->removeActive(sv->Clients, client->Username);
            close(client->getSocket());
            break;
        }
        client->no_mess++;
        if (client->no_mess == 1000)
        {
            cout << "Success 1" << endl;
        }
        INFO_LOG("SERVER RECEIVED: ");
        cout << buffer3 << endl;
        string req(buffer3); // REAL FORMATED MESSAGE
        if (req.empty() || req.compare("") == 0 || req.length() == 0)
        {
            cout << "Req Empty";
            continue;
        }

        // cout << "ChatServer Received: " << req << endl;
        string TAG = req.substr(0, 5); // Extract TAG from received message
        // cout << "Message Tag: " << TAG << endl;

        req = req.substr(5); // From character 5 to end of string (remove the 5 character tag)

        if (TAG.compare(TAG::_LOGOUT) == 0) //Client want to log out
        {

            sv->removeActive(sv->Clients, client->Username);
            close(client->getSocket());

            break;
        }
        else if (TAG.compare(TAG::_SEND) == 0) // Client send to server 1-1 message
        {
            //FORMAT:  IDRoom % color % MSG
            vector<string> result;
            stringstream s_stream(req);
            int count = 0;
            while (s_stream.good() && count < 2)
            {
                string substr;
                getline(s_stream, substr, '%'); //get first string delimited by %
                result.push_back(substr);
                count++;
            }
            string msg;
            getline(s_stream, msg); //TRUE MESSAGE WITHOUT ANY TAG
            if (result.size() != 2 || stoi(result[0]) < 0 || stoi(result[1]) < 0)
            {
                cout << "-------------------------------------------------------------------\n";
                cout << "ERROR OCCUR " << RECEIVED_MESSAGE_CODE::INVALID_SEND_FORMAT << ": INVALID_SEND_FORMAT" << endl;
                cout << "-------------------------------------------------------------------\n";
                string msg = "Action incompleted, please try again later";
                send(client->getSocket(), (void *)msg.c_str(), msg.length(), 0);
                continue;
            }
            int roomID = stoi(result[0]);
            string color = result[1];
            // cout << "RoomId: " << roomID << endl;
            // cout << "Color: " << color << endl;
            cout << "trying to forward message : " << msg << endl;
            if (!msg.empty())
                if (db_error = (DATABASE_RETURN_CODE)sv->forwardRoomMsg(roomID, client->Username, msg, color))
                    if (db_error != DATABASE_RETURN_CODE::SUCCESS)
                    {
                        cout << "-------------------------------------------------------------------\n";
                        cout << "(" << db_error << ") ERROR OCCUR WHILE TRYING TO FORWARDING MESSAGE TO ROOM" << endl;
                        cout << "-------------------------------------------------------------------\n";
                    }
        }
        else if (TAG.compare(TAG::_REQROOM) == 0) // Client finding room
        {
            int roomID;
            vector<string> insider;
            insider.push_back(client->Username);
            insider.push_back(req);

            roomID = mySQL->createChatRoom(insider);
            cout << "Roomid: " << roomID << endl;
            if (roomID > 0)
            {
                //Room found or created
                //check if this client is still online (connected to server)
                if (sv->ClientSocks.find(client->Username) != sv->ClientSocks.end())
                {
                    string returnRoom = TAG::_REQROOM + to_string(roomID);
                    cout << "Room found: " << returnRoom << endl;
                    send(client->getSocket(), (void *)returnRoom.c_str(), returnRoom.length(), 0);
                }
            }
            else
            {
                //Error finding/Creating room
                if (sv->ClientSocks.find(client->Username) != sv->ClientSocks.end())
                {
                    string returnRoom = "#Error occur while Finding Room";
                    cout << "Room unfound: " << returnRoom << endl;
                    send(client->getSocket(), (void *)returnRoom.c_str(), returnRoom.length(), 0);
                }
            }
        }
        else if (TAG.compare(TAG::_GROUP_SEND) == 0)
        {
            vector<string> result;
            stringstream s_stream(req);
            int count = 0;
            while (s_stream.good() && count < 2)
            {
                string substr;
                getline(s_stream, substr, '%'); //get first string delimited by %
                result.push_back(substr);
                count++;
            }
            string msg;
            getline(s_stream, msg); //TRUE MESSAGE WITHOUT ANY TAG
            if (result.size() != 2 || stoi(result[0]) < 0 || stoi(result[1]) < 0)
            {
                cout << "-------------------------------------------------------------------\n";
                cout << "ERROR OCCUR " << RECEIVED_MESSAGE_CODE::INVALID_SEND_FORMAT << ": INVALID_GROUP_SEND_FORMAT" << endl;
                cout << "-------------------------------------------------------------------\n";
                string msg = "Action incompleted, please try again later";
                send(client->getSocket(), (void *)msg.c_str(), msg.length(), 0);
                continue;
            }
            int roomID = stoi(result[0]);
            string color = result[1];
            vector<string> users = mySQL->getGroupUser(roomID);
            bool inside = false;
            for (auto i : users)
            {
                if (i.compare(client->Username) == 0)
                {
                    inside = true;
                    break;
                }
            }
            if (inside == true)
                if (!msg.empty())
                    if (db_error = (DATABASE_RETURN_CODE)sv->forwardGroupMsg(roomID, client->Username, msg, color))
                        if (db_error != DATABASE_RETURN_CODE::SUCCESS)
                        {
                            cout << "-------------------------------------------------------------------\n";
                            cout << "(" << db_error << ") ERROR OCCUR WHILE TRYING TO FORWARDING MESSAGE TO GROUP" << endl;
                            cout << "-------------------------------------------------------------------\n";
                        }
        }
    }
    pthread_exit(NULL);
}

int ChatServer::addClient(ClientSock *visitor)
{
    if (visitor == NULL)
        return -1;
    Clients.push_back(visitor);
    ClientSocks[visitor->Username] = visitor;
    return 0;
}
void *processWaiter(void *par)
{
    int pid = *((int *)par);
    int status;
    waitpid(pid, &status, 0);
}

void *acceptClientLooping(void *param)
{
    ChatServer *server = ChatServer::getInstance();
    struct sockaddr_in client_addr;
    socklen_t addr_size = sizeof(struct sockaddr_in);
    char buffer[c_length];

    while (1)
    {
        int sock = accept(server->Sock, (struct sockaddr *)&client_addr, &addr_size);
        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(client_addr.sin_addr), ip, INET_ADDRSTRLEN);

        if (sock < 0)
        {
            ERROR_LOG(string("Accepting client failed on IP: ") + string(ip));
            cout << errno << endl;
            // break;
        }
        else
        {
            INFO_LOG("Sending & receiving initial connection message to client\n");

            // strcpy(buffer, "Welcome the DECOV25");
            // send(sock, buffer, c_length, 0);

            char critical[1024];
            // bzero(critical, c_length);
            int recv_code = recv(sock, critical, 1024, 0);

            std::string passport(critical);
            ClientSock *cli = new ClientSock(passport, sock, client_addr);
            server->addClient(cli);
            SUCCESS_LOG("Connection established with IP: ");
            printf("%s and PORT : %d on Username: %s\n",
                   ip, ntohs(client_addr.sin_port), critical);

            pthread_t thread_id;
            pthread_create(&thread_id, NULL, recvMessageFromClient, (void *)cli);
        }
    }
}