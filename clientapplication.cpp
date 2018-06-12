#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlProperty>
#include <QDebug>

#include "clientapplication.h"
#include "message.h"

ClientApplication::ClientApplication(int argc, char *argv[])
    :app_(argc, argv ),engine_(), client_()
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    engine_.load(QUrl(QStringLiteral("qrc:/main.qml")));
    auto *connectToServer = engine_.rootObjects()[0]->findChild<QObject *>("connectToServer");
    auto *disconnectToServer = engine_.rootObjects()[0]->findChild<QObject *>("disconnectToServer");
    auto *indicator = engine_.rootObjects()[0]->findChild<QObject *>("indicator");
    auto *sendMessage = engine_.rootObjects()[0]->findChild<QObject *>("sendText");
    auto *messageInput = engine_.rootObjects()[0]->findChild<QObject *>("messageInput");
    auto *afterLoginContainer = engine_.rootObjects()[0]->findChild<QObject *>("afterLoginContainer");
    auto *history = engine_.rootObjects()[0]->findChild<QObject *>("history");

    connect(&client_, &Client::connected, this, [indicator, afterLoginContainer](){
        QQmlProperty::write(indicator, "indicatorColor", "green");
        QQmlProperty::write(afterLoginContainer, "logged", true);
    });

    connect(&client_, &Client::disconnected, this, [indicator, afterLoginContainer](){
        QQmlProperty::write(indicator, "indicatorColor", "gray");
        QQmlProperty::write(afterLoginContainer, "logged", false);
    });

    connect(&client_, &Client::messageReceived, this, [this, history](){
        qDebug() << "Message received";
        QString text = QQmlProperty::read(history, "historyText").toString();
        text = text.append(client_.getReceivedMessage().getSender());
        text = text.append(":");
        text = text.append(client_.getReceivedMessage().getMessage());
        text = text.append("\n");
        qDebug() << "Final Text: " << text;
        QQmlProperty::write(history, "historyText", text);
    });


    connect(connectToServer, SIGNAL(connectionClicked(QString)), &client_, SLOT(connectToServer(QString)));
    connect(disconnectToServer, SIGNAL(disconnectionClicked()), &client_, SLOT(disconnectToServer()));
    connect(disconnectToServer, SIGNAL(disconnectionClicked()), &client_, SLOT(disconnectToServer()));
    connect(sendMessage, SIGNAL(sendMessageClicked(QString)), &client_, SLOT(sendMessage(QString)));
    connect(messageInput, SIGNAL(messageChangedd(QString)), &client_, SLOT(setMessage(QString)));

    QHostAddress ip;
    ip.setAddress("127.0.0.1");
    client_.setClient(ip,9009);
}

int ClientApplication::run()
{
    return app_.exec();
}
