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

    auto *connectToServer = engine_.rootObjects()[0]->findChild<QObject *>("connectToServer");
    connect(connectToServer, SIGNAL(connectionClicked(QString)), &client_, SLOT(connectToServer(QString)));

    engine_.rootContext()->setContextProperty("helper", &helper_);

    connect(&client_, &Client::connectionConfirmed, this, [this](){

        auto *disconnectToServer = engine_.rootObjects()[0]->findChild<QObject *>("disconnectToServer");
        auto *sendMessage = engine_.rootObjects()[0]->findChild<QObject *>("sendText");
        auto *messageInput = engine_.rootObjects()[0]->findChild<QObject *>("messageInput");


        /*connect(disconnectToServer, SIGNAL(disconnectionClicked()), &client_, SLOT(disconnectToServer()));
        connect(sendMessage, SIGNAL(sendMessageClicked(QString)), &client_, SLOT(sendMessage(QString)));
        connect(messageInput, SIGNAL(messageChangedd(QString)), &client_, SLOT(setMessage(QString)));*/

        auto *history = engine_.rootObjects()[0]->findChild<QObject *>("history");
        auto *connectToServer = engine_.rootObjects()[0]->findChild<QObject *>("connectToServer");
        disconnect(connectToServer, SIGNAL(connectionClicked(QString)), &client_, SLOT(connectToServer(QString)) );



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
        });

        messageHistoryLoader_.getUsersWhoWrote();






        auto *setReceiver = engine_.rootObjects()[0]->findChild<QObject *>("setReceiver");
        connect(setReceiver, SIGNAL(setReceiver(QString)), &client_, SLOT(connectToServer(QString)));

        connect(&client_, &Client::messageReceived, this, [this, history](){
            qDebug() << "Message received";
            QString text = QQmlProperty::read(history, "historyText").toString();
            text = text.append(client_.getReceivedMessage().getSender());
            text = text.append(":");
            text = text.append(client_.getReceivedMessage().getMessage());
            text = text.append("\n");
            QQmlProperty::write(history, "historyText", text);
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
    auto *connectToServer = engine_.rootObjects()[0]->findChild<QObject *>("connectToServer");
    connect(connectToServer, SIGNAL(connectionClicked(QString)), &client_, SLOT(connectToServer(QString)));
}


