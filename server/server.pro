#-------------------------------------------------
#
# Project created by QtCreator 2019-09-22T20:47:21
#
#-------------------------------------------------

QT       += core gui
QT += network sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = server
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11
#CONFIG += mobility
#MOBILITY += multimedia

SOURCES += \
        clientsocket.cpp \
        dao/sqldao.cpp \
        dao/sqldaomysql.cpp \
        dao/sqlmanager.cpp \
        dao/sqluser.cpp \
        handler/chathandler.cpp \
        handler/gifthandler.cpp \
        handler/handler.cpp \
        handler/ihandler.cpp \
        handler/loginhandler.cpp \
        handler/registerhandler.cpp \
        handler/roomhandler.cpp \
        main.cpp \
        room/room.cpp \
        room/roommanager.cpp \
        room/user.cpp \
        server.cpp

HEADERS += \
    clientsocket.h \
    dao/sqldao.h \
    dao/sqldaomysql.h \
    dao/sqlmanager.h \
    dao/sqluser.h \
    handler/chathandler.h \
    handler/gifthandler.h \
    handler/handler.h \
    handler/ihandler.h \
    handler/loginhandler.h \
    handler/registerhandler.h \
    handler/roomhandler.h \
    protocol.h \
    room/room.h \
    room/roommanager.h \
    singleton.h \
    room/user.h \
    server.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
