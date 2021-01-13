#include "Thread.h"
#include "tagClass.h"
#include "chatbox.h"
#include <QDebug>

tagClass tagthread;

Thread::Thread()
{

}

void Thread::run(){
    clientClass *client = clientClass::getInstance();
    string fTag = "";
    string temp = "";
    string name = "";
    int id = 0;
    while (1)
    {
        memset(client->recivemsg, 0, sizeof(client->recivemsg));
        recv(client->Sock, client->recivemsg, sizeof(client->recivemsg), 0);
        // get tag
        temp = client->recivemsg;
        fTag = temp.substr(0, 5);
        ///////////////////////

        //client->recivemsg.strtok(NULL,"%");
        if (fTag.compare(tagthread.x) == 0)
        {
            close(client->Sock);
            break;
        }

        if (fTag.compare(tagthread.recv) == 0)
        {
            int color;
            temp = temp.substr(5); // xoa tag
            id = stoi(temp.substr(0, temp.find("%")));
            temp = temp.substr(temp.find("%") + 1);
            name = temp.substr(0, temp.find("%")); // lay tenB
            temp = temp.substr(temp.find("%") + 1);
            color = stoi(temp.substr(0, temp.find("%"))); //Lay mau
            temp = temp.substr(temp.find("%") + 1);
            if(name.compare(chatbox::getInstance()->currentitem.toStdString()) == 0){
                emit getUserMsg(QString::fromStdString(name), QString::fromStdString(temp), color);
                clientClass::getInstance()->chatHistory(temp, name, id, 1);
                continue;
            } 
            emit notify(QString::fromStdString(name), 1); 
            clientClass::getInstance()->chatHistory(temp, name, id, 1);
            //clientClass::getInstance()->statusbar();
        }

        if (fTag.compare(tagthread.grouprecive) == 0)
        {
            int color;
            temp = temp.substr(5); // xoa tag
            id = stoi(temp.substr(0, temp.find("%")));
            temp = temp.substr(temp.find("%") + 1);
            name = temp.substr(0, temp.find("%")); // lay tenB
            temp = temp.substr(temp.find("%") + 1);
            color = stoi(temp.substr(0, temp.find("%"))); //Lay mau
            temp = temp.substr(temp.find("%") + 1);

            if(id == clientClass::getInstance()->GroupID){
                emit getUserMsg(QString::fromStdString(name), QString::fromStdString(temp), color);
                clientClass::getInstance()->chatHistory(temp, name, id, 0);
                continue;
            }
            clientClass::getInstance()->chatHistory(temp, name, id, 0);
            emit notify(QString::fromStdString(to_string(id)), 0);
            // clientClass::getInstance()->chatHistory(temp, name, id);
            // clientClass::getInstance()->groupChatHistory();
            // cout << "\033[s";
            // cout << "\033[5;0H";
            // cout << "\033[K";
            // string a = "New message from user: " + name;
            // cout << Color.color_Message(a, 1);
            // cout << "\033[u";     
            //clientClass::getInstance()->statusbar();
        }

        if (fTag.compare(tagthread.reqsRoom) == 0)
        {
            clientClass::getInstance()->GroupID = stoi(temp.substr(5));
        }
    }
    cout << "Timeout" << endl;
    cout << "Disconnect from server" << endl;
    exit(0);
}