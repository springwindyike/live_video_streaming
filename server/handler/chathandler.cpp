#include "chathandler.h"
#include <QDateTime>
#include "singleton.h"
#include "room/roommanager.h"
#include "dao/sqlmanager.h"

ChatHandler::ChatHandler()
{

}

void ChatHandler::handle(QTcpSocket *socket, const Protocol &protocol)
{
    QString type = protocol["type"].toString();
    if(type == "groupChat"){
        Protocol p(Protocol::chat);
        QString roomName = protocol["roomName"].toString();
        QString username = protocol["username"].toString();

        p["type"] = "groupChat";

        Room* room = SingleTon<RoomManager>::getInstance()->selectRoom(roomName);
        if(room == nullptr) return;
        const QVector<User>& users = room->allUsers();
        for (int i = 0;i < users.size();i++) {
            if(users[i].getUsername() == username){
                if(users[i].getSpeak() == false){
                    p["message"] = "你已被禁言";
                    socket->write(p.pack());
                    return;
                }
            }
        }

        QString message = username + " " + QDateTime::currentDateTime().toString()
                + " :\n" +protocol["message"].toString();
        p["message"] = message;
        p["subMessage"] = protocol["message"].toString();

        for (int i = 0;i < users.size();i++) {
            if(users[i].getUsername() != username){
                users[i].getSocket()->write(p.pack());
            }else{
                int count = protocol["count"].toInt();
                count+=10;
                SqlDao * dao = SingleTon<SqlManager>::getInstance()->dao();
                dao->updateUserCount(username,count);//没有选择查数据库，节省时间
                p["count"] = count;
                p["flag"] = 1;
                socket->write(p.pack());
            }
        }
    }
}


