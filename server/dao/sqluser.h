#ifndef SQLUSER_H
#define SQLUSER_H

#include <QString>

class SqlUser
{
private:
    QString username;
    QString password;
    bool online;
    double balance;
    int count;

public:
    SqlUser();
    SqlUser(const QString& username,const QString& password,bool online);
    inline QString getUsername()const{return this->username;}
    inline QString getPassword()const{return this->password;}
    inline bool getOnline()const{return this->online;}
    inline double getBalance()const{return this->balance;}
    inline int getCount()const{return this->count;}

    inline void setUsername(const QString& username){this->username = username;}
    inline void setPassword(const QString& password){this->password = password;}
    inline void setOnline(bool online){this->online = online;}
    inline void setBalance(double balance){this->balance = balance;}
    inline void setCount(int count){this->count = count;}
};

#endif // SQLUSER_H
