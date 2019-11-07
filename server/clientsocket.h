#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <QObject>
#include <QTcpSocket>

class ClientSocket : public QObject
{
    Q_OBJECT
public:
    explicit ClientSocket(QTcpSocket* socket,QObject *parent = nullptr);

private:
    QTcpSocket* socket;
    int count;
    int timer;
    QString username;

    // QObject interface
protected:
    void timerEvent(QTimerEvent *event);
};

#endif // CLIENTSOCKET_H
