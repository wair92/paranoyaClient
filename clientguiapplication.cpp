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
    :app_(argc, argv ),engine_(), client_(), messageHistoryLoader_("message_history.json"), helper_(this)
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    engine_.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    messageHistoryLoader_.loadMessages();
    engine_.rootContext()->setContextProperty("helper", &helper_);

    QStringList dataList;
    engine_.rootContext()->setContextProperty("myModel", QVariant::fromValue(dataList));
    engine_.rootContext()->setContextProperty("myMessages", QVariant::fromValue(dataList));
    emit helper_.connectionConfirmed();

    connectToServer_ = engine_.rootObjects()[0]->findChild<QObject *>("connectToServer");
    connect(connectToServer_, SIGNAL(connectionClicked(QString)), &client_, SLOT(connectToServer(QString)));

    connect(&client_, &Client::connectionConfirmed, this, [this](){

        history_ = engine_.rootObjects()[0]->findChild<QObject *>("history");
        connectToServer_ = engine_.rootObjects()[0]->findChild<QObject *>("connectToServer");
        disconnect(connectToServer_, SIGNAL(connectionClicked(QString)), &client_, SLOT(connectToServer(QString)) );

        connect(&client_, &Client::userListProcessed, this, [this]( QVector<QString> users ){
            QStringList dataList;
            for(const QString& i: users){
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

void ClientGUIApplication::chatOpenedd(QString receiver)
{
    qDebug() << "Chat Openedd";
    engine_.rootContext()->setContextProperty("Receiver", QVariant::fromValue(receiver));

    connect(&client_, &Client::messageReceived, this, [this](){
        qDebug() << "Message received";
        QString text = QQmlProperty::read(history_, "historyText").toString();
        text = text.append(client_.getReceivedMessage().getSender());
        text = text.append(":");
        text = text.append(client_.getReceivedMessage().getMessage());
        text = text.append("\n");
        QQmlProperty::write(history_, "historyText", text);
    });
}

void ClientGUIApplication::sendMessage(QString message)
{
    client_.sendMessage(message);
}

void ClientGUIApplication::setMessage(QString message)
{
    client_.setMessage(message);
}

Helper::Helper(ClientGUIApplication* mainApp):
    mainApp_( mainApp)
{

}

void Helper::onChatOpened(QString receiver){
    mainApp_->chatOpenedd(receiver);
}

void Helper::sendMessage(QString message)
{
    mainApp_->sendMessage(message);
}

void Helper::setMessage(QString message)
{
    mainApp_->setMessage(message);
}
