#-------------------------------------------------
#
# Project created by QtCreator 2019-09-22T20:46:39
#
#-------------------------------------------------

QT       += core gui

QT += network multimediawidgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = client
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
CONFIG += mobility
MOBILITY += multimedia


SOURCES += \
    handler/gifthandler.cpp \
        main.cpp \
    loginwidget.cpp \
    hallwidget.cpp \
    chatwidget.cpp \
    client.cpp \
    handler/handler.cpp \
    handler/ihandler.cpp \
    handler/loginhandler.cpp \
    handler/registerhandler.cpp \
    handler/roomhandler.cpp \
    handler/chathandler.cpp \
    rechargewidget.cpp \
    screen/screenmanager.cpp \
    screen/videosurface.cpp

HEADERS += \
    handler/gifthandler.h \
    loginwidget.h \
    hallwidget.h \
    chatwidget.h \
    client.h \
    rechargewidget.h \
    screen/imagepackage.h \
    screen/screenmanager.h \
    screen/videosurface.h \
    singleton.h \
    protocol.h \
    handler/handler.h \
    handler/ihandler.h \
    handler/loginhandler.h \
    handler/registerhandler.h \
    handler/roomhandler.h \
    handler/chathandler.h

FORMS += \
    loginwidget.ui\
    hallwidget.ui \
    chatwidget.ui \
    rechargewidget.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    gift.qrc


