#include "server.h"
#include "clientsocket.h"
#include "singleton.h"
#include "dao/sqlmanager.h"
#include <QDebug>

Server::Server(QObject *parent)
    :QObject (parent),
    server(nullptr),port(0)
{
    /** 数据库设置 **/
    SqlManager* manager = SingleTon<SqlManager>::getInstance();
    manager->setHostname("localhost");
    manager->setUsername("root");
    manager->setPassword("123456");
    manager->setPort(3306);
    manager->setDatabaseName("user");

    manager->init("MYSQL");
    manager->open();
}

void Server::initServer()
{
    /** 1.创建套接字 **/
    server = new QTcpServer(this);//QObject 多态
    /** 2.绑定 3.监听套接字 **/
    server->listen(QHostAddress(ip),port);
    /** 4.接收套接字 **/

    /** 匿名函数/lamda函数 **/
    QObject::connect(server,&QTcpServer::newConnection,[=](){
        while (server->hasPendingConnections()) {
            QTcpSocket* socket = server->nextPendingConnection();
            if(socket){
                new ClientSocket(socket,this);
                qDebug()<<"新连接";
            }
        }
    });
}

void Server::initServer(const QString &ip, int port)
{
    this->setIp(ip);
    this->setPort(port);
    this->initServer();
}
