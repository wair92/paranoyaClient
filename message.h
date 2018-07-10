#ifndef MESSAGE_H
#define MESSAGE_H

#include <QString>

class Message
{
public:
    Message(QString sender, QString receiver, QString message);
    QString getSender() const {return sender_;}
    QString getReceiver() const {return receiver_;}
    QString getMessage() const {return message_;}
private:
    QString sender_;
    QString message_;
    QString receiver_;
    bool read = {false};
};

#endif // MESSAGE_H
