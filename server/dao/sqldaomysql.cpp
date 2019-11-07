#include "sqldaomysql.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

SqlDaoMysql::SqlDaoMysql()
{

}

bool SqlDaoMysql::insertUser(const SqlUser &user)
{
    bool ret = false;
    QSqlQuery query;
    query.prepare("insert into user (username,password)values(:username,:password);");
    query.bindValue(":username",user.getUsername());
    query.bindValue(":password",user.getPassword());

    ret = query.exec();
    if(!ret){
        qDebug()<<query.lastError().text();
    }

    return ret;
}

bool SqlDaoMysql::deleteUser(const QString &username)
{
    bool ret = false;
    QSqlQuery query;
    query.prepare("delete from user where username = :username;");
    query.bindValue(":username",username);

    ret = query.exec();
    if(!ret){
        qDebug()<<query.lastError().text();
    }

    return ret;
}

bool SqlDaoMysql::selectUser(const QString &username, SqlUser &user)
{
    bool ret = false;
    QSqlQuery query;
    query.prepare("select * from user where username = :username;");
    query.bindValue(":username",username);

    ret = query.exec();
    if(!ret){
        qDebug()<<query.lastError().text();
    }else{
        ret = query.first();
        if(ret){
            user.setUsername(query.value("username").toString());
            user.setPassword(query.value("password").toString());
            user.setOnline(query.value("online").toBool());
            user.setBalance(query.value("balance").toDouble());
            user.setCount(query.value("count").toInt());
        }
    }

    return ret;
}

bool SqlDaoMysql::updateUserOnline(const QString &username, bool online)
{
    bool ret = false;
    QSqlQuery query;
    query.prepare("update user set online = :online where username = :username;");
    query.bindValue(":online",online);
    query.bindValue(":username",username);

    ret = query.exec();
    if(!ret){
        qDebug()<<query.lastError().text();
    }

    return ret;
}

bool SqlDaoMysql::updateUserBalance(const QString &username, double balance)
{
    bool ret = false;
    QSqlQuery query;
    query.prepare("update user set balance = :balance where username = :username;");
    query.bindValue(":balance", balance);
    query.bindValue(":username", username);

    ret = query.exec();
    if(!ret){
        qDebug()<<query.lastError().text();
    }

    return ret;
}

bool SqlDaoMysql::updateUserCount(const QString &username, int count)
{
    bool ret = false;
    QSqlQuery query;
    query.prepare("update user set count = :count where username = :username;");
    query.bindValue(":count", count);
    query.bindValue(":username", username);

    ret = query.exec();
    if(!ret){
        qDebug()<<query.lastError().text();
    }

    return ret;

}
