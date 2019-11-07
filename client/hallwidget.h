#ifndef HALLWIDGET_H
#define HALLWIDGET_H

#include <QWidget>
#include <QListWidgetItem>

namespace Ui {
class HallWidget;
}

class HallWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HallWidget(QWidget *parent = nullptr);
    ~HallWidget();

    void setUsername(const QString& username );
    void flushRoom(const QStringList& roomList);

protected:
    virtual void closeEvent(QCloseEvent *event);
    virtual void showEvent(QShowEvent *event);
private slots:
    void on_btnCreateRoom_clicked();
    void on_btnFlushRoom_clicked();

    void on_lwRoomList_itemDoubleClicked(QListWidgetItem *item);

    void on_btnLogout_clicked();

private:
    Ui::HallWidget *ui;
};

#endif // HALLWIDGET_H
