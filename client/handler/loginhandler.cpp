#include "loginhandler.h"
#include "singleton.h"
#include "loginwidget.h"
#include "hallwidget.h"

#include <QMessageBox>
#include <QDebug>

LoginHandler::LoginHandler()
{

}

void LoginHandler::login(const QString &username, const QString &password)
{
    Protocol protocol(Protocol::login);
    protocol["type"] = "login";
    protocol["username"] = username;
    protocol["password"] = password;

    SingleTon<QTcpSocket>::getInstance()->write(protocol.pack());
}

void LoginHandler::quit(const QString &username)
{
    Protocol protocol(Protocol::login);
    protocol["type"] = "quit";
    protocol["username"] = username;

    SingleTon<QTcpSocket>::getInstance()->write(protocol.pack());

    //qDebug()<<"已发送";
}

void LoginHandler::handle(QTcpSocket *, const Protocol &protocol)
{
    QString type = protocol["type"].toString();
    if(type == "login"){
        QString result = protocol["result"].toString();
        QString info = protocol["info"].toString();

        if(result == "success"){
            SingleTon<HallWidget>::getInstance()->setUsername(
                        SingleTon<LoginWidget>::getInstance()->getLastUsername());//设置用户名
            SingleTon<LoginWidget>::getInstance()->hide();
            SingleTon<HallWidget>::getInstance()->show();
        }else if (result == "failed") {
            QMessageBox::warning(nullptr,"登录信息",info);
        }
    }
}
