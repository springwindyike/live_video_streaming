#ifndef SQLDAOMYSQL_H
#define SQLDAOMYSQL_H

#include "sqldao.h"

class SqlDaoMysql : public SqlDao
{
public:
    SqlDaoMysql();

public:
    virtual bool insertUser(const SqlUser &user);
    virtual bool deleteUser(const QString &username);
    virtual bool selectUser(const QString &username,SqlUser &user);
    virtual bool updateUserOnline(const QString &username,bool online);
    virtual bool updateUserBalance(const QString &username,double balance);
    virtual bool updateUserCount(const QString &username,int count);
};

#endif // SQLDAOMYSQL_H
