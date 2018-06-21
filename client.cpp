#include <QDebug>
#include <QQuickView>
#include <QQuickItem>
#include <QJsonObject>
#include <QJsonDocument>
#include "client.h"
#include "message.h"

Client::Client()
    :receivedMesssage_("","","")
{
    connect( &client_, &QTcpSocket::connected, this,
             [this](){
        sendLogin();
        emit connected();
    });
    connect( &client_, &QTcpSocket::disconnected, this, &Client::disconnected);
    connect( &client_, &QTcpSocket::readyRead, this, [this](){
        QTcpSocket* readSocket = qobject_cast<QTcpSocket*>(sender());
        auto data = readSocket->readAll();
        process(data);
        qDebug() << data;
    } );
    heartbeat_.setInterval(1000);
    connect(&heartbeat_, &QTimer::timeout, this, &Client::sendHeartBeat);

}

void Client::connectToServer(QString username)
{
    username_ = username;
    qDebug() << "Connecting to server on adderss" << address_ << "and port: " << port_;
    client_.connectToHost(address_, port_);
    if( client_.isOpen() ){
        heartbeat_.start();
    }
}

void Client::disconnectToServer()
{
    qDebug() << "Logging out";
    sendLogout();
    qDebug() << "Disconnecting ...";
    heartbeat_.stop();
    client_.disconnectFromHost();
}

void Client::setClient(QHostAddress address, quint16 port)
{
    address_ = address;
    port_ = port;
}

void Client::sendMessage(QString receiver)
{
    qDebug() << "Sending message: " << message_;

    QJsonObject messageObject;
    messageObject.insert("Id", QJsonValue::fromVariant("Message"));
    messageObject.insert("Sender", QJsonValue::fromVariant( username_ ));
    messageObject.insert("Receiver", QJsonValue::fromVariant(receiver));
    messageObject.insert("Message", QJsonValue::fromVariant( message_ ));

    QJsonDocument doc( messageObject );
    auto dataToSend = doc.toJson(QJsonDocument::Compact);
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

void Client::sendLogin()
{
    qDebug() << "Sending login: " << username_;

    QJsonObject loginObject;
    loginObject.insert("Id", QJsonValue::fromVariant("Login"));
    loginObject.insert("Login", QJsonValue::fromVariant(username_));

    QJsonDocument doc( loginObject );
    auto dataToSend = doc.toJson(QJsonDocument::Compact);

    //const char* data = dataToSend.toLatin1();
    client_.write( dataToSend, dataToSend.length());
}

void Client::sendLogout()
{
    qDebug() << "Sending logout: " << username_;

    QJsonObject loginObject;
    loginObject.insert("Id", QJsonValue::fromVariant("Logout"));
    loginObject.insert("Logout", QJsonValue::fromVariant(username_));

    QJsonDocument doc( loginObject );
    auto dataToSend = doc.toJson(QJsonDocument::Compact);

    client_.write( dataToSend, dataToSend.length());
}

void Client::sendHeartBeat()
{
    qDebug() << "Sending Heartbeat: " << username_;

    QJsonObject loginObject;
    loginObject.insert("Id", QJsonValue::fromVariant("HeartBeat"));
    loginObject.insert("HeartBeat", QJsonValue::fromVariant(username_));

    QJsonDocument doc( loginObject );
    auto dataToSend = doc.toJson(QJsonDocument::Compact);

    client_.write( dataToSend, dataToSend.length());
}

void Client::process(QByteArray data)
{
    auto document = QJsonDocument::fromJson(data);
    QJsonObject object = document.object();
    if(isMessage(object)){
        processMessage(object);
    }
    if(isLoginConfirm(object)){
        processLoginConfirm(object);
    }
}

void Client::processMessage(const QJsonObject& object)
{
    auto message = object.value(QString("Message")).toString();
    auto receiver = object.value(QString("Receiver")).toString();
    auto sender = object.value(QString("Sender")).toString();
    qDebug() << "Sender: " << sender << "Receiver: " << receiver << "Message: " << message;
    Message recMesssage(sender, receiver, message);
    receivedMesssage_ = recMesssage;
    emit messageReceived();
}

void Client::processLoginConfirm(const QJsonObject &object)
{
    auto username = object.value(QString("Username")).toString();
    qDebug() << username << "sucessfully logged in";
    emit connectionConfirmed();
}

bool Client::isMessage(const QJsonObject &obj) const
{
    auto message = obj.value(QString("Id"));
    if(message.toString() == "Message")
        return true;
    else
        return false;
}

bool Client::isLoginConfirm(const QJsonObject &obj) const
{
    auto message = obj.value(QString("Id"));
    if(message.toString() == "LoginConfirm")
        return true;
    else
        return false;
}


