#ifndef GIFTHANDLER_H
#define GIFTHANDLER_H

#include "ihandler.h"

class GiftHandler : public IHandler
{
public:
    GiftHandler();

    void sendGift(const QString& receiver,const QString& sender,const QString& kind);
    void recharge(const QString& username,double money);
    void rechargeWindow(const QString& username);
    // IHandler interface
public:
    void handle(QTcpSocket *socket, const Protocol &protocol);
};

#endif // GIFTHANDLER_H
