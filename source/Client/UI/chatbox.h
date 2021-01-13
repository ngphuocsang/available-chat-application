#ifndef CHATBOX_H
#define CHATBOX_H

#include <QWidget>
#include <vector>
#include <string>
#include <QString>
#include <QListWidget>
#include <QListWidgetItem>
#include <vector>
#include <pthread.h>
#include <QScrollBar>
#include <QMessageBox>
#include <QDebug>
#include <QInputDialog>
#include <QDir>
#include "tagClass.h"
#include "clientClass.h"
#include "AccountsAPI.h"
#include "MyLibrary.h"
#include "chatbox.h"
#include "roomClass.h"
#include "Thread.h"

using namespace std;

namespace Ui {
class chatbox;
}

class chatbox : public QWidget
{
    Q_OBJECT

public:
    explicit chatbox(QWidget *parent = nullptr);
    int numberOfUser;
    vector<string> name;
    QString currentitem;
    vector<string> activeName;
    vector<string> nameHistory;
    vector<string> msgHistory;
    vector<roomClass> listRoom;
    vector<string> currentGroupUser;
    void init();
    int oldID;
    static chatbox* getInstance();
    void sendmsg(QString msg, QString name, int color);
    void printChatHistory(int grtype);
    ~chatbox();

private slots:
    void on_sendButton_clicked();

    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_listWidget_itemPressed(QListWidgetItem *item);

    void on_settingButton_clicked();

    void on_SettingbackButton_clicked();

    void on_groupManagementButton_clicked();

    void on_accountManagementButton_clicked();

    void on_groupManagementBackButton_clicked();

    void on_accountManagementBackButton_clicked();

    void on_textColorBackButton_clicked();

    void on_LogoutButton_clicked();

    void on_textColorButton_clicked();

    void on_redButton_clicked();
    
    void on_blackButton_clicked();
    
    void on_blueButton_clicked();
    
    void on_greenButton_clicked();

    void on_accountManagementAcceptButton_clicked();

    void on_leaveGroupButton_clicked();
    
    void on_inviteGroupButton_clicked();
    
    void on_groupWidget_itemPressed(QListWidgetItem *item);

    void on_accessButton_clicked();

    void on_removeMemberButton_clicked();

    void on_listWidget_2_itemPressed(QListWidgetItem *item);

    void printMsg(QString name, QString msg, int color);

    void on_createGroupButton_clicked();

    void on_deleteGroupButton_clicked();

    void on_modifyGroupButton_clicked();

    void sendNotify(QString Name, int grtype);

private:
    Ui::chatbox *ui;
    Thread *mythread;
    void getuser();
    void sendmsgtoserver(string str, string tag, int grtype);
    void getGroup(); 
    static chatbox *_instance;  
    // chatbox(){
    //     _instance = this;
    // };
};

#endif // CHATBOX_H
