#ifndef HANDLER_H
#define HANDLER_H

#include "protocol.h"
#include <QTcpSocket>

class Handler
{
public:
    Handler();

    static void handle(QTcpSocket* socket,const Protocol& protocol);
};

#endif // HANDLER_H
