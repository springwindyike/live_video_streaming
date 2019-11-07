#ifndef ROOM_H
#define ROOM_H

#include <QString>
#include <QVector>


#include "user.h"
#include "protocol.h"
class Room
{

public:
    Room(const QString& roomName = "");

    inline QString getRoomName()const{return this->roomName;}
    void setRoomName(const QString& roomName){this->roomName = roomName;}

    void appendUser(const User& user);
    void removeUser(const User& user);
    inline const QVector<User>& allUsers()const{return this->users;}
    //inline QVector<User>& allUsers(){return this->users;}
    void mute(const QString& username,const QByteArray& info);
    void enable(const QString& username,const QByteArray& info);

    void setShareAddress(const QString &value);
    void setPort(int value);

    QString getShareAddress() const;
    int getPort() const;

private slots:
    void onReadyRead();
private:
    QString roomName;
    QVector<User> users;

    QString shareAddress;
    int port;

};

#endif // ROOM_H
