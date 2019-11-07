#include "registerhandler.h"
#include "singleton.h"
#include <QApplication>
#include <QMessageBox>

RegisterHandler::RegisterHandler()
{

}

void RegisterHandler::regist(const QString &username, const QString &password)
{
    Protocol p(Protocol::regist);
    p["type"] = "regist";
    p["username"] = username;
    p["password"] = password;

    SingleTon<QTcpSocket>::getInstance()->write(p.pack());
}

void RegisterHandler::logout(const QString &username)
{
    Protocol protocol(Protocol::regist);
    protocol["type"] = "logout";
    protocol["username"] = username;
    SingleTon<QTcpSocket>::getInstance()->write(protocol.pack());
}

void RegisterHandler::handle(QTcpSocket *, const Protocol &protocol)
{
    QString type = protocol["type"].toString();
    QString result = protocol["result"].toString();
    QString info = protocol["info"].toString();

    if(type == "regist"){
        if(result == "success"){
            QMessageBox::information(nullptr,"注册信息",info);
        }else if(result == "failed"){
            QMessageBox::warning(nullptr,"注册信息",info);
        }
    }
    else if(type == "logout"){
        if(result == "success"){
            QMessageBox::information(nullptr,"注销信息",info);
            QApplication* app;
            app->quit();
        }else if(result == "failed"){
            QMessageBox::warning(nullptr,"注销信息",info);
        }
    }
}

