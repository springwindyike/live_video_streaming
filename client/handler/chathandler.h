#ifndef CHATHANDLER_H
#define CHATHANDLER_H

#include "ihandler.h"

class ChatHandler : public IHandler
{
public:
    ChatHandler();

    void groupChat(const QString& roomName,const QString& username,const QString message,int count);
    // IHandler interface
public:
    void handle(QTcpSocket *socket, const Protocol &protocol);
};

#endif // CHATHANDLER_H
