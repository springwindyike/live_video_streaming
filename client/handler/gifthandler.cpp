#include "gifthandler.h"
#include "singleton.h"
#include <QMessageBox>

#include "chatwidget.h"
#include "rechargewidget.h"
#include <QDebug>

GiftHandler::GiftHandler()
{

}

void GiftHandler::sendGift(const QString &receiver, const QString &sender, const QString &kind)
{
    Protocol protocol(Protocol::gift);
    protocol["type"] = "gift";
    protocol["kind"] = kind;
    protocol["receiver"] =receiver;
    protocol ["sender"] = sender;

    SingleTon<QTcpSocket>::getInstance()->write(protocol.pack());
}

void GiftHandler::recharge(const QString &username, double money)
{
    Protocol protocol(Protocol::gift);
    protocol["type"] = "recharge";
    protocol["username"] = username;
    protocol["money"] = money;

    SingleTon<QTcpSocket>::getInstance()->write(protocol.pack());
}

void GiftHandler::rechargeWindow(const QString &username)
{
    SingleTon<RechargeWidget>::getInstance()->setUsername(username);
    SingleTon<RechargeWidget>::getInstance()->setWindowTitle("充值界面");
    SingleTon<RechargeWidget>::getInstance()->show();
}

void GiftHandler::handle(QTcpSocket *, const Protocol &protocol)
{
    QString type = protocol["type"].toString();
    QString result = protocol["result"].toString();
    if(type == "send"){
        if(result == "failed"){
            QMessageBox::warning(nullptr,"送礼信息","送礼失败");
            return;
        }else if(result == "success"){

            QMessageBox::information(nullptr,"送礼信息","送礼成功");
            /** 刷新积分以及余额 **/
            SingleTon<ChatWidget>::getInstance()->flushCount(protocol["count"].toInt());
            SingleTon<ChatWidget>::getInstance()->flushBalance(protocol["balance"].toDouble());
        }
    }else if (type == "receive") {
        if(result == "failed"){
            return;
        }else if(result == "success"){//主播余额增加

            /** 刷新积分以及余额 **/
            SingleTon<ChatWidget>::getInstance()->flushCount(protocol["count"].toInt());
            SingleTon<ChatWidget>::getInstance()->flushBalance(protocol["balance"].toDouble());

        }
    }else if (type == "recharge") {
        if(result == "failed"){
            QMessageBox::warning(nullptr,"充值信息","充值失败");
            return;
        }else if(result == "success"){
            QMessageBox::information(nullptr,"充值信息","充值成功");

            /** 刷新余额 **/
            SingleTon<ChatWidget>::getInstance()->flushBalance(protocol["balance"].toDouble());
        }
    }
}
