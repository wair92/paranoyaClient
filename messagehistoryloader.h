#ifndef MESSAGEHISTORYLOADER_H
#define MESSAGEHISTORYLOADER_H
#include "message.h"
#include <QObject>
#include <QString>
#include <QVector>

class MessageHistoryLoader: public QObject
{
    Q_OBJECT
public:
    MessageHistoryLoader(QString path);
    QVector<Message> loadMessages();
    void getUsersWhoWrote();
signals:
    void messagesLoaded(QVector<Message> messages);
    void messageFileMissing();
    void messageFileParseError();
    void messageFileEmpty();
    void usersWhoWrote(QStringList users);
private:
    QString filePath_;
    bool parseFile();
};

#endif // MESSAGEHISTORYLOADER_H
