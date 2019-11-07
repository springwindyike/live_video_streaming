#include "roomhandler.h"
#include "hallwidget.h"
#include "chatwidget.h"
#include "singleton.h"

#include <QJsonArray>
#include <QMessageBox>
#include <QStringList>

RoomHandler::RoomHandler()
{

}

void RoomHandler::createRoom(const QString &roomName,int port)
{
    Protocol protocol(Protocol::room);
    protocol["type"] = "createRoom";
    protocol["roomName"] = roomName;

    /**  组播 地址 **/
    protocol["shareAddress"] = "239.168.168.168";//ip固定
    protocol["port"] = port;

    SingleTon<QTcpSocket>::getInstance()->write(protocol.pack());
}

void RoomHandler::flushRoom()
{
    Protocol protocol(Protocol::room);
    protocol["type"] = "flushRoom";

    SingleTon<QTcpSocket>::getInstance()->write(protocol.pack());
}

void RoomHandler::joinRoom(const QString &roomName, const QString &username)
{
//    if(roomName == username) return; 插入时已判断重名

    Protocol protocol(Protocol::room);
    protocol["type"] = "joinRoom";
    protocol["roomName"] = roomName;
    protocol["username"] = username;

    SingleTon<QTcpSocket>::getInstance()->write(protocol.pack());
}

void RoomHandler::quitRoom(const QString &roomName, const QString &username)
{
    Protocol protocol(Protocol::room);
    protocol["type"] = "quitRoom";
    protocol["roomName"] = roomName;
    protocol["username"] = username;

    SingleTon<QTcpSocket>::getInstance()->write(protocol.pack());
}

void RoomHandler::muteUser(const QString &roomName, const QString &username)
{
    Protocol protocol(Protocol::room);
    protocol["type"] = "mute";
    protocol["roomName"] = roomName;
    protocol["username"] = username;

    SingleTon<QTcpSocket>::getInstance()->write(protocol.pack());
}

void RoomHandler::enableUser(const QString &roomName, const QString &username)
{
    Protocol protocol(Protocol::room);
    protocol["type"] = "enable";
    protocol["roomName"] = roomName;
    protocol["username"] = username;

    SingleTon<QTcpSocket>::getInstance()->write(protocol.pack());
}

void RoomHandler::kickUser(const QString &roomName, const QString &username)
{
    Protocol protocol(Protocol::room);
    protocol["type"] = "kick";
    protocol["roomName"] = roomName;
    protocol["username"] = username;

    SingleTon<QTcpSocket>::getInstance()->write(protocol.pack());
}

void RoomHandler::handle(QTcpSocket *, const Protocol &protocol)
{
    QString type = protocol["type"].toString();
    QString result = protocol["result"].toString();

    if(type == "createRoom"){
        QString roomName = protocol["roomName"].toString();
        if(result == "success"){

            /** 切换房间并刷新积分和余额 **/
            SingleTon<ChatWidget>::getInstance()->setRoomName(roomName);
            SingleTon<ChatWidget>::getInstance()->setUsername(roomName);//创建者名字和房间名同名
            SingleTon<ChatWidget>::getInstance()->clearText();
            SingleTon<ChatWidget>::getInstance()->setWritePort(protocol["writePort"].toInt());//设置发送数据端口
            SingleTon<HallWidget>::getInstance()->hide();
            SingleTon<ChatWidget>::getInstance()->flushCount(protocol["count"].toInt());//刷新积分以及余额
            SingleTon<ChatWidget>::getInstance()->flushBalance(protocol["balance"].toDouble());
            SingleTon<ChatWidget>::getInstance()->addAction();
            SingleTon<ChatWidget>::getInstance()->show();
        }else if(result == "failed"){
            QString info = protocol["info"].toString();
            QMessageBox::warning(nullptr,"创建房间信息",info);
        }
    }else if (type == "flushRoom") {
        QJsonArray roomArray = protocol["roomList"].toArray();
        QStringList rooms;

        for (int i = 0;i < roomArray.size();i++) {
            rooms.append(roomArray[i].toString());
        }

        SingleTon<HallWidget>::getInstance()->flushRoom(rooms);
    }else if (type == "joinRoom") {
        QString roomName = protocol["roomName"].toString();
        QString username = protocol["username"].toString();

        /** 切换界面并更新积分和余额 **/
        SingleTon<ChatWidget>::getInstance()->setRoomName(roomName);
        SingleTon<ChatWidget>::getInstance()->setUsername(username);
        SingleTon<ChatWidget>::getInstance()->clearText();
        SingleTon<ChatWidget>::getInstance()->flushCount(protocol["count"].toInt());
        SingleTon<ChatWidget>::getInstance()->flushBalance(protocol["balance"].toDouble());
        qDebug()<<"进入房间"<<protocol["port"].toInt()<<" 1  "<<protocol["shareAddress"].toString();
        SingleTon<ChatWidget>::getInstance()->setPort(protocol["port"].toInt());//设置接收数据的端口和IP
        SingleTon<ChatWidget>::getInstance()->setShareAddress(protocol["shareAddress"].toString());
        SingleTon<HallWidget>::getInstance()->hide();
        if(username == roomName){
            SingleTon<ChatWidget>::getInstance()->addAction();
            SingleTon<ChatWidget>::getInstance()->setWritePort(protocol["port"].toInt());//房主掉线重新设置write端口
        }
        SingleTon<ChatWidget>::getInstance()->show();

    }else if (type == "quitRoom") {
        SingleTon<HallWidget>::getInstance()->show();
        SingleTon<ChatWidget>::getInstance()->cancelAction();
        SingleTon<ChatWidget>::getInstance()->hide();

    }else if (type == "userList") {//更新用户列表
        QJsonArray userArray = protocol["userList"].toArray();
        QStringList userList;

        for (int i = 0;i < userArray.size();i++) {
            userList.append(userArray[i].toString());
        }
        SingleTon<ChatWidget>::getInstance()->flushUserList(userList);
    }else if (type == "mute") {
        QMessageBox::warning(nullptr,"警告","你已被禁言");
    }else if (type == "kick") {
        QMessageBox::warning(nullptr,"警告","你已被移出房间");
        SingleTon<ChatWidget>::getInstance()->hide();
        SingleTon<HallWidget>::getInstance()->show();//大厅名字已设定
    }else if (type == "enable") {
        QMessageBox::information(nullptr,"提示","禁言已恢复");
    }
}
