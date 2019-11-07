#ifndef REGISTERHANDLER_H
#define REGISTERHANDLER_H

#include "ihandler.h"

class RegisterHandler : public IHandler
{
public:
    RegisterHandler();
    /** register 是关键字**/
    void regist(const QString& username,const QString& password);
    void logout(const QString& username);
    virtual void handle(QTcpSocket *socket,const Protocol &protocol);
};

#endif // REGISTERHANDLER_H
