#ifndef THREAD_H
#define THREAD_H

#include <QObject>
#include <QWidget>
#include <QThread>
#include <string>
#include <QString>
#include "clientClass.h"

using namespace std;

class Thread : public QThread
{
    Q_OBJECT
public:
    void run();
    Thread();
signals:
    void getUserMsg(QString, QString, int);
    void notify(QString, int);
};

#endif // THREAD_H
