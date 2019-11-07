#include "client.h"
#include "singleton.h"
#include "protocol.h"
#include "handler/handler.h"
#include <QMessageBox>
#include "chatwidget.h"
#include "loginwidget.h"


Client::Client(QObject *parent)
    :QObject(parent),connectFlag(false)
{
    /** 单例 方便访问socket**/
    socket = SingleTon<QTcpSocket>::getInstance();
    socket->connectToHost(QHostAddress("192.168.50.136"),12000);
    startTimer(5000);//心跳计时
    QObject::connect(socket,&QTcpSocket::readyRead,[=](){
        QByteArray array = socket->readAll();
        Protocol p;
        int len;
        while((len = p.unPack(array)) > 0){
            array = array.mid(len);//砍掉已读的字节
            /** 策略模式 根据类型进行处理 **/
            Handler::handle(socket,p);
        }
    });

}



Client::~Client()
{

}

void Client::show()
{
    SingleTon<LoginWidget>::getReference().show();
}

/** 心跳 **/
void Client::timerEvent(QTimerEvent *)
{
    Protocol p(Protocol::heartbeat);
    p["heartbeat"] = 1;
    socket->write(p.pack());
    qDebug()<<"heartbeat";

    /** 自动重连 **/
    if(socket->state() != 3){
        QMessageBox::warning(nullptr,"网络连接","您已掉线，自动重连中");//服务器异常中断后数据恢复暂时未考虑
        socket->connectToHost(QHostAddress("192.168.50.136"),12000);
        connectFlag = true;
    }
    if(connectFlag && socket->state() == 3){
        QMessageBox::information(nullptr,"网络连接","重新连接成功");
        SingleTon<ChatWidget>::getInstance()->hide();//强制返回大厅界面
        SingleTon<LoginWidget>::getInstance()->show();
        connectFlag = false;
    }
}
