#ifndef CLIENTAPPLICATION_H
#define CLIENTAPPLICATION_H

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "client.h"

class ClientApplication: public QObject
{
    Q_OBJECT
public:
    ClientApplication(int argc, char *argv[]);
    virtual ~ClientApplication() = default;
    int run();
private:
    QGuiApplication app_;
    QQmlApplicationEngine engine_;
    Client client_;
};

#endif // CLIENTAPPLICATION_H
