#include "chathandler.h"
#include "singleton.h"
#include "chatwidget.h"


ChatHandler::ChatHandler()
{

}

void ChatHandler::groupChat(const QString &roomName, const QString &username, const QString message,int count)
{
    Protocol protocol(Protocol::chat);
    protocol["type"] = "groupChat";
    protocol["roomName"] = roomName;
    protocol["username"] = username;
    protocol["message"] = message;
    protocol["count"] = count;

    SingleTon<QTcpSocket>::getInstance()->write(protocol.pack());
}

void ChatHandler::handle(QTcpSocket *, const Protocol &protocol)
{
    QString type = protocol["type"].toString();
    if(type == "groupChat"){
       QString message = protocol["message"].toString();
       QString subMessage = protocol["subMessage"].toString();
       SingleTon<ChatWidget>::getInstance()->appendText(message);
       if(subMessage != ""){
       SingleTon<ChatWidget>::getInstance()->acrossScreen(subMessage);}//弹幕
       if(protocol["flag"].toInt()){//判断是否带count，带则刷新积分
             SingleTon<ChatWidget>::getInstance()->flushCount(protocol["count"].toInt());
       }
    }else if (type == "giftChat") {
        QString message = protocol["message"].toString();
        QString kind = protocol["kind"].toString();
        SingleTon<ChatWidget>::getInstance()->appendText(message);
        SingleTon<ChatWidget>::getInstance()->giftShow(kind);//展示礼物
    }
}
