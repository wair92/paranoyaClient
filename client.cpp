#include <QDebug>
#include <QQuickView>
#include <QQuickItem>
#include <QJsonObject>
#include <QJsonDocument>
#include "client.h"

Client::Client()
{
    connect( &client_, &QTcpSocket::connected, this, &Client::connected);
    connect( &client_, &QTcpSocket::disconnected, this, &Client::disconnected);
}

void Client::connectToServer()
{
    qDebug() << "Connecting to server on adderss" << address_ << "and port: " << port_;
    client_.connectToHost(address_, port_);
}

void Client::disconnectToServer()
{
    qDebug() << "Disconnecting ...";
    client_.disconnectFromHost();
}

void Client::setClient(QHostAddress address, quint16 port)
{
    address_ = address;
    port_ = port;
}

void Client::sendMessage()
{
    qDebug() << "Sending message: " << message_;

    QJsonObject messageObject;
    messageObject.insert("Sender", QJsonValue::fromVariant("User1"));
    messageObject.insert("Receiver", QJsonValue::fromVariant("User2"));
    messageObject.insert("Message", QJsonValue::fromVariant( message_ ));

    QJsonDocument doc( messageObject );
    auto dataToSend = doc.toJson(QJsonDocument::Compact);

    //const char* data = dataToSend.toLatin1();
    client_.write( dataToSend, dataToSend.length());
}

QString Client::getMessage() const
{
    return message_;
}

void Client::setMessage(const QString &message)
{
    message_ = message;
}
