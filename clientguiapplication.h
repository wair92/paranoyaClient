#ifndef CLIENTAPPLICATION_H
#define CLIENTAPPLICATION_H

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "client.h"
#include "messagehistoryloader.h"

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

class ClientGUIApplication: public QObject
{
    Q_OBJECT
public:
    ClientGUIApplication(int argc, char *argv[]);
    virtual ~ClientGUIApplication() = default;
    int run();
public slots:
    void reconnect();
    void chatOpenedd();
private:
    QGuiApplication app_;
    QQmlApplicationEngine engine_;
    Client client_;
    Helper helper_;
    MessageHistoryLoader messageHistoryLoader_;

    QTimer myTimer;

    QObject* connectToServer_ {nullptr};
    QObject* message_ {nullptr};
    QObject* history_ {nullptr};
    QObject* sendMessage_ {nullptr};
    QObject* messageInput_ {nullptr};
};

#endif // CLIENTAPPLICATION_H
