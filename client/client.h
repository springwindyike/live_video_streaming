#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>

#include "loginwidget.h"
#include "chatwidget.h"
#include "loginwidget.h"

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);
    ~Client();

    void show();

    // QObject interface
protected:
    void timerEvent(QTimerEvent *event);
private:
    QTcpSocket* socket;
    bool connectFlag;
};

#endif // CLIENT_H
