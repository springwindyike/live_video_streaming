#ifndef CHATWIDGET_H
#define CHATWIDGET_H

#include <QWidget>
#include "screen/screenmanager.h"
#include <QCamera>
#include <QCameraInfo>
#include <QUdpSocket>

#include <QAudio>
#include <QAudioFormat>
#include <QAudioInput>
#include <QAudioOutput>
#include <QIODevice>

#include <QMenu>


namespace Ui {
class ChatWidget;
}

class ChatWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChatWidget(QWidget *parent = nullptr);
    ~ChatWidget();

    void setRoomName(const QString& roomName);
    void setUsername(const QString& username);
    void flushUserList(const QStringList& userlist);
    void flushCount(int count);
    void flushBalance(double balance);
    void appendText(const QString& message);
    void acrossScreen(const QString& message);
    void giftShow(const QString& kind);


    void connectUdp();
    void setShareAddress(const QString &value);
    void setPort(int value);
    void setWritePort(int value);

    void clearText();
    void addAction();
    void cancelAction();
private:
    Ui::ChatWidget *ui;
    ScreenManager* screenManager;
    QCamera* camera;

    QIODevice* audioOutputIODevice;
    QIODevice* audioInputIODevice;
    QAudioInput* audioInput;
    QAudioOutput* audioOutput;
    QAudioFormat format;

    QUdpSocket* socketWrite;
    QUdpSocket* socketRead;
    QUdpSocket* socketAudioRead;
    QUdpSocket* socketAudioWrite;

    QMenu* menu;

    QString shareAddress;
    int port;
    int writePort;
    int count;
    int timerId;
    // QWidget interface
protected:


    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event);
private slots:
    void on_btnSend_clicked();
    void on_btnCharge_clicked();
    void on_btnFlower_clicked();
    void on_btnPlane_clicked();
    void on_btnRocket_clicked();
    void on_rbtnShowChat_clicked();
    void on_btnHall_clicked();
    void on_cbBarrage_clicked();
    void on_cbCamera_clicked();
    void videoChangedSlot(QVideoFrame curFrame);
    void onReadyReadSocket();
    void on_cbVideo_clicked();

    void on_cbSound_clicked();
    void onReadyReadAudio();
    void captureDataFromDevice();
    void onCustomContextMenuRequested(const QPoint &pos);
    void onTriggered(QAction* action);


    // QObject interface
protected:
    void timerEvent(QTimerEvent *event);
};

#endif // CHATWIDGET_H
