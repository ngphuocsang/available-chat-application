#include "mainwindow.h"
#include <iostream>
#include <string>
#include <string.h>
#include <QWidget>
#include <QPixmap>
#include "./ui_mainwindow.h"
#include <QString>
#include "clientClass.h"
#include "AccountsAPI.h"
#include "MyLibrary.h"
#include "chatbox.h"

using namespace std;
using namespace MyLibrary;
using namespace Client::RestAPI;
using namespace MyLibrary::Models::Requests;
using namespace MyLibrary::Models::Responses;
using namespace Client::RestAPI;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    char cwd[PATH_MAX];
    getcwd(cwd, sizeof(cwd));
    string fileName = cwd;
    fileName = clientClass::getInstance()->getExecutionPath() +"/resources/logo2";
    fileName = fileName + ".png";
    QPixmap pix(QString::fromStdString(fileName)); 
    ui->label_4->setPixmap(pix.scaled(300,300,Qt::KeepAspectRatio));
    //ui->label_4->setPixmap(pix);
    this->setWindowTitle("Login Screen");
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Login_clicked()
{
    
    clientClass::getInstance()->Username = ui->lineEdit_2->text().toStdString();
    QString password = ui->lineEdit->text();
    AuthenAccountRequest req(clientClass::getInstance()->Username, password.toStdString());
    AuthenAccountResponse res = AccountsAPI::postAuthenAccount(req);
    clientClass::getInstance()->AuthKey = res.AuthKey;
    clientClass::getInstance()->Role = res.Role;
    int result = RESULT_CODE::RESULT_UNKNOWN;
    switch (res.Result)
        {
        case RESULT_CODE::RESULT_SUCCESS:{
            if (clientClass::getInstance()->initialize() != RESULT_SUCCESS)
            {
                cout << "Initializing socket failed.\nGo back now." << endl;
                break;
            }
            if (clientClass::getInstance()->connectToServer() != RESULT_SUCCESS)
            {
                cout << "Go back now." << endl;
            }
            ui->label_3->clear();
            ui->lineEdit->clear();
            ui->lineEdit_2->clear();
            close();
            break;
        }
        case RESULT_CODE::USERNAME_NOT_FOUND:
            ui->label_3->setText(QString::fromStdString("Username not found!"));
            break;
        case RESULT_CODE::PASSWORD_INVALID:
            ui->lineEdit->clear();
            ui->label_3->setText(QString::fromStdString("Incorrect password!"));
            break;
        default:
            break;
        }

}

void MainWindow::on_Exit_clicked()
{
    clientClass::getInstance()->Role = "-1";
    close();
}
