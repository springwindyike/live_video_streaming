#ifndef CHATHANDLER_H
#define CHATHANDLER_H

#include "ihandler.h"

class ChatHandler : public IHandler
{
public:
    ChatHandler();

    // IHandler interface
public:
    void handle(QTcpSocket *socket, const Protocol &protocol);
};

#endif // CHATHANDLER_H
