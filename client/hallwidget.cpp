#include "hallwidget.h"
#include "ui_hallwidget.h"
#include "handler/loginhandler.h"
#include "handler/registerhandler.h"
#include "handler/roomhandler.h"
#include <QMessageBox>
#include <QCloseEvent>
#include <unistd.h>

HallWidget::HallWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HallWidget)
{
    ui->setupUi(this);
    /** 背景 **/
    QString fileName = ":/image/background1.png";
    QPixmap pixmap = QPixmap(fileName);
    QPixmap scaledPixmap = pixmap.scaled(this->size(),Qt::KeepAspectRatio);
    QPalette palette(this->palette());
    palette.setBrush(QPalette::Background,QBrush(scaledPixmap));
    this->setPalette(palette);
}

HallWidget::~HallWidget()
{
    delete ui;
}

void HallWidget::setUsername(const QString &username)
{
    ui->lbUsername->setText(username);
}

void HallWidget::flushRoom(const QStringList &roomList)
{
    ui->lwRoomList->clear();
    ui->lwRoomList->addItems(roomList);
}

void HallWidget::closeEvent(QCloseEvent *event)
{
    LoginHandler lh;
    lh.quit(ui->lbUsername->text());
    QMessageBox::information(this,"退出登录","您即将离开");
    event->accept();//本来想写成 选择退出还是留下，这里需要给服务器接收时间，不然有时候服务器收不到
//    QMessageBox::StandardButton button;
//    button = QMessageBox::information(this,"退出登录","您即将离开");
//    if(button == QMessageBox::No){
//        event->ignore();
//    }else {
//        event->accept();
//    }
}
//改写showevent
void HallWidget::showEvent(QShowEvent *)
{//每次显示大厅都刷新房间列表
    RoomHandler rh;
    rh.flushRoom();
}

void HallWidget::on_btnCreateRoom_clicked()
{
    QString username = ui->lbUsername->text();
    RoomHandler rh;
    int port = this->ui->lePort->text().toInt();
    if(port < 1024 || port > 49151){//端口是否合法
        rh.createRoom(username);//默认端口 9999
    }else {
        rh.createRoom(username,port);//自定义端口
    }
}
void HallWidget::on_btnFlushRoom_clicked()
{
    RoomHandler rh;
    rh.flushRoom();
}

void HallWidget::on_lwRoomList_itemDoubleClicked(QListWidgetItem *item)
{
    QString roomName = item->text();
    QString username = ui->lbUsername->text();

    RoomHandler rh;
    rh.joinRoom(roomName,username);
}

void HallWidget::on_btnLogout_clicked()
{
    int confirm = QMessageBox::question(nullptr,"注销信息","您确定要注销吗？",QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
    if(confirm == QMessageBox::Yes){
        RegisterHandler rh;
        rh.logout(ui->lbUsername->text());
    }
}
