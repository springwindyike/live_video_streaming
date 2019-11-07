#include "rechargewidget.h"
#include "ui_rechargewidget.h"
#include "handler/gifthandler.h"

RechargeWidget::RechargeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RechargeWidget)
{
    ui->setupUi(this);
}

RechargeWidget::~RechargeWidget()
{
    delete ui;
}

void RechargeWidget::setUsername(const QString &username)
{
    this->ui->leUsername->setText(username);
}

void RechargeWidget::on_btnRecharge_clicked()
{//充值
    double money = ui->cbMoney->currentText().toDouble();
    QString username = ui->leUsername->text();
    GiftHandler gh;
    gh.recharge(username,money);
    this->close();
}

void RechargeWidget::on_btnCancel_clicked()
{
    this->close();
}
