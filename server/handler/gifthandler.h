#ifndef GIFTHANDLER_H
#define GIFTHANDLER_H

#include "ihandler.h"

class GiftHandler : public IHandler
{
public:
    GiftHandler();

    // IHandler interface
public:
    void handle(QTcpSocket *socket, const Protocol &protocol);
};

#endif // GIFTHANDLER_H
