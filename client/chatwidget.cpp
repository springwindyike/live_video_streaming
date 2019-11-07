#include "chatwidget.h"
#include "ui_chatwidget.h"
#include "handler/chathandler.h"
#include "handler/roomhandler.h"
#include "handler/gifthandler.h"
#include <QLabel>
#include <QMessageBox>
#include <ctime>

#include <QCloseEvent>
#include <QDebug>

#include "screen/videosurface.h"
#include "screen/imagepackage.h"

ChatWidget::ChatWidget(QWidget *parent):
    QWidget(parent),
    ui(new Ui::ChatWidget),
    screenManager(new ScreenManager),timerId(100)
    //socketAudioRead(nullptr)使用初始化列表
{
    ui->setupUi(this);
    /** 用户列表 **/
    menu = new QMenu(ui->lwUserList);
    ui->lwUserList->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(ui->lwUserList,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(onCustomContextMenuRequested(QPoint)));
    QObject::connect(menu,SIGNAL(triggered(QAction*)),this,SLOT(onTriggered(QAction*)));

    /** 1.视频 **/
    socketRead = nullptr;
    socketWrite = new QUdpSocket(this);
//  socketWrite->bind(QHostAddress::AnyIPv4,9999,QUdpSocket::ShareAddress);
//  发送者可以加入组播也可以不加入
//  socketWrite->joinMulticastGroup(QHostAddress("239.168.168.168"));
    QCameraInfo info = QCameraInfo::defaultCamera();
    camera = new QCamera(info,this);
    VideoSurface* vf = new VideoSurface(this);
    camera->setViewfinder(vf);
    QObject::connect(vf,SIGNAL(videoChanged(QVideoFrame)),this,SLOT(videoChangedSlot(QVideoFrame)));

    /** 2.音频 **/

    format.setSampleRate(8000);
    format.setChannelCount(1);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);
    audioInput = new QAudioInput(format,this);
    audioOutput = new QAudioOutput(format,this);
    audioOutputIODevice = audioOutput->start();
    socketAudioRead = nullptr;
    socketAudioWrite = new QUdpSocket(this);

    /** 3.背景 **/
    QString fileName = ":/image/background2.png";
    QPixmap pixmap = QPixmap(fileName);
    QPixmap scaledPixmap = pixmap.scaled(this->size(),Qt::KeepAspectRatio);
    QPalette palette(this->palette());
    palette.setBrush(QPalette::Background,QBrush(scaledPixmap));
    this->setPalette(palette);
}

ChatWidget::~ChatWidget()
{
    delete ui;
    killTimer(screenManager->getH_TimerID());//定时器关闭
    killTimer(screenManager->getV_TimerID());
    if(ui->cbBarrage->isChecked()) killTimer(screenManager->getBarrageTimerID());
    delete screenManager;
}

void ChatWidget::setRoomName(const QString &roomName)
{
    this->setWindowTitle(roomName);
}

void ChatWidget::setUsername(const QString &username)
{
    this->ui->lbUsername->setText(username);
}

void ChatWidget::flushUserList(const QStringList &userlist)
{
    ui->lwUserList->clear();
    ui->lwUserList->addItems(userlist);
}

void ChatWidget::flushCount(int count)
{
    ui->lbCount->setText(QString::number(count));
}

void ChatWidget::flushBalance(double balance)
{
    ui->lbBalance->setText(QString::number(balance));
}

void ChatWidget::appendText(const QString &message)
{
    ui->tbShow->append(message);
}

void ChatWidget::acrossScreen(const QString &message)
{
    QLabel* label = new QLabel(ui->lbVideo);
    qsrand(time(nullptr));


    int height = ui->lbVideo->height() - 40;//防止弹幕只显示一部分
    int x = ui->lbVideo->width();//从最右端开始
    int y = qrand()%height;//弹幕出现位置随机
    //随机彩色弹幕
    int r = qrand()%255;
    int g = qrand()%255;
    int b = qrand()%255;
    QString styleSheet = "color: rgb(";
    styleSheet += QString::number(r);
    styleSheet += ",";
    styleSheet += QString::number(g);
    styleSheet += ",";
    styleSheet += QString::number(b);
    styleSheet += ");";

    label->resize(120,40);
    label->setStyleSheet(styleSheet);
    label->setText(message);
    label->move(x,y);
//  label->show();
//  qDebug()<<"addLabel  "<<message;
    screenManager->addLabel(label);

}

void ChatWidget::giftShow(const QString &kind)
{
    /** 设置初始位置 **/
    QLabel* label = new QLabel(ui->lbVideo);
    if(kind == "plane"){
        label->resize(150,60);
        int x = ui->lbVideo->width();//从最右端开始
        int y = 0;
        label->move(x,y);
    }else if (kind == "rocket") {
        label->resize(80,120);
        int x = ui->lbVideo->width() - 80;
        int y = ui->lbVideo->height();
        label->move(x,y);
    }/*else if(kind == "flower"){
        label->resize(40,80);
        int x = ui->lbVideo->width() - 40;
        int y = ui->lbVideo->height() - 80;
        label->move(x,y);
    }*/else {
        delete label;
        return;
    }


    screenManager->giftShow(label,kind);
}

void ChatWidget::connectUdp()
{
//    qDebug()<<"udp连接 "<<shareAddress<<"  "<<port;
    count = 0;
    timerId = startTimer(300);
    socketRead = new QUdpSocket(this);
    socketRead->bind(QHostAddress::AnyIPv4,port,QUdpSocket::ShareAddress|QUdpSocket::ReuseAddressHint);//组播  必须设置复用才能多人共享
    socketRead->joinMulticastGroup(QHostAddress(shareAddress));//ip
    QObject::connect(socketRead,SIGNAL(readyRead()),this,SLOT(onReadyReadSocket()));

}

void ChatWidget::setShareAddress(const QString &value)
{
    shareAddress = value;
}

void ChatWidget::setPort(int value)
{
    port = value;
}

void ChatWidget::setWritePort(int value)
{
    writePort = value;
}

void ChatWidget::clearText()
{
    ui->tbShow->clear();
}

void ChatWidget::addAction()
{
    menu->addAction(new QAction("禁言",menu));
    menu->addAction(new QAction("允许聊天",menu));
    menu->addAction(new QAction("强行请出",menu));
}

void ChatWidget::cancelAction()
{
    menu->clear();
}




void ChatWidget::closeEvent(QCloseEvent *event)
{
    event->ignore();//禁用关闭
}

void ChatWidget::on_btnSend_clicked()
{
    QString message = ui->teInput->toPlainText();
    if(message.isEmpty())  return;
    ui->teInput->clear();
    ChatHandler ch;
    ch.groupChat(this->windowTitle(),ui->lbUsername->text(),message,ui->lbCount->text().toInt());
}

void ChatWidget::on_btnCharge_clicked()
{
    QString username = ui->lbUsername->text();
    GiftHandler gh;
    gh.rechargeWindow(username);
}

void ChatWidget::on_btnFlower_clicked()
{
    QString roomName = this->windowTitle();
    QString username = ui->lbUsername->text();

    if(roomName == username){
        QMessageBox::warning(nullptr,"送礼信息","不能给自己送礼");
        return;
    }

    if(ui->lbCount->text().toInt() < 100){
        QMessageBox::warning(nullptr,"送礼信息","余额不足");
        return;
    }
    int ret = QMessageBox::question(nullptr,"送礼信息","您确定赠送鲜花吗？",QMessageBox::Yes|QMessageBox::No,QMessageBox::No);
    if(ret == QMessageBox::Yes){
        GiftHandler gh;
        gh.sendGift(roomName,username,"flower");
    }
}

void ChatWidget::on_btnPlane_clicked()
{
    QString roomName = this->windowTitle();
    QString username = ui->lbUsername->text();

    if(roomName == username){
        QMessageBox::warning(nullptr,"送礼信息","不能给自己送礼");
        return;
    }

    if(ui->lbBalance->text().toInt() < 100){
        QMessageBox::warning(nullptr,"送礼信息","余额不足");
        return;
    }
    int ret = QMessageBox::question(nullptr,"送礼信息","您确定要赠送飞机吗？",QMessageBox::Yes|QMessageBox::No,QMessageBox::No);
    if(ret == QMessageBox::Yes){
        GiftHandler gh;
        gh.sendGift(roomName,username,"plane");
    }
}

void ChatWidget::on_btnRocket_clicked()
{
    QString roomName = this->windowTitle();
    QString username = ui->lbUsername->text();

    if(roomName == username){
        QMessageBox::warning(nullptr,"送礼信息","不能给自己送礼");
        return;
    }

    if(ui->lbBalance->text().toInt() < 500){
        QMessageBox::warning(nullptr,"送礼信息","余额不足");
        return;
    }
    int ret = QMessageBox::question(nullptr,"送礼信息","您确定要赠送火箭吗？",QMessageBox::Yes|QMessageBox::No,QMessageBox::No);
    if(ret == QMessageBox::Yes){
        GiftHandler gh;
        gh.sendGift(roomName,username,"rocket");
    }
}

void ChatWidget::on_rbtnShowChat_clicked()
{
    if(ui->rbtnShowChat->isChecked()){
        ui->lbVideo->hide();//聊天模式
    }else {
        ui->lbVideo->show();//普通模式
    }
}

void ChatWidget::on_btnHall_clicked()
{
    //定义指针的话需要动态分配  或者赋初始值

    if(socketRead != nullptr){
        //socketRead->leaveMulticastGroup(QHostAddress(shareAddress));
        delete socketRead;
        socketRead = nullptr;
    }
    if(socketAudioRead != nullptr){
        //socketAudioRead->leaveMulticastGroup(QHostAddress(shareAddress));
        delete  socketAudioRead;
        socketAudioRead = nullptr;
    }
//    audioInput->stop();
//    if(audioOutput != nullptr)  audioOutput->stop();
    if(ui->cbCamera->isChecked()){
        ui->cbCamera->setCheckState(Qt::Unchecked);
        on_cbCamera_clicked();
        ui->lbVideo->clear();
    }

    if(ui->cbVideo->isChecked()){
        ui->cbVideo->setCheckState(Qt::Unchecked);
        on_cbVideo_clicked();
        ui->lbVideo->clear();
    }

    if(ui->cbSound->isChecked()) {
        ui->cbSound->setCheckState(Qt::Unchecked);
        on_cbSound_clicked();
    }


    RoomHandler rh;
    rh.quitRoom(this->windowTitle(),this->ui->lbUsername->text());
}


void ChatWidget::on_cbBarrage_clicked()
{
    int state = ui->cbBarrage->isChecked();
    if(state){
        screenManager->openBarrage();
    }else {
        screenManager->closeBarrage();
    }
}

void ChatWidget::on_cbCamera_clicked()
{
    if(ui->lbUsername->text() != this->windowTitle())
    {
        QMessageBox::warning(nullptr,"打开摄像头","只有主播才能打开摄像头");
        ui->cbCamera->setChecked(false);
        return;
    }

    if(ui->cbCamera->isChecked()){
        camera->start();
    }else {
        camera->stop();
        //cameraViewFinder->close();
        ui->lbVideo->clear();
    }
}

void ChatWidget::on_cbVideo_clicked()
{
    if(ui->lbUsername->text() == this->windowTitle()){
        QMessageBox::warning(nullptr,"观看直播","请专心直播");
        ui->cbVideo->setChecked(false);
        return;
    }else{

        if(ui->cbVideo->isChecked()){
            connectUdp();
        }else {
            if(socketRead == nullptr) return;
            delete socketRead;
            socketRead = nullptr;
            ui->lbVideo->clear();//清空屏幕
            if(timerId != 100){
                killTimer(timerId);
            }
        }
    }
}



void ChatWidget::videoChangedSlot(QVideoFrame curFrame)
{
    static int i = 0;
    i++;
    int unitBytes = 4096;
    QVideoFrame frame(curFrame);
    frame.map(QAbstractVideoBuffer::ReadOnly);

    //通过QVideoFrame对象构建QImage对象
    QImage image(frame.bits(),
                 frame.width(),
                 frame.height(),
                 QVideoFrame::imageFormatFromPixelFormat(frame.pixelFormat()));
    //图像旋转180
    QMatrix matrix;
    matrix.rotate(180);
    image = image.transformed(matrix);
    //QImage 转 QPixMap
    QPixmap pixmap = QPixmap::fromImage(image);
    pixmap = pixmap.scaled(ui->lbVideo->size());
    ui->lbVideo->setPixmap(pixmap);

    if(i % 4 != 0)
        return;
    int byteCount = image.byteCount();
    int width = image.width();
    int height = image.height();
    int bytePerLine = image.bytesPerLine();
    int writeBytes = 0;

    while (true)
    {
        ImagePackage pack;
        memset(&pack,0,sizeof (ImagePackage));

        pack.width = width;
        pack.height = height;
        pack.bytesPerline = bytePerLine;
        if(writeBytes < byteCount){
            memcpy(pack.data,(char*)image.bits()+writeBytes,unitBytes);
            pack.packTaken = unitBytes;
            writeBytes += unitBytes;
            if(writeBytes>=byteCount){
                pack.isLastPack = true;
                pack.packTaken = unitBytes - writeBytes + byteCount;
                qDebug()<<"writeBytes:"<<writeBytes;
            }else {
                pack.isLastPack = false;
                pack.packTaken = unitBytes;
            }
            socketWrite->writeDatagram((char*)&pack,sizeof (ImagePackage),QHostAddress("239.168.168.168"),writePort);
        }else {
            break;
        }
    }
}

void ChatWidget::onReadyReadSocket()
{
    qDebug()<<"接收视频";
    count = 0;
    static QByteArray *imageBytes = new QByteArray(2280000,0);
    static QDataStream *dataStream = new QDataStream(imageBytes,QIODevice::ReadWrite);

    while (socketRead->hasPendingDatagrams()) {
        ImagePackage pack;
        memset(&pack,0,sizeof(ImagePackage));
        socketRead->readDatagram((char*)&pack,sizeof (ImagePackage));
        if(true == pack.isLastPack){
            qDebug()<<"packTaken:"<<pack.packTaken;
            dataStream->writeRawData(pack.data,pack.packTaken);
            QImage image = QImage((uchar*)imageBytes->data(),
                                  pack.width,
                                  pack.height,
                                  pack.bytesPerline,
                                  QImage::Format_RGB32);

            QPixmap map = QPixmap::fromImage(image);
            qDebug()<<"接收的图片大小:"<<image.byteCount();
            map = map.scaled(ui->lbVideo->size());
            ui->lbVideo->setPixmap(map);
            dataStream->device()->seek(0);
        }else {
            dataStream->writeRawData(pack.data,pack.packTaken);
        }
    }
}

void ChatWidget::on_cbSound_clicked()
{
    if(ui->lbUsername->text() == windowTitle()){
        /** 主播 **/
        if(ui->cbSound->isChecked()){
            audioInputIODevice = audioInput->start();
            connect(audioInputIODevice,SIGNAL(readyRead()),this,SLOT(captureDataFromDevice()));
        }else{
            audioInput->stop();
        }
    }else {
        /** 观众 **/
        if(ui->cbSound->isChecked()){
        socketAudioRead = new QUdpSocket(this);
        socketAudioRead->bind(QHostAddress::AnyIPv4,port+1,QUdpSocket::ShareAddress|QUdpSocket::ReuseAddressHint);
        socketAudioRead->joinMulticastGroup(QHostAddress("239.168.168.168"));
        connect(socketAudioRead,SIGNAL(readyRead()),this,SLOT(onReadyReadAudio()));
        }else {
            if(socketAudioRead == nullptr) return;
            delete socketAudioRead;
            socketAudioRead = nullptr;
        }
    }
}

void ChatWidget::onReadyReadAudio()
{
    AudioPackage pack;
    memset(&pack,0,sizeof(AudioPackage));
    socketAudioRead->readDatagram((char*)&pack,sizeof(AudioPackage));
    audioOutputIODevice->write(pack.data,pack.dataLen);


}

void ChatWidget::captureDataFromDevice()
{
    AudioPackage pack;
    memset(&pack,0,sizeof(AudioPackage));
    pack.dataLen = audioInputIODevice->read(pack.data,1024);
    socketAudioWrite->writeDatagram((char*)&pack,
                                    sizeof(AudioPackage),
                                    QHostAddress("239.168.168.168"),
                                    writePort+1);
}

void ChatWidget::onCustomContextMenuRequested(const QPoint &)
{
    if(ui->lwUserList->currentRow() < 0 || ui->lwUserList->currentItem()->text() == windowTitle()) return;
    menu->exec(QCursor::pos());
}

void ChatWidget::onTriggered(QAction *action)
{//"允许聊天""强行请出",me
    RoomHandler rh;

    if(action->text() == "禁言"){
        rh.muteUser(windowTitle(),ui->lwUserList->currentItem()->text());
    }else if(action->text() == "允许聊天"){
        rh.enableUser(windowTitle(),ui->lwUserList->currentItem()->text());
    }else if (action->text() == "强行请出") {
        rh.kickUser(windowTitle(),ui->lwUserList->currentItem()->text());
    }
}

void ChatWidget::timerEvent(QTimerEvent *event)
{
    count++;
    if(count > 3){
        ui->lbVideo->clear();
    }
}
