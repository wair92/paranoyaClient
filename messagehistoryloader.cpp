#include "messagehistoryloader.h"
#include <QFile>
#include <QVector>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QString>
#include <QDebug>
MessageHistoryLoader::MessageHistoryLoader(QString path)
{
    filePath_ = path;
}

QVector<Message> MessageHistoryLoader::loadMessages()
{
    qDebug() << "Loading messages";
    QVector<Message> messages;
    if(parseFile() == false){
        emit messageFileParseError();
    }

    emit messagesLoaded(messages);

    return messages;
}

void MessageHistoryLoader::getUsersWhoWrote()
{
    QFile file;
    QString data;
    file.setFileName(filePath_);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    if(file.isOpen()){
        data = file.readAll();
        file.close();
    }
    else{
        qDebug() << "Unable to open file";
    }
    QJsonDocument messageHistory = QJsonDocument::fromJson(data.toUtf8());
    if(messageHistory.isNull()){
        emit messageFileEmpty();
    }
    QJsonObject object = messageHistory.object();
    auto list = object.keys();
    qDebug() << "emmiting userlist";
    emit usersWhoWrote( list );
}

bool MessageHistoryLoader::parseFile()
{
    QFile file;
    QString data;
    file.setFileName(filePath_);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    if(file.isOpen()){
        data = file.readAll();
        file.close();
    }
    else{
        qDebug() << "Unable to open file";
    }
    QJsonDocument messageHistory = QJsonDocument::fromJson(data.toUtf8());
    if(messageHistory.isNull()){
        emit messageFileEmpty();
    }
    QJsonObject object = messageHistory.object();
    qDebug() << object.keys();

}
