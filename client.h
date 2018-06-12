#ifndef CLIENT_H
#define CLIENT_H

#include <QTcpSocket>
#include <QHostAddress>
#include <QDebug>

class Client: public QObject
{
    Q_OBJECT

public:
    Client(QString username);
    virtual ~Client() = default;
    void setClient(QHostAddress address, quint16 port);
    QString getMessage() const;

signals:
    void connected();
    void disconnected();
    void messageChangedd(QString message);

public slots:
    void connectToServer();
    void disconnectToServer();
    void sendMessage();
    void setMessage(const QString& message);

private:
    void sendLogin();
    QTcpSocket client_;
    QHostAddress address_;
    quint16 port_;
    QString message_;
    QString username_;
};

#endif // CLIENT_H
