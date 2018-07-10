#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlProperty>
#include <QDebug>
#include <QQmlComponent>
#include <QQmlContext>
#include <QVector>
#include <QStringList>

#include "clientguiapplication.h"
#include "message.h"

ClientGUIApplication::ClientGUIApplication(int argc, char *argv[])
    :app_(argc, argv ),engine_(), client_(), messageHistoryLoader_("message_history.json")
{
    messageHistoryLoader_.loadMessages();
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    engine_.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));

    QStringList dataList;
    engine_.rootContext()->setContextProperty("myModel", QVariant::fromValue(dataList));
    engine_.rootContext()->setContextProperty("myMessages", QVariant::fromValue(dataList));
    emit helper_.connectionConfirmed();

    connectToServer_ = engine_.rootObjects()[0]->findChild<QObject *>("connectToServer");
    connect(connectToServer_, SIGNAL(connectionClicked(QString)), &client_, SLOT(connectToServer(QString)));

    engine_.rootContext()->setContextProperty("helper", &helper_);

    connect(&client_, &Client::connectionConfirmed, this, [this](){

        history_ = engine_.rootObjects()[0]->findChild<QObject *>("history");
        connectToServer_ = engine_.rootObjects()[0]->findChild<QObject *>("connectToServer");
        disconnect(connectToServer_, SIGNAL(connectionClicked(QString)), &client_, SLOT(connectToServer(QString)) );

        connect(&client_, &Client::userListProcessed, this, [this]( QVector<QString> users ){
            QStringList dataList;
            for(const QString i: users){
                dataList.push_back(i);
            }
            engine_.rootContext()->setContextProperty("myModel", QVariant::fromValue(dataList));
            emit helper_.connectionConfirmed();

        });

        client_.askForUserList();

        connect(&messageHistoryLoader_, &MessageHistoryLoader::usersWhoWrote, this, [this]( QStringList users ){
            qDebug() << "Processing message list";
            engine_.rootContext()->setContextProperty("myMessages", QVariant::fromValue(users));
            emit helper_.connectionConfirmed();

            qDebug() << "Connecting to chat ... ";
            message_ = engine_.rootObjects()[0]->findChild<QObject *>("list");
            if(!message_)
                qDebug() << "IT IS NULL";
            auto retVal = connect(message_, SIGNAL(chatOpened()), this, SLOT(chatOpenedd()));
            qDebug() << __FUNCTION__ << "retVal from connect: " << retVal;

            connect(message_, &QObject::destroyed,
                    [] { qDebug() << "Sender got deleted!"; });
            connect(this, &QObject::destroyed,
                    [] { qDebug() << "Receiver got deleted!"; });

        });

        messageHistoryLoader_.getUsersWhoWrote();

        connect(&client_, &Client::messageReceived, this, [this](){
            qDebug() << "Message received";
            QString text = QQmlProperty::read(history_, "historyText").toString();
            text = text.append(client_.getReceivedMessage().getSender());
            text = text.append(":");
            text = text.append(client_.getReceivedMessage().getMessage());
            text = text.append("\n");
            QQmlProperty::write(history_, "historyText", text);
        });

        connect(&client_, &Client::disconnected, this, [this](){
            reconnect();
        });



    });
}

int ClientGUIApplication::run()
{
    return app_.exec();
}

void ClientGUIApplication::reconnect()
{
    qDebug() << "Reconnect";
    connectToServer_ = engine_.rootObjects()[0]->findChild<QObject *>("connectToServer");
    connect(connectToServer_, SIGNAL(connectionClicked(QString)), &client_, SLOT(connectToServer(QString)));
}

void ClientGUIApplication::chatOpenedd()
{
    qDebug() << "Chat Openedd";
    sendMessage_ = engine_.rootObjects()[0]->findChild<QObject *>("sendText");
    messageInput_ = engine_.rootObjects()[0]->findChild<QObject *>("messageInput");

    connect(sendMessage_, SIGNAL(sendMessageClicked(QString)), &client_, SLOT(sendMessage(QString)));
    connect(messageInput_, SIGNAL(messageChangedd(QString)), &client_, SLOT(setMessage(QString)));
}


