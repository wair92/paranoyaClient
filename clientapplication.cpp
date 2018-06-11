#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlProperty>

#include "clientapplication.h"

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

    connect(&client_, &Client::connected, this, [indicator](){
        QQmlProperty::write(indicator, "indicatorColor", "green");
    });

    connect(&client_, &Client::disconnected, this, [indicator](){
        QQmlProperty::write(indicator, "indicatorColor", "gray");
    });


    connect(connectToServer, SIGNAL(connectionClicked()), &client_, SLOT(connectToServer()));
    connect(disconnectToServer, SIGNAL(disconnectionClicked()), &client_, SLOT(disconnectToServer()));
    connect(disconnectToServer, SIGNAL(disconnectionClicked()), &client_, SLOT(disconnectToServer()));
    connect(sendMessage, SIGNAL(sendMessageClicked()), &client_, SLOT(sendMessage()));
    connect(messageInput, SIGNAL(messageChangedd(QString)), &client_, SLOT(setMessage(QString)));

    QHostAddress ip;
    ip.setAddress("127.0.0.1");
    client_.setClient(ip,9006);
}

int ClientApplication::run()
{
    return app_.exec();
}
