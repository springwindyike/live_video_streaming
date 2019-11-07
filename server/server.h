#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);

    void initServer();
    void initServer(const QString& ip,int port);

    inline void setIp(const QString& ip){this->ip = ip;}
    inline void setPort(int port){this->port = port;}

    inline QString getIp()const{return this->ip;}
    inline int getPort()const{return this->port;}

private:
    QTcpServer *server;

    int port;
    QString ip;
};

#endif // SERVER_H
