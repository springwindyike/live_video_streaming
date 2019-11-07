#include "loginhandler.h"
#include "dao/sqlmanager.h"
#include "singleton.h"

#include <QDebug>
LoginHandler::LoginHandler()
{

}

void LoginHandler::handle(QTcpSocket *socket, const Protocol &protocol)
{
    QString type = protocol["type"].toString();
 qDebug()<<"收到信息  "<<type;
    if(type == "login"){
        QString username = protocol["username"].toString();
        QString password = protocol["password"].toString();

        Protocol p(Protocol::login);
        p["type"] = "login";

        /** 1.是否注册 **/
        SqlDao* sd = SingleTon<SqlManager>::getInstance()->dao();
        SqlUser user;
        if(sd->selectUser(username,user) == false){
            p["result"] = "failed";
            p["info"] = "无此用户";
            socket->write(p.pack());
            delete sd;
            return;
        }

        /** 2.判断重复登录 **/
        if(user.getOnline()){
            p["result"] = "failed";
            p["info"] = "重复登录";
            socket->write(p.pack());
            delete  sd;
            return;
        }

        /** 3.密码错误 **/
        if(user.getPassword() != password){
            p["result"] = "failed";
            p["info"] = "密码错误";
            socket->write(p.pack());
            delete sd;
            return;
        }

        /** 4.密码正确 **/
        p["result"] = "success";
        p["info"] = "登录成功";

        sd->updateUserOnline(username,true);

        socket->write(p.pack());
        return;
    }else if (type == "quit") {
        QString username = protocol["username"].toString();

        qDebug()<<"退出成功";
        SqlDao* sd = SingleTon<SqlManager>::getInstance()->dao();
        sd->updateUserOnline(username,false);
        delete sd;
        return;
    }
}
