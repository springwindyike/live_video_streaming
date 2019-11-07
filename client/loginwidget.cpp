#include "loginwidget.h"
#include "ui_loginwidget.h"
#include "handler/loginhandler.h"
#include "handler/registerhandler.h"
#include <QImage>
#include <QPalette>

LoginWidget::LoginWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginWidget)
{
    ui->setupUi(this);
    /** 设置背景图 **/
    QString fileName = ":/image/login2.png";
    QPixmap pixmap = QPixmap(fileName);
    QPixmap scaledPixmap = pixmap.scaled(this->size(),Qt::KeepAspectRatio);
    QPalette palette(this->palette());
    palette.setBrush(QPalette::Background,QBrush(scaledPixmap));
    this->setPalette(palette);
//    this->setStyleSheet("image: url(:/image/login2.png);");
}

LoginWidget::~LoginWidget()
{
    delete ui;
}

QString LoginWidget::getLastUsername()
{
    return this->username;
}

void LoginWidget::on_btnRegister_clicked()
{
    QString username = ui->leUsername->text();
    QString password = ui->lePassword->text();
    /** 如果用户名或者密码为空 返回**/
    if(username.isEmpty() || password.isEmpty())  return;

    ui->leUsername->clear();
    ui->lePassword->clear();

    /** 处理注册 **/
    RegisterHandler rh;
    rh.regist(username,password);
}

void LoginWidget::on_btnLogin_clicked()
{
    QString username = ui->leUsername->text();
    QString password = ui->lePassword->text();

    /** 如果用户名或密码为空 返回**/
    if(username.isEmpty() || password.isEmpty())  return;

    ui->leUsername->clear();
    ui->lePassword->clear();

    /** 处理登录 **/
    LoginHandler lh;
    lh.login(username,password);

    this->username = username;
}
