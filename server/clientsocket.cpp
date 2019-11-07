#include "clientsocket.h"
#include "protocol.h"
#include "handler/handler.h"
#include "dao/sqlmanager.h"
#include "room/roommanager.h"
#include "singleton.h"


ClientSocket::ClientSocket(QTcpSocket *socket, QObject *parent)
    :QObject(parent),
      socket(socket)
{
    count = 0;
//    /** 1.断开连接时删除 **/
//    QObject::connect(socket,&QTcpSocket::disconnected,[=](){
//        this->socket->deleteLater();
//        this->deleteLater();
//    });

    /** 2.检测到有新信息 **/
    QObject::connect(socket,&QTcpSocket::readyRead,[=](){
        /** 拆包 **/
        QByteArray array = socket->readAll();
        Protocol p;
        int len;
        while((len = p.unPack(array)) > 0){
            array = array.mid(len);
            if(p.getType() == Protocol::heartbeat){
                count = 0;
            }else if(p.getType() == Protocol::login){
                Handler::handle(socket,p);
                username = p["username"].toString();//设置用户名
                qDebug()<<"username: "<<username;
            }else {
                Handler::handle(socket,p);
            }
        }
    });

    /** 3.检测心跳 **/   //disconned 函数有时失效，所以常用的是心跳检测
    timer = startTimer(5000);
}

void ClientSocket::timerEvent(QTimerEvent *)
{
    count++;
//    qDebug()<<count;
    if(count > 3){
        qDebug()<<"log out";
        SqlDao* dao = SingleTon<SqlManager>::getInstance()->dao();
        dao->updateUserOnline(username,false);

        /**  从房间删除用户 以及下线处理 **/
        int flag = 0;
        QVector<Room>& rooms = SingleTon<RoomManager>::getInstance()->allRooms();
        //这里必须引用
        for (int i = 0;i < rooms.size();i++) {
            QVector<User> users = rooms[i].allUsers();
            for (int j = 0;j < users.size();j++) {
                if(users[j].getUsername() == username){
                     rooms[i].removeUser(users[j]);
                     flag = 1;
                     qDebug()<<"delete succeed";
                     break;
                }
            }

            if(flag == 1){
                break;
            }
        }
        this->socket->deleteLater();//万恶之源
        this->deleteLater();
    }
}
