#include "gifthandler.h"
#include "dao/sqlmanager.h"
#include "singleton.h"

#include "room/roommanager.h"
#include <QVector>

#include <QDebug>

GiftHandler::GiftHandler()
{

}

void GiftHandler::handle(QTcpSocket *socket, const Protocol &protocol)
{

    QString type = protocol["type"].toString();
    SqlDao* dao = SingleTon<SqlManager>::getInstance()->dao();

    if(type == "gift"){
        QString receiver = protocol["receiver"].toString();
        QString sender = protocol["sender"].toString();
        SqlUser receiveUser;
        SqlUser sendUser;
        dao->selectUser(receiver,receiveUser);
        dao->selectUser(sender,sendUser);

        QString kind = protocol["kind"].toString();
        QString kindChinese;
        int senderCount = sendUser.getCount();
        int receiverCount = receiveUser.getCount();
        double senderBalance = sendUser.getBalance();
        double receiverBalance = receiveUser.getBalance();
        bool retRecv = true;
        bool retSend = true;//记录数据更新是否成功
        if(kind == "flower"){
            kindChinese = "鲜花";
            senderCount-=100;
            receiverCount+=100;
            retSend = dao->updateUserCount(sender,senderCount);
            retRecv = dao->updateUserCount(receiver,receiverCount);
        }else if(kind == "plane") {
            kindChinese = "飞机";
            senderBalance-=100;
            receiverBalance+=100;
            retSend = dao->updateUserBalance(sender,senderBalance);
            retRecv = dao->updateUserBalance(receiver,receiverBalance);
        }else if(kind == "rocket"){
            kindChinese = "火箭";
            senderBalance-=500;
            receiverBalance+=500;
            retSend = dao->updateUserBalance(sender,senderBalance);
            retRecv = dao->updateUserBalance(receiver,receiverBalance);
        }

//        qDebug()<<"收到";
        Protocol pChat(Protocol::chat);
        pChat["type"] = "giftChat";
        Protocol pGift(Protocol::gift);
        bool flag = retRecv&&retSend;
        /** 假定两次跟新操作只能都成功或者都失败 **/
        if(!flag){
            pGift["type"] = "send";
            pGift["result"] = "failed";
            socket->write(pGift.pack());
            return;
        }else {
            pGift["result"] = "success";
        }

        QString message = "感谢 " + sender + " 送出的 " + kindChinese + "，老板大气！";
        pChat["message"] = message;
        pChat["kind"] = kind;
        QTcpSocket* soc = nullptr;
        /** 送礼成功 所有人收到信息 **/
        Room* room = SingleTon<RoomManager>::getInstance()->selectRoom(receiver);
        const QVector<User>& users = room->allUsers();
        for(int i = 0;i < users.size();i++){
                //之前的disconnect信号槽
             /** 发送前检查错误**/
            soc = users[i].getSocket();
            int state = soc->state();
            qDebug()<<"state  "<<state;
            if(state != 3) {
                /** 意外掉线处理 **/
                dao->updateUserOnline(users[i].getUsername(),false);
                room->removeUser(users[i]);
                continue;
            }

            /** 更新积分余额 **/
            if(users[i].getUsername() == receiver){
                pGift["type"] = "receive";
                pGift["count"] = receiverCount;
                pGift["balance"] = receiverBalance;
                soc->write(pGift.pack());
            }else if(users[i].getUsername() == sender){
                pGift["type"] = "send";
                pGift["count"] = senderCount;
                pGift["balance"] = senderBalance;

                soc->write(pGift.pack());
            }

            soc->write(pChat.pack());
        }

    }else if (type == "recharge") {
        SqlUser user;
        QString username = protocol["username"].toString();
        bool exist = dao->selectUser(username,user);

        double money = protocol["money"].toDouble();
        double balance = user.getBalance();
        balance+=money;
        bool ret = dao->updateUserBalance(username,balance);
        Protocol p(Protocol::gift);
        p["type"] = "recharge";
        if(!(ret&&exist)){
            p["result"] = "failed";
        }else {
            p["balance"] = balance;
            p["result"] = "success";
        }

        socket->write(p.pack());
    }
}
