#ifndef ROOMHANDLER_H
#define ROOMHANDLER_H

#include "ihandler.h"

class RoomHandler : public IHandler
{
public:
    RoomHandler();

    void createRoom(const QString& roomName,int port = 9999);
    void flushRoom();
    void joinRoom(const QString& roomName,const QString& username);
    void quitRoom(const QString& roomName,const QString& username);
    void muteUser(const QString& roomName,const QString& username);
    void enableUser(const QString& roomName,const QString& username);
    void kickUser(const QString& roomName,const QString& username);
    // IHandler interface
public:
    void handle(QTcpSocket *socket, const Protocol &protocol);
};

#endif // ROOMHANDLER_H
