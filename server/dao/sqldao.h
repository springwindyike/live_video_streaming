#ifndef SQLDAO_H
#define SQLDAO_H

#include "sqluser.h"

class SqlDao
{
public:
    SqlDao();
    virtual ~SqlDao();
    virtual bool insertUser(const SqlUser& user)=0;
    virtual bool deleteUser(const QString &username)=0;
    virtual bool selectUser(const QString& username,SqlUser& user)=0;
    virtual bool updateUserOnline(const QString& username,bool online)=0;
    virtual bool updateUserBalance(const QString &username,double balance)=0;
    virtual bool updateUserCount(const QString &username,int count)=0;

};

#endif // SQLDAO_H
