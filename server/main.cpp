#include <QApplication>

#include "server.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Server s;
    s.initServer("192.168.50.136",12000);

    return a.exec();
}
