#ifndef USER_H
#define USER_H

#include <QString>
#include <QTcpSocket>
#include <QUdpSocket>

class User
{
public:
    User();
    /** 重载< 比较或者删除用到**/
    bool operator<(const User& rhs)const;

    inline QString getUsername()const{return this->username;}
    inline void setUsername(const QString& username){this->username = username;}

    inline bool getSpeak()const{return this->speak;}
    inline void setSpeak(bool state){this->speak = state;}

    inline QTcpSocket* getSocket()const{return this->socket;}
    inline void setSocket(QTcpSocket* socket){this->socket = socket;}

private:
    QString username;
    QTcpSocket* socket;
    QUdpSocket* udpSocket;
    bool speak;
};

#endif // USER_H
