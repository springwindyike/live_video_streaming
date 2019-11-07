#include "registerhandler.h"
#include "dao/sqlmanager.h"
#include "singleton.h"


RegisterHandler::RegisterHandler()
{

}

void RegisterHandler::handle(QTcpSocket *socket, const Protocol &protocol)
{
    QString type = protocol["type"].toString();
    QString username = protocol["username"].toString();
    SqlDao* sd = SingleTon<SqlManager>::getInstance()->dao();
    Protocol p(Protocol::regist);

    if(type == "regist"){
    QString password = protocol["password"].toString();
    p["type"] = "regist";
    /** 1.判断重复注册 **/
    SqlUser user(username,password,false);
    if(sd->insertUser(user) == false){
        p["result"] = "failed";
        p["info"] = "注册失败";
    }else{
    /** 2.注册 **/
        p["result"] = "success";
        p["info"] = "注册成功";
    }

    socket->write(p.pack());

    }else if(type == "logout"){//注销用户回执
        p["type"] = "logout";
        if(sd->deleteUser(username) == false){
            p["result"] = "failed";
            p["info"] = "注销失败";
        }else{
            p["result"] = "success";
            p["info"] = "注销成功";
        }
        socket->write(p.pack());
    }
    delete sd;
}

