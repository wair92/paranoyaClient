#ifndef CLIENT_H
#define CLIENT_H

#include <QTcpSocket>
#include <QHostAddress>
#include <QDebug>
#include "message.h"

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

public slots:
    void connectToServer(QString username);
    void disconnectToServer();
    void sendMessage(QString receiver);
    void setMessage(const QString& message);

private:
    void sendLogin();
    void process(QByteArray data);
    void processMessage( QJsonObject object );
    bool isMessage(const QJsonObject& obj) const;
    QTcpSocket client_;
    QHostAddress address_;
    quint16 port_;
    QString message_;
    QString username_;
    Message receivedMesssage_;
};

#endif // CLIENT_H
