#ifndef REGISTERHANDLER_H
#define REGISTERHANDLER_H

#include "ihandler.h"

class RegisterHandler : public IHandler
{
public:
    RegisterHandler();

    virtual void handle(QTcpSocket *socket,const Protocol &protocol);
};

#endif // REGISTERHANDLER_H
