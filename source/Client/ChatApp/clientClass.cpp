#include "clientClass.h"
#include "Text_Color.h"
#include "tagClass.h"
#include "MyLibrary.h"
#include "UserInterface.h"

#include <pthread.h>

using namespace Text_Color;
using namespace std;
using namespace text_align;
using namespace MyLibrary;

clientClass *clientClass::_instance = NULL;

tagClass tag;
color_text Color;

clientClass *clientClass::getInstance()
{
    if (_instance == NULL)
    {
        _instance = new clientClass();
    }
    return _instance;
}

string clientClass::getExecutionPath()
    {
        char getPath[PATH_MAX];
        ssize_t count = readlink("/proc/self/exe", getPath, PATH_MAX);
        const char *path;
        if (count != -1)
            path = dirname(getPath);
        return string(path);
    }

int clientClass::initialize()
{
    if ((Sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        return RESULT_CODE::CLIENT_SOCK_INIT_ERR;

    memset(&svIPaddr, '0', sizeof(svIPaddr));
    svIPaddr.sin_family = AF_INET;
    svIPaddr.sin_addr.s_addr = INADDR_ANY;
    svIPaddr.sin_port = htons(
        atoi(AppSetting::SettingValues["ChatServPort"].c_str()));

    if (inet_pton(AF_INET, AppSetting::SettingValues["ServerIP"].c_str(), &svIPaddr.sin_addr) <= 0)
        return RESULT_CODE::IP_ADDRESS_ERR;
    return RESULT_CODE::RESULT_SUCCESS;
}

void *recvMessageFromServer(void *param)
{
    clientClass *client = clientClass::getInstance();
    string fTag;
    string temp;
    string name;
    int id;
    while (1)
    {
        memset(client->recivemsg, 0, sizeof(client->recivemsg));
        recv(client->Sock, client->recivemsg, sizeof(client->recivemsg), 0);
        // get tag
        temp = client->recivemsg;
        fTag = temp.substr(0, 5);
        ///////////////////////

        //client->recivemsg.strtok(NULL,"%");
        if (fTag.compare(tag.x) == 0)
        {
            close(client->Sock);
            break;
        }

        if (fTag.compare(tag.recv) == 0)
        {
            int color;
            temp = temp.substr(5); // xoa tag
            id = stoi(temp.substr(0, temp.find("%")));
            temp = temp.substr(temp.find("%") + 1);
            name = temp.substr(0, temp.find("%")); // lay tenB
            temp = temp.substr(temp.find("%") + 1);
            color = stoi(temp.substr(0, temp.find("%"))); //Lay mau
            temp = temp.substr(temp.find("%") + 1);

            if (id == clientClass::getInstance()->GroupID)
            {
                cout << " ";
                cout << std::left << std::setw(68) << " ";
                cout << " " << endl;
                align::cout_left_with_username(name, color, temp);
                clientClass::getInstance()->chatHistory(temp, name, id, 1);
                continue;
            }
            clientClass::getInstance()->chatHistory(temp, name, id, 1);
            cout << "\033[s";
            cout << "\033[5;0H";
            cout << "\033[K";
            string a = "New message from user: " + name;
            cout << Color.color_Message(a, 1);
            cout << "\033[u";
            //clientClass::getInstance()->statusbar();
        }

        if (fTag.compare(tag.grouprecive) == 0)
        {
            int color;
            temp = temp.substr(5); // xoa tag
            id = stoi(temp.substr(0, temp.find("%")));
            temp = temp.substr(temp.find("%") + 1);
            name = temp.substr(0, temp.find("%")); // lay tenB
            temp = temp.substr(temp.find("%") + 1);
            color = stoi(temp.substr(0, temp.find("%"))); //Lay mau
            temp = temp.substr(temp.find("%") + 1);

            if (id == clientClass::getInstance()->GroupID)
            {
                cout << " ";
                cout << std::left << std::setw(68) << " ";
                cout << " " << endl;
                align::cout_left_with_username(name, color, temp);
                clientClass::getInstance()->chatHistory(temp, name, id, 0);
                continue;
            }
            clientClass::getInstance()->chatHistory(temp, name, id, 0);
            cout << "\033[s";
            cout << "\033[5;0H";
            cout << "\033[K";
            string a = "New message from user: " + name;
            cout << Color.color_Message(a, 1);
            cout << "\033[u";
            //clientClass::getInstance()->statusbar();
        }

        if (fTag.compare(tag.reqsRoom) == 0)
        {
            clientClass::getInstance()->GroupID = stoi(temp.substr(5));
        }
    }
    system("clear");
    cout << "Timeout" << endl;
    cout << "Disconnect from server" << endl;
    exit(0);
    pthread_exit(NULL);
}

int clientClass::connectToServer()
{
    int con = connect(Sock, (struct sockaddr *)&svIPaddr, sizeof(svIPaddr));
    if (con < 0)
    {
        cout << "Connection Failed" << endl;
        return RESULT_CODE::CLIENT_CONN_FAIL;
    }
    cout << "Connect successfully" << endl;
    send(Sock, Username.c_str(), 1024, 0);
    // cout << Username;
    // cout << sizeof(Username);
    //pthread_t threads;
    // int rc = pthread_create(&threads, NULL, recvMessageFromServer, NULL);
    return RESULT_CODE::RESULT_SUCCESS;
}

void *clientClass::sendMessageToServer(string str, string tag)
{
    bzero((void *)msg.c_str(), msg.length());
    msg = tag + to_string(clientClass::getInstance()->GroupID) + "%" + to_string(this->color) + "%" + str;
    // this->chatHistory(str, Username, clientClass::getInstance()->GroupID);
    int code = send(Sock, (void *)msg.c_str(), 1024, 0);
    if (code < 0)
    {
        cout << "------------------------------------------------------------------\n";
        cout << "Send FAIL\n";
        cout << "------------------------------------------------------------------\n";
        // return NULL;
    }
    // memset(sendmsg, 0, sizeof(sendmsg));
    // strcpy(sendmsg, str.c_str());
    // send(Sock,(char*)&sendmsg, strlen(sendmsg),0);
}

void clientClass::CtoC()
{
    cin.ignore();
    while (1)
    {
        string str;
        cin.clear();
        getline(cin, str);
        //statusbar();
        cout << "\033[1A"
             << "\033[K";
        // cout << Username << ": " << Color.color_Message(str, color) << endl;
        cout << " ";
        cout << std::left << std::setw(68) << " ";
        cout << " " << endl;
        align::cout_right_with_username(Username, color, str);
        if (str == "/exit")
        {
            GroupID = -1;
            break;
        }
        clientClass::getInstance()->sendMessageToServer(str, tag.send);
    }
}

void clientClass::CtoG()
{
    cin.ignore();
    while (1)
    {
        string str;
        cin.clear();
        getline(cin, str);
        //statusbar();
        cout << "\033[1A"
             << "\033[K";
        // cout << Username << ": " << Color.color_Message(str, color) << endl;
        cout << " ";
        cout << std::left << std::setw(68) << " ";
        cout << " " << endl;
        align::cout_right_with_username(Username, color, str);
        if (str == "/exit")
        {
            GroupID = -1;
            break;
        }
        clientClass::getInstance()->sendMessageToServer(str, tag.groupsend);
    }
}

int clientClass::Authenticate()
{
    return 1;
}

int clientClass::Logout()
{
    send(Sock, (void *)tag.logout.c_str(), 1024, 0);
    close(Sock);
    cout << "Sending logout message\n";
}

int clientClass::resetClient()
{
    // this->Sock = 0;
    // this->svIPaddr = {0};
    this->Username = "\0";
    // this->sendmsg[1024] = {0};
    this->color = 0;
    this->msg = "/0";
    // this->recivemsg[1024] = {0};
    //this->IDs = 0;
    this->AuthKey = "";
    this->Role = "-1";
    this->GroupID = -1;
}

// int clientClass::chatHistory(string msgHistory, string name, int ID)
// {
//     char cwd[PATH_MAX];
//     getcwd(cwd, sizeof(cwd));
//     string fileName = cwd;
//     fileName = fileName + "/source/Client/chat_history/" + to_string(ID) + Username;
//     fileName = fileName + ".txt";
//     ofstream History;
//     History.open(fileName, std::ios::app);
//     time_t now = time(0);
//     tm *ltm = localtime(&now);
//     char buffer[80];
//     strftime(buffer, 80, "[%d/%m/%Y %H:%M:%S]", ltm);
//     History << buffer << " " << name << ":" << msgHistory << endl;
//     //History.close();
//     return 1;
// }

int clientClass::printChatHistory(int ID)
{
    fstream fileStream;
    string line;
    string buffer;
    int length = fileStream.tellg();
    string historyPath = getExecutionPath() + "/chat_history/";
    string fileName = historyPath + to_string(ID) + Username;
    fileStream.open(fileName, ios::in);
    if (!fileStream)
    {
        fileStream.open(fileName, ios::trunc);
        fileStream.close();
    }
    else
    {
        while (!fileStream.eof())
        {
            getline(fileStream, line);
            if (line.empty())
                break;
            line = line.substr(22);
            string name = line.substr(0, line.find(":"));
            line = line.substr(line.find(":") + 1);
            if(name == Username){
                cout << " ";
                cout << std::left << std::setw(68) << " ";
                cout << " " << endl;
                align::cout_right_with_username(Username, 0, line);
            }
            else
            {
                cout << " ";
                cout << std::left << std::setw(68) << " ";
                cout << " " << endl;
                align::cout_left_with_username(name, 0, line);
            }
        }
    }
    fileStream.close();
    cout << line;
    return 1;
}

int clientClass::chatHistory(string msgHistory, string name, int ID, int grtype){
    string historyPath = getExecutionPath() + "/chat_history/";
    string fileName;
    if(grtype == 0)
        fileName = historyPath + "G" + to_string(ID) + Username;
    else
        fileName = historyPath + to_string(ID) + Username;
    ofstream ofileStream;
    ofstream History;
    History.open(fileName, std::ios::app);
    time_t now = time(0);
    tm *ltm = localtime(&now);
    char buffer[80];
    strftime(buffer, 80, "[%d/%m/%Y %H:%M:%S]", ltm);
    History << buffer << " " << name << ":" << msgHistory << endl;
    //History.close();
    return 1;
}