#ifndef RECHARGEWIDGET_H
#define RECHARGEWIDGET_H

#include <QWidget>

namespace Ui {
class RechargeWidget;
}

class RechargeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RechargeWidget(QWidget *parent = nullptr);
    ~RechargeWidget();
    void setUsername(const QString& username);

private slots:
    void on_btnRecharge_clicked();

    void on_btnCancel_clicked();

private:
    Ui::RechargeWidget *ui;
};

#endif // RECHARGEWIDGET_H
