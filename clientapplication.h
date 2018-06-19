#ifndef CLIENTAPPLICATION_H
#define CLIENTAPPLICATION_H

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "client.h"

class Helper: public QObject{
    //Class for calling qml function
    Q_OBJECT
public:
    Helper() = default;
    virtual ~Helper() = default;
    using QObject::QObject;
signals:
   void connectionConfirmed();
};

class ClientApplication: public QObject
{
    Q_OBJECT
public:
    ClientApplication(int argc, char *argv[]);
    virtual ~ClientApplication() = default;
    int run();
private slots:
    void reconnect();
private:
    QGuiApplication app_;
    QQmlApplicationEngine engine_;
    Client client_;
    Helper helper_;
};

#endif // CLIENTAPPLICATION_H
