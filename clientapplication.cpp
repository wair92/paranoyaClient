#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlProperty>
#include <QDebug>
#include <QQmlComponent>
#include <QQmlContext>

#include "clientapplication.h"
#include "message.h"

ClientApplication::ClientApplication(int argc, char *argv[])
    :app_(argc, argv ),engine_(), client_()
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    engine_.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));

    auto *connectToServer = engine_.rootObjects()[0]->findChild<QObject *>("connectToServer");
    connect(connectToServer, SIGNAL(connectionClicked(QString)), &client_, SLOT(connectToServer(QString)));

    //emit signal after confirmed connection
    engine_.rootContext()->setContextProperty("helper", &helper_);
    connect(&client_, &Client::connectionConfirmed, this, [this](){
        emit helper_.connectionConfirmed();
        auto *disconnectToServer = engine_.rootObjects()[0]->findChild<QObject *>("disconnectToServer");
        auto *sendMessage = engine_.rootObjects()[0]->findChild<QObject *>("sendText");
        auto *messageInput = engine_.rootObjects()[0]->findChild<QObject *>("messageInput");


        connect(disconnectToServer, SIGNAL(disconnectionClicked()), &client_, SLOT(disconnectToServer()));
        connect(sendMessage, SIGNAL(sendMessageClicked(QString)), &client_, SLOT(sendMessage(QString)));
        connect(messageInput, SIGNAL(messageChangedd(QString)), &client_, SLOT(setMessage(QString)));

        auto *history = engine_.rootObjects()[0]->findChild<QObject *>("history");
        auto *connectToServer = engine_.rootObjects()[0]->findChild<QObject *>("connectToServer");
        disconnect(connectToServer, SIGNAL(connectionClicked(QString)), &client_, SLOT(connectToServer(QString)) );

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


    QHostAddress ip;
    ip.setAddress("127.0.0.1");
    client_.setClient(ip,9000);
}

int ClientApplication::run()
{
    return app_.exec();
}

void ClientApplication::reconnect()
{
    qDebug() << "Reconnect";
    auto *connectToServer = engine_.rootObjects()[0]->findChild<QObject *>("connectToServer");
    connect(connectToServer, SIGNAL(connectionClicked(QString)), &client_, SLOT(connectToServer(QString)));
}


