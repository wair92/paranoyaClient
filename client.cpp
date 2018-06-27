#include <QDebug>
#include <QQuickView>
#include <QQuickItem>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QVector>
#include "client.h"
#include "message.h"

Client::Client()
    :receivedMesssage_("","",""),
      config_("config.json")
{
    QHostAddress ip;
    ip.setAddress(config_.getIp());
    qDebug() << "XXX" << config_.getPort();
    setClient(ip, config_.getPort());

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
}

void Client::connectToServer(QString username)
{
    username_ = username;
    qDebug() << "Connecting to server on adderss" << address_ << "and port: " << port_;
    client_.connectToHost(address_, port_);
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

void Client::askForUserList()
{
    qDebug() << username_ << "is asking for user list: ";

    QJsonObject message;
    message.insert("Id", QJsonValue::fromVariant("UserList"));
    message.insert("Username", QJsonValue::fromVariant(username_));
    QJsonDocument doc( message );
    auto dataToSend = doc.toJson(QJsonDocument::Compact);
    client_.write( dataToSend, dataToSend.length());
}

void Client::setReceiver(QString receiver)
{
    receiver_ = receiver;
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
    if(isUserList(object)){
        processUserList(object);
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
    auto heartBeat = object.value(QString("HeartBeatTimer")).toInt();
    qDebug() << username << "sucessfully logged in" << "heartbeatTimer set to:" << heartBeat;
    if(heartBeat != 0){
        heartbeat_.setInterval(heartBeat);
        connect(&heartbeat_, &QTimer::timeout, this, &Client::sendHeartBeat);
        heartbeat_.start();
    }
    //askForUserList();
    emit connectionConfirmed();
}

void Client::processUserList(const QJsonObject &object)
{
    qDebug() << "UserList: ";
    QJsonArray message = object.value(QString("Users")).toArray();
    QVector<QString> users;
    for( const auto& i : message){
        qDebug() << "User: " << i;
        users.push_back(i.toString());
    }

    emit userListProcessed(users);

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

bool Client::isUserList(const QJsonObject &obj) const
{
    auto message = obj.value(QString("Id"));
    if(message.toString() == "UserListResponse")
        return true;
    else
        return false;
}


