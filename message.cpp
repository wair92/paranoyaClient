#include "message.h"
#include <QString>

Message::Message(QString sender, QString receiver, QString message)
{
    sender_ = sender;
    receiver_ = receiver;
    message_ = message;
}
