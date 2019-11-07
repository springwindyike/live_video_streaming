#include "room.h"
#include "roommanager.h"
#include "singleton.h"
#include <QObject>

Room::Room(const QString &roomName)
    :roomName(roomName)
{
    /** 创建了一个空的容器 **/
}

void Room::appendUser(const User &user)
{
    int i;
    for(i = 0;i < users.size();i++){
        if(!(users[i] < user || user < users[i]) ) return;
    }

    users.append(user);
    return;
}

void Room::removeUser(const User &user)
{

    QVector<User>::iterator it;
    for (it = users.begin();it != users.end();++it) {
        if(!(*it < user || user < *it)){
            users.erase(it);
            return;
        }
    }

    /** 房间无人 **/
//    if(users.size() == 0){
//        SingleTon<RoomManager>::getInstance()->removeRoom(roomName);

    //    }
}

void Room::mute(const QString &username,const QByteArray& info)
{
    QVector<User>::iterator it;
    for (it = users.begin();it != users.end();++it) {
        if(it->getUsername() == username){
            it->setSpeak(false);
            it->getSocket()->write(info);
            return;
        }
    }
}

void Room::enable(const QString &username,const QByteArray& info)
{
    QVector<User>::iterator it;
    for (it = users.begin();it != users.end();++it) {
        if(it->getUsername() == username){
            it->setSpeak(true);
            it->getSocket()->write(info);
            return;
        }
    }

}

void Room::setPort(int value)
{
    port = value;
}

QString Room::getShareAddress() const
{
    return shareAddress;
}

int Room::getPort() const
{
    return port;
}

void Room::setShareAddress(const QString &value)
{
    shareAddress = value;
}



