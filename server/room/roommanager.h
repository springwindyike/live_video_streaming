#ifndef ROOMMANAGER_H
#define ROOMMANAGER_H

#include "room.h"

#include <QVector>

class RoomManager
{
public:
    RoomManager();

    Room* createRoom(const QString& roomName);
    void removeRoom(const QString& roomName);
    Room* selectRoom(const QString& roomName);
    void appendPort(int port);

    inline const QVector<Room>& allRooms()const{return this->rooms;}
    inline QVector<Room>& allRooms(){return this->rooms;}//函数重载

    inline const QVector<int>& allPorts()const{return  this->ports;}
private:
    QVector<Room> rooms;
    QVector<int> ports;
};

#endif // ROOMMANAGER_H
