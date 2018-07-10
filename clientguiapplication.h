#ifndef CLIENTAPPLICATION_H
#define CLIENTAPPLICATION_H

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "client.h"
#include "messagehistoryloader.h"

class ClientGUIApplication;

class Helper: public QObject{
    Q_OBJECT
public:
    Helper(ClientGUIApplication* mainApp);
    virtual ~Helper() = default;
    using QObject::QObject;
signals:
   void connectionConfirmed();
public slots:
   void onChatOpened();
   void sendMessage(QString message);
   void setMessage(QString message);
private:
   ClientGUIApplication* mainApp_;
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
    void sendMessage(QString message);
    void setMessage(QString message);
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
