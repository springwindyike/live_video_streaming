#include "roomhandler.h"
#include "room/roommanager.h"
#include "singleton.h"
#include "dao/sqlmanager.h"
#include <QJsonArray>
#include <QUdpSocket>

#include <QDebug>
RoomHandler::RoomHandler()
{

}

void RoomHandler::handle(QTcpSocket *socket, const Protocol &protocol)
{
    QString type = protocol["type"].toString();
    QString roomName = protocol["roomName"].toString();

    Protocol p(Protocol::room);
    Protocol pChange(Protocol::room);

    SqlDao* dao = SingleTon<SqlManager>::getInstance()->dao();//mysql数据接口
    SqlUser sqlUser;

    if(type == "createRoom"){//创建房间 返回余额以及积分
        p["type"] = "createRoom";

        /** 只检查端口重复 **/
        int port = protocol["port"].toInt();
        QVector<Room> rooms = SingleTon<RoomManager>::getInstance()->allRooms();
        for (int i = 0;i < rooms.size();i++) {
            if (port == rooms[i].getPort()) {
                p["result"] = "failed";
                p["info"] = "端口错误";
                socket->write(p.pack());
                return;
            }
        }


        Room* room = SingleTon<RoomManager>::getInstance()->createRoom(roomName);

        if(room != nullptr){
            User user;
            user.setUsername(roomName);
            user.setSocket(socket);
            room->appendUser(user);
            room->setPort(port);
            room->setShareAddress(protocol["shareAddress"].toString());
            p["writePort"] = port;
            p["result"] = "success";
            p["roomName"] = roomName;//房间名为创建者名字
            dao->selectUser(roomName,sqlUser);
            p["count"] = sqlUser.getCount();
            p["balance"] = sqlUser.getBalance();

            /** 列表显示自己 **/
            pChange["type"] = "userList";
            QJsonArray userArray;
            userArray.append(roomName);
            pChange["userList"] = userArray;
            socket->write(pChange.pack());
        }else{
            p["result"] = "failed";
            p["info"] = "已存在的房间";
        }
        socket->write(p.pack());

    }else if(type == "flushRoom"){
        p["type"] = "flushRoom";
        QJsonArray roomArray;

        const QVector<Room>& rooms = SingleTon<RoomManager>::getInstance()->allRooms();
        int i;
        for (i = 0;i < rooms.size();i++) {
            roomArray.append(rooms[i].getRoomName());
        }
        p["roomList"] = roomArray;

        socket->write(p.pack());
        return;
    }else if(type == "joinRoom"){//加入房间 返回余额以及积分
        QString username = protocol["username"].toString();

        Protocol pWelcome(Protocol::chat);
        pWelcome["type"] = "groupChat";

        Room* room = SingleTon<RoomManager>::getInstance()->selectRoom(roomName);
        if(room == nullptr){//房间不存在
            return;
        }else {
            User user;
            user.setUsername(username);
            user.setSocket(socket);
            room->appendUser(user);
        }

//        qDebug()<<"joinRoom";

        p["type"] = "joinRoom";
        p["roomName"] = roomName;
        p["username"] = username;
        dao->selectUser(username,sqlUser);
        p["count"] = sqlUser.getCount();
        p["balance"] = sqlUser.getBalance();
        p["port"] = room->getPort();
        p["shareAddress"] = room->getShareAddress();
        socket->write(p.pack());

        /** 刷新房间用户列表 **/
        pChange["type"] = "userList";
        QJsonArray userArray;
        QVector<User> users = room->allUsers();
        for (int i = 0;i < users.size();i++) {
            userArray.append(users[i].getUsername());
        }
        pChange["userList"] = userArray;
        QTcpSocket* soc = nullptr;
        int state = 0;
        for (int i = 0;i < users.size();i++) {
            soc = users[i].getSocket();
            state = soc->state();
            if(state != 3){
                /** 意外掉线处理 **/
                room->removeUser(users[i]);
                dao->updateUserOnline(users[i].getUsername(),false);
                continue;
            }
            pWelcome["message"] = "欢迎"+username+"进入直播间";
            pWelcome["subMessage"] = "欢迎"+username+"进入直播间";
            soc->write(pWelcome.pack());//欢迎用户进入
            soc->write(pChange.pack());//用户非正常断开连接  服务器无法写入
        }


    }else if(type == "quitRoom"){
        QString username = protocol["username"].toString();

        Room* room = SingleTon<RoomManager>::getInstance()->selectRoom(roomName);
        if(room == nullptr){//房间不存在
            return;
        }else {
            User user;
            user.setUsername(username);
            user.setSocket(socket);
            room->removeUser(user);
        }

        p["type"] = "quitRoom";
        p["roomName"] = roomName;
        p["username"] = username;
        socket->write(p.pack());

        /** 刷新房间用户列表 **/
        pChange["type"] = "userList";
        QJsonArray userArray;
        QVector<User> users = room->allUsers();
        for (int i = 0;i < users.size();i++) {
            userArray.append(users[i].getUsername());
        }
        pChange["userList"] = userArray;

        /** 判断socket是否可用  **/
        QTcpSocket* soc = nullptr;
        int state = 0;
        for (int i = 0;i < users.size();i++) {
            soc = users[i].getSocket();
            state = soc->state();
            if(state != 3){
                /** 意外掉线处理 **/
                room->removeUser(users[i]);
                dao->updateUserOnline(users[i].getUsername(),false);
                continue;
            }
            soc->write(pChange.pack());
        }
    }else if (type == "mute") {
        QString username = protocol["username"].toString();
        Room* room = SingleTon<RoomManager>::getInstance()->selectRoom(roomName);
        if(room == nullptr){//房间不存在
            return;
        }

        p["type"] = "mute";
        //p["info"] = "你已被禁言";
        room->mute(username,p.pack());
    }else if (type == "kick"){
        QString username = protocol["username"].toString();
        Room* room = SingleTon<RoomManager>::getInstance()->selectRoom(roomName);
        if(room == nullptr){//房间不存在
            return;
        }

        const QVector<User>& users = room->allUsers();
        for (int i = 0;i < users.size();i++) {
            if(users[i].getUsername() == username){
                p["type"] = "kick";
                //p["info"] = "你已被房主移出房间";
                users[i].getSocket()->write(p.pack());
                room->removeUser(users[i]);
                return;
            }
        }
    }else if (type == "enable") {
        QString username = protocol["username"].toString();
        Room* room = SingleTon<RoomManager>::getInstance()->selectRoom(roomName);
        if(room == nullptr){//房间不存在
            return;
        }

        p["type"] = "enable";
        //p["info"] = "你已被禁言";
        room->enable(username,p.pack());
    }

}
