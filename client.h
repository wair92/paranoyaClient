#ifndef CLIENT_H
#define CLIENT_H

#include <QTcpSocket>
#include <QHostAddress>
#include <QDebug>
#include <QTimer>
#include "message.h"
#include "configloader.h"

class Client: public QObject
{
    Q_OBJECT
public:
    Client();
    virtual ~Client() = default;
    void setClient(QHostAddress address, quint16 port);
    QString getMessage() const;
    Message getReceivedMessage() const {return receivedMesssage_;}

signals:
    void connected();
    void disconnected();
    void messageChangedd(QString message);
    void messageReceived();
    void connectionConfirmed();
    void userListProcessed(QVector<QString> users);

public slots:
    void connectToServer(QString username);
    void disconnectToServer();
    void sendMessage(QString receiver);
    void setMessage(QString message);
    void askForUserList();
    void setReceiver(QString receiver);

private:
    void sendLogin();
    void sendLogout();
    void sendMessageReceivedConfirmation();
    void sendHeartBeat();


    void process(QByteArray data);
    void processMessage( const QJsonObject& object );
    void processLoginConfirm( const QJsonObject& object );
    void processUserList(const QJsonObject& object);

    bool isMessage(const QJsonObject& obj) const;
    bool isLoginConfirm(const QJsonObject& obj) const;
    bool isUserList(const QJsonObject& obj) const;

private:
    ConfigLoader config_;
    QTcpSocket client_;
    QHostAddress address_;
    quint16 port_;
    QString message_;
    QString username_;
    Message receivedMesssage_;
    QTimer heartbeat_;
    QString receiver_;
};

#endif // CLIENT_H
