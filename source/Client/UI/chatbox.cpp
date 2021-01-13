#include "chatbox.h"
#include "ui_chatbox.h"
#include <QMessageBox>

using namespace std;
using namespace MyLibrary;
using namespace Client::RestAPI;
using namespace MyLibrary::Models::Requests;
using namespace MyLibrary::Models::Responses;
using namespace Client::RestAPI;

tagClass tagbox;

chatbox *chatbox::_instance = NULL;

// string getExecutionPath()
// {
//     char getPath[PATH_MAX];
//     ssize_t count = readlink("/proc/self/exe", getPath, PATH_MAX);
//     const char *path;
//     if (count != -1)
//         path = dirname(getPath);
//     return string(path);
// }

void chatbox::printChatHistory(int grtype)
{
    // while(clientClass::getInstance()->GroupID == oldID){

    // }
    ui->textEdit->clear();
    usleep(100000);
    fstream Read;
    string line;
    string buffer;
    int length = Read.tellg();
    char cwd[PATH_MAX];
    getcwd(cwd, sizeof(cwd));
    string fileName;
    string historyPath = clientClass::getInstance()->getExecutionPath() + "/chat_history/";
    if(grtype == 0)
        fileName = historyPath + "G" + to_string(clientClass::getInstance()->GroupID) + clientClass::getInstance()->Username;
    else
        fileName = historyPath + to_string(clientClass::getInstance()->GroupID) + clientClass::getInstance()->Username;
    //cout << fileName << endl;
    Read.open(fileName, ios::in);
    if (!Read)
    {
        Read.open(fileName, ios::trunc);
        Read.close();
    }
    else
    {
        int index = 0;
        while (!Read.eof())
        {
            getline(Read, line);
            if (line.empty())
            {
                break;
            }
            //cout << line << endl;
            line = line.substr(22);
            //cout << line << endl;
            string name = line.substr(0, line.find(":"));
            //cout << line.substr(0,line.find(":")) << endl;
            line = line.substr(line.find(":") + 1);
            //cout << line.substr(0,line.find("[")) << endl;
            //line = line.substr(line.find("%")+1);

            if (name == clientClass::getInstance()->Username)
            {
                chatbox::getInstance()->sendmsg(QString::fromStdString(line), QString::fromStdString(clientClass::getInstance()->Username), 0);
            }
            else
            {
                chatbox::getInstance()->sendmsg(QString::fromStdString(line), QString::fromStdString(name), 0);
            }
            index++;
        }
    }
    Read.close();
    cout << line;
    //pthread_exit(NULL);
}

void chatbox::init(){
    this->setWindowTitle(QString::fromStdString(clientClass::getInstance()->Username));
    ui->groupPassEdit->hide();
    ui->stackedWidget->currentWidget()->hide();
    ui->stackedWidget->setCurrentWidget(ui->stackedWidget->widget(0));
    ui->stackedWidget->currentWidget()->show();
    ui->textEdit->setReadOnly(1);
    QPalette pal = ui->redButton->palette();
    pal.setColor(QPalette::Button, QColor(Qt::red));
    ui->redButton->setAutoFillBackground(true);
    ui->redButton->setPalette(pal);
    ui->redButton->update();
    pal.setColor(QPalette::Button, QColor(Qt::green));
    ui->greenButton->setAutoFillBackground(true);
    ui->greenButton->setPalette(pal);
    ui->greenButton->update();
    pal.setColor(QPalette::Button, QColor(Qt::blue));
    ui->blueButton->setAutoFillBackground(true);
    ui->blueButton->setPalette(pal);
    ui->blueButton->update();
    pal.setColor(QPalette::Button, QColor(Qt::gray));
    ui->blackButton->setAutoFillBackground(true);
    ui->blackButton->setPalette(pal);
    ui->blackButton->update();
    ui->textEdit->clear();
    ui->listWidget->clear();
    ui->listWidget_2->clear();
    ui->groupWidget->clear();
    ui->groupMemberWidget->clear();
    ui->listWidget->setIconSize(QSize(10,10));
    ui->listWidget_2->setIconSize(QSize(10,10));
    getGroup();
    getuser();
    // for(int i=0;i<name.size();i++){
    //     cout <<name.at(i);
    // }
    ActiveAccountsRequest req(clientClass::getInstance()->AuthKey);
    vector<string> activeName = AccountsAPI::getActiveAccounts(req).Usernames;
    char cwd[PATH_MAX];
    getcwd(cwd, sizeof(cwd));
    string fileName = cwd;
    fileName = clientClass::getInstance()->getExecutionPath() +"/resources/active-icon-24";
    fileName = fileName + ".png";

    int index = 0;
    for (int i = 0; i < name.size(); i++)
    {
        if (name.at(i).compare(clientClass::getInstance()->Username) == 0)
        {
            continue;
        }
        QListWidgetItem *item = new QListWidgetItem();
        item->setText(QString::fromStdString(name.at(i)));
        if (index != activeName.size())
        {
            if (name.at(i).compare(activeName.at(index)) == 0)
            {
                item->setIcon(QIcon(QString::fromStdString(fileName)));
                index++;
                if (index == activeName.size())
                {
                    index--;
                }
            }
        }
        ui->listWidget->insertItem(i, item);
    }

    fileName = clientClass::getInstance()->getExecutionPath() +"/resources/locked";
    fileName = fileName + ".png";


    for(int i=0;i < listRoom.size();i++){
        QListWidgetItem *item = new QListWidgetItem();
        item->setText(QString::fromStdString(listRoom.at(i).Name));
        if(listRoom.at(i).TypeOfRoom == 1){
            item->setIcon(QIcon(QString::fromStdString(fileName)));
        }
        ui->listWidget_2->insertItem(i,item);
    }
}

chatbox *chatbox::getInstance()
{
    if (_instance == NULL)
    {
        _instance = new chatbox();
    }
    return _instance;
}

chatbox::chatbox(QWidget *parent) : QWidget(parent),
                                    ui(new Ui::chatbox)
{
    ui->setupUi(this);
    _instance = this;
    mythread = new Thread();
    connect(mythread, SIGNAL(getUserMsg(QString,QString,int)),
            this, SLOT(printMsg(QString,QString,int)));
    connect(mythread, SIGNAL(notify(QString, int)),
            this, SLOT(sendNotify(QString, int)));
    mythread->start();
}

chatbox::~chatbox()
{
    delete ui;
}

void chatbox::on_sendButton_clicked()
{
    QString msg = ui->lineEdit->text();
    
    if(msg.toStdString().empty() == true){
        return;
    }

    if(ui->tabWidget->currentIndex()==0){
        sendmsg(msg, QString::fromStdString(clientClass::getInstance()->Username), clientClass::getInstance()->color);
        sendmsgtoserver(msg.toStdString(), tagbox.send, 1);
    }
    else
    {
        sendmsg(msg, QString::fromStdString(clientClass::getInstance()->Username), clientClass::getInstance()->color);
        sendmsgtoserver(msg.toStdString(), tagbox.groupsend, 0);
    }
}

void chatbox::sendmsg(QString msgg, QString name, int color){
    
    ui->textEdit->setTextColor(Qt::black);
    if (clientClass::getInstance()->Username.compare(name.toStdString()) == 0)
    {
        ui->textEdit->setAlignment(Qt::AlignRight);
    }
    else
        ui->textEdit->setAlignment(Qt::AlignLeft);

    ui->textEdit->setFontWeight(QFont::Bold);
    ui->textEdit->insertPlainText(name + "\n");
    ui->textEdit->setFontWeight(QFont::Thin);
    switch (color)
    {
    case 0:
        ui->textEdit->setTextColor(Qt::black);
        break;
    case 1:
        ui->textEdit->setTextColor(Qt::red);
        break;
    case 2:
        ui->textEdit->setTextColor(Qt::green);
        break;
    case 3:
        ui->textEdit->setTextColor(Qt::blue);
        break;
    }
    ui->textEdit->insertPlainText(msgg + "\n" + "\n");
    ui->lineEdit->clear();
    ui->textEdit->verticalScrollBar()->setValue(ui->textEdit->verticalScrollBar()->maximum());
}

void chatbox::on_listWidget_itemClicked(QListWidgetItem *item)
{
}

void chatbox::on_listWidget_2_itemPressed(QListWidgetItem *item)
{
    ui->listWidget_2->currentItem()->setTextColor(Qt::black);
    JoinRoomResponse res;
    RoomMemberResponse resmem;
    for (auto item : listRoom)
    {
        int temp = 0;
        if (ui->listWidget_2->currentItem()->text().toStdString() == item.Name)
        {
            RoomMemberRequest reqmem(item.IDs, clientClass::getInstance()->AuthKey);
            resmem = RoomAPI::getRoomMember(reqmem);
            clientClass::getInstance()->GroupID = item.IDs;
            for(int i=0;i<resmem.Username.size();i++){
                if(resmem.Username.at(i) == clientClass::getInstance()->Username){
                    temp = 1;
                    break;
                }
            }
            if (temp == 1)
                break;
            if(item.TypeOfRoom == 1){
                QString pass = QInputDialog::getText(this, "This is a private group", "Password",QLineEdit::Password);
                JoinRoomRequest req(item.IDs, pass.toStdString(), clientClass::getInstance()->Username, clientClass::getInstance()->AuthKey);
                res = RoomAPI::joinRoom(req);
            }
            else
            {
                JoinRoomRequest req(item.IDs, "", clientClass::getInstance()->Username, clientClass::getInstance()->AuthKey);
                res = RoomAPI::joinRoom(req);
            }
            clientClass::getInstance()->GroupID = item.IDs;
        }
    }
    qDebug() << clientClass::getInstance()->GroupID;
    printChatHistory(0);
    currentitem = ui->listWidget_2->currentItem()->text();
}

void chatbox::on_listWidget_itemPressed(QListWidgetItem *item)
{
    ui->listWidget->currentItem()->setTextColor(Qt::black);
    currentitem = ui->listWidget->currentItem()->text();
    string msg = tagbox.reqsRoom + currentitem.toStdString();
    //clientClass::getInstance()->senderName = user.at(localChoose - 1);
    //oldID = clientClass::getInstance()->GroupID;
    send(clientClass::getInstance()->Sock, (void *)msg.c_str(),1024, 0);
    //sleep(1);
    printChatHistory(1);
}

void chatbox::on_settingButton_clicked()
{
    ui->stackedWidget->currentWidget()->hide();
    ui->stackedWidget->setCurrentWidget(ui->stackedWidget->widget(1));
    ui->stackedWidget->currentWidget()->show();
    getuser();
    getGroup();
}

void chatbox::on_SettingbackButton_clicked()
{
    getuser();
    getGroup();
    init();
    ui->stackedWidget->currentWidget()->hide();
    ui->stackedWidget->setCurrentWidget(ui->stackedWidget->widget(0));
    ui->stackedWidget->currentWidget()->show();
    ui->listWidget->clear();
    ActiveAccountsRequest req(clientClass::getInstance()->AuthKey);
    activeName = AccountsAPI::getActiveAccounts(req).Usernames;
    char cwd[PATH_MAX];
    string fileName;
    fileName = clientClass::getInstance()->getExecutionPath() +"/resources/active-icon-24";
    fileName = fileName + ".png";

    int index = 0;
    for (int i = 0; i < name.size(); i++)
    {
        if (name.at(i).compare(clientClass::getInstance()->Username) == 0)
        {
            continue;
        }
        QListWidgetItem *item = new QListWidgetItem();
        item->setText(QString::fromStdString(name.at(i)));
        if (index != activeName.size())
        {
            if (name.at(i).compare(activeName.at(index)) == 0)
            {
                item->setIcon(QIcon(QString::fromStdString(fileName)));
                index++;
                if (index == activeName.size())
                {
                    index--;
                }
            }
        }
        ui->listWidget->insertItem(i, item);
    }
}

void chatbox::getuser()
{
    AccountsRequest req(clientClass::getInstance()->AuthKey);
    auto accounts = AccountsAPI::getAllAccounts(req).Accounts;
    name.clear();
    for (auto item : accounts)
        name.push_back(item.Username);
    for (auto i = name.begin(); i != name.end(); ++i)
        if (*i == clientClass::getInstance()->Username)
        {
            //cout << *i << endl;
            name.erase(i);
            break;
        }
}

void chatbox::getGroup()
{
    listRoom = roomClass::getListRoom();
}

void chatbox::sendmsgtoserver(string str, string tag, int grtype){
    int id;
    bzero((void *)clientClass::getInstance()->msg.c_str(), clientClass::getInstance()->msg.length());
    clientClass::getInstance()->msg = tag + to_string(clientClass::getInstance()->GroupID) + "%" + to_string(clientClass::getInstance()->color) + "%" + str;
    clientClass::getInstance()->chatHistory(str, clientClass::getInstance()->Username, clientClass::getInstance()->GroupID, grtype);
    send(clientClass::getInstance()->Sock, (void *)clientClass::getInstance()->msg.c_str(), 1024, 0);
}
void chatbox::on_groupManagementButton_clicked()
{
    getGroup();
    currentGroupUser.clear();
    ui->stackedWidget->currentWidget()->hide();
    ui->stackedWidget->setCurrentWidget(ui->stackedWidget->widget(2));
    ui->stackedWidget->currentWidget()->show();
    ui->groupWidget->clear();
    ui->groupMemberWidget->clear();
    ui->allUserWidget->clear();
    ui->groupWidget->setIconSize(QSize(10, 10));
    char cwd[PATH_MAX];
    getcwd(cwd, sizeof(cwd));
    string fileName = cwd;
    fileName = clientClass::getInstance()->getExecutionPath() +"/resources/active-icon-24";
    fileName = fileName + ".png";
    for (int i = 0; i < listRoom.size(); i++)
    {
        QListWidgetItem *item = new QListWidgetItem();
        item->setText(QString::fromStdString(listRoom.at(i).Name));
        if (listRoom.at(i).Owner == clientClass::getInstance()->Username)
            item->setIcon(QIcon(QString::fromStdString(fileName)));

        ui->groupWidget->insertItem(i, item);
    }
    for (int i = 0; i < name.size(); i++)
    {
        QListWidgetItem *item = new QListWidgetItem();
        item->setText(QString::fromStdString(name.at(i)));
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
        ui->allUserWidget->insertItem(i, item);
    }
}

void chatbox::on_accountManagementButton_clicked()
{
    ui->stackedWidget->currentWidget()->hide();
    ui->stackedWidget->setCurrentWidget(ui->stackedWidget->widget(3));
    ui->stackedWidget->currentWidget()->show();
}

void chatbox::on_groupManagementBackButton_clicked()
{
    ui->stackedWidget->currentWidget()->hide();
    ui->stackedWidget->setCurrentWidget(ui->stackedWidget->widget(1));
    ui->stackedWidget->currentWidget()->show();
}

void chatbox::on_accountManagementBackButton_clicked()
{
    ui->stackedWidget->currentWidget()->hide();
    ui->stackedWidget->setCurrentWidget(ui->stackedWidget->widget(1));
    ui->stackedWidget->currentWidget()->show();
}

void chatbox::on_textColorButton_clicked()
{
    ui->stackedWidget->currentWidget()->hide();
    ui->stackedWidget->setCurrentWidget(ui->stackedWidget->widget(4));
    ui->stackedWidget->currentWidget()->show();
}

void chatbox::on_textColorBackButton_clicked()
{
    ui->stackedWidget->currentWidget()->hide();
    ui->stackedWidget->setCurrentWidget(ui->stackedWidget->widget(1));
    ui->stackedWidget->currentWidget()->show();
}

void chatbox::on_LogoutButton_clicked()
{
    clientClass::getInstance()->Logout();
    clientClass::getInstance()->resetClient();
    close();
}

void chatbox::on_redButton_clicked()
{
    clientClass::getInstance()->color = 1;
}

void chatbox::on_blackButton_clicked()
{
    clientClass::getInstance()->color = 0;
}

void chatbox::on_blueButton_clicked()
{
    clientClass::getInstance()->color = 3;
}

void chatbox::on_greenButton_clicked()
{
    clientClass::getInstance()->color = 2;
}

void chatbox::on_accountManagementAcceptButton_clicked()
{
    if(ui->newPass->text() != ui->reNewPass->text()){
        ui->accountManagementLabel->setText("Password not mathch");
    }
    else
    {
        UpdateAccountRequest req(clientClass::getInstance()->AuthKey);
        req.AuthKey = clientClass::getInstance()->AuthKey;
        req.Username = clientClass::getInstance()->Username;
        req.Role = "0";
        req.Password = ui->reNewPass->text().toStdString();
        RESULT_CODE a = AccountsAPI::postUpdateAccount(req);
    }
}

void chatbox::on_leaveGroupButton_clicked()
{
     if(ui->groupWidget->currentItem() == 0x0){
        QMessageBox::about(this,"Error","Please choose a group");
        return;
    }
    LeaveRoomResponse res;
    QString grname = ui->groupWidget->currentItem()->text();
    int index = 0;
    for (auto item : listRoom)
    {
        if (item.Name == grname.toStdString())
        {
            LeaveRoomRequest req(item.IDs, clientClass::getInstance()->Username, clientClass::getInstance()->AuthKey);
            res = RoomAPI::leaveRoom(req);
        }
    }
    qDebug() << QString::fromStdString(res.Status);
    getGroup();
    if(res.Status == "Success"){
        on_groupWidget_itemPressed(ui->groupWidget->currentItem());
        on_groupManagementButton_clicked();
    }
}

void chatbox::on_groupWidget_itemPressed(QListWidgetItem *item)
{
    ui->groupMemberWidget->clear();
    RoomMemberResponse res;
    QString grname = ui->groupWidget->currentItem()->text();
    int index = 0;
    for (auto item : listRoom)
    {
        if (item.Name == grname.toStdString())
        {
            RoomMemberRequest req(item.IDs, clientClass::getInstance()->AuthKey);
            res = RoomAPI::getRoomMember(req);
            if (item.TypeOfRoom == 1)
            {
                ui->oldPassLineEdit->show();
                ui->newPassLineEdit->show();
                ui->reNewPassLineEdit->show();
            }
            else
            {
                ui->oldPassLineEdit->hide();
                ui->newPassLineEdit->hide();
                ui->reNewPassLineEdit->hide();
            }
        }
    }
    currentGroupUser.clear();
    for (int i = 0; i < res.Username.size(); i++)
    {
        ui->groupMemberWidget->insertItem(i, QString::fromStdString(res.Username.at(i)));
        currentGroupUser.push_back(res.Username.at(i));
    }
}

void chatbox::on_accessButton_clicked()
{
    if (ui->accessButton->isChecked() == true)
    {
        ui->groupPassEdit->show();
    }
    else
    {
        ui->groupPassEdit->hide();
    }
}

void chatbox::on_inviteGroupButton_clicked()
{
    if(ui->groupWidget->currentItem() == 0x0){
        QMessageBox::about(this,"Error","Please choose a group");
        return;
    }
    vector<string> inviteName;
    InviteRoomResponse res;
    QString grname = ui->groupWidget->currentItem()->text();
    for (int i = 0; i < name.size(); i++)
    {
        if (ui->allUserWidget->item(i)->checkState() == Qt::Checked)
        {
            inviteName.push_back(name.at(i));
        }
    }
    for (auto item : listRoom)
    {
        if (item.Name == grname.toStdString())
        {
            if (item.Owner == clientClass::getInstance()->Username)
            {
                InviteRoomRequest req(item.IDs, inviteName, clientClass::getInstance()->AuthKey);
                res = RoomAPI::inviteRoom(req);
            }
            else{
                QMessageBox::about(this, "Error", "You are not the owner of this group");
            }
        }
    }
    if(res.Status == "Success"){
        on_groupWidget_itemPressed(ui->groupWidget->currentItem());
    }
}

void chatbox::on_removeMemberButton_clicked()
{
    if(ui->groupWidget->currentItem() == 0x0){
        QMessageBox::about(this,"Error","Please choose a group");
        return;
    }
     if(ui->groupMemberWidget->currentItem() == 0x0){
        QMessageBox::about(this,"Error","Please choose a group member");
        return;
    }
    LeaveRoomResponse res;
    QString grname = ui->groupWidget->currentItem()->text();
    QString urName = ui->groupMemberWidget->currentItem()->text();
    int index = 0;
    for (auto item : listRoom)
    {
        if (item.Name == grname.toStdString())
        {
            if (item.Owner == clientClass::getInstance()->Username)
            {
                LeaveRoomRequest req(item.IDs, urName.toStdString(), clientClass::getInstance()->AuthKey);
                res = RoomAPI::leaveRoom(req);
            }
            else{
                QMessageBox::about(this, "Error", "You are not the owner of this group");
            }
        }
    }
    qDebug() << QString::fromStdString(res.Status);
    if(res.Status == "Success"){
        on_groupWidget_itemPressed(ui->groupWidget->currentItem());

    }
}

void chatbox::printMsg(QString name, QString msg, int color)
{
    ui->textEdit->setTextColor(Qt::black);
    if (clientClass::getInstance()->Username.compare(name.toStdString()) == 0)
    {
        ui->textEdit->setAlignment(Qt::AlignRight);
    }
    else
        ui->textEdit->setAlignment(Qt::AlignLeft);

    ui->textEdit->setFontWeight(QFont::Bold);
    qDebug() << name;
    ui->textEdit->insertPlainText(name + "\n");
    ui->textEdit->setFontWeight(QFont::Thin);
    switch (color)
    {
    case 0:
        ui->textEdit->setTextColor(Qt::black);
        break;
    case 1:
        ui->textEdit->setTextColor(Qt::red);
        break;
    case 2:
        ui->textEdit->setTextColor(Qt::green);
        break;
    case 3:
        ui->textEdit->setTextColor(Qt::blue);
        break;
    }
    ui->textEdit->insertPlainText(msg + "\n" + "\n");
    ui->textEdit->verticalScrollBar()->setValue(ui->textEdit->verticalScrollBar()->maximum());
}

void chatbox::on_createGroupButton_clicked()
{
    CreateRoomResponse res;
    QString grname = ui->groupNameEdit->text();
    if (ui->accessButton->isChecked())
    {
        QString pass = ui->groupPassEdit->text();
        CreateRoomRequest req(grname.toStdString(), 1, clientClass::getInstance()->Username, pass.toStdString(), clientClass::getInstance()->AuthKey);
        res = RoomAPI::createRoom(req);
    }
    else
    {
        CreateRoomRequest req(grname.toStdString(), 0, clientClass::getInstance()->Username, "", clientClass::getInstance()->AuthKey);
        res = RoomAPI::createRoom(req);
    }
    getGroup();
    qDebug() << QString::fromStdString(res.Status);
    if(res.Status == "success"){
        on_groupManagementButton_clicked();
    }
}

void chatbox::on_deleteGroupButton_clicked()
{
    if(ui->groupWidget->currentItem() == 0x0){
        QMessageBox::about(this,"Error","Please choose a group");
        return;
    }
    getGroup();
    DeleteRoomResponse res;
    for (auto item : listRoom)
    {
        if (item.Name == ui->groupWidget->currentItem()->text().toStdString())
        {
            QMessageBox msgBox;
            msgBox.setText("You are about to delete a group");
            msgBox.setInformativeText("this change can not be undo");
            msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
            msgBox.setDefaultButton(QMessageBox::Ok);
            int ret = msgBox.exec();
            if (ret == QMessageBox::Ok)
            {
                DeleteRoomRequest req(item.IDs, clientClass::getInstance()->Username, ui->lineEdit_2->text().toStdString(), clientClass::getInstance()->AuthKey);
                res = RoomAPI::deleteRoom(req);
                break;
            }
            else
            {
                break;
            }
        }
    } 
    switch(res.Result){
        case RESULT_SUCCESS:
            qDebug() << "Success";
            on_groupManagementButton_clicked();
            break;
        case RESULT_CODE::GROUP_OWNER_OR_PASSWORD_INVALID:
            QMessageBox::about(this, "Error", "Your are not the owner or wrong password");
            break;
    }
}

void chatbox::on_modifyGroupButton_clicked()
{
    if(ui->groupWidget->currentItem() == 0x0){
        QMessageBox::about(this,"Error","Please choose a group");
        return;
    }
    ModifyRoomResponse res;
    getGroup();
    for (auto item : listRoom)
    {
        if (item.Name == ui->groupWidget->currentItem()->text().toStdString())
        {
            if (item.TypeOfRoom == 1)
            {
                if (ui->newPassLineEdit->text() != ui->reNewPassLineEdit->text())
                {
                    QMessageBox::about(this, "error", "Password miss match");
                    break;
                }
                ModifyRoomRequest req(item.IDs, clientClass::getInstance()->Username, ui->newNameLineEdit->text().toStdString(), ui->newPassLineEdit->text().toStdString(), ui->oldPassLineEdit->text().toStdString(), clientClass::getInstance()->AuthKey);
                res = RoomAPI::modifyRoom(req);
                if(res.Result == 38){
                    QMessageBox::about(this,"Error","You are not group owner or wrong password");
                    return;
                }
            }
            else
            {
                ModifyRoomRequest req(item.IDs, clientClass::getInstance()->Username, ui->newNameLineEdit->text().toStdString(), "", "", clientClass::getInstance()->AuthKey);
                res = RoomAPI::modifyRoom(req);
                if(res.Result == 38){
                    QMessageBox::about(this,"Error","You are not group owner");
                    return;
                }
            }
        }
    }
    if(res.Status == "Success"){
        on_groupManagementButton_clicked();
    }  
}

void chatbox::sendNotify(QString Name, int grtype){
    if(grtype == 1){
        for(int i=0 ; i< name.size() ; i++){
            QListWidgetItem* item = ui->listWidget->item(i);
            if(item->text() == Name){
                item->setTextColor(Qt::red);
            }  
        }
    }
    else{
        for(auto item: listRoom){
            if(item.IDs == Name.toInt()){
                Name = QString::fromStdString(item.Name);
                qDebug() << "Name" ;
            }
        }
        for(int i=0 ; i< listRoom.size() ; i++){
            QListWidgetItem* item = ui->listWidget_2->item(i);
                if(item->text() == Name){
                    item->setTextColor(Qt::red);
                }
        }
    }
}