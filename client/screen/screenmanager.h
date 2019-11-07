#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include <QObject>
#include <QLabel>
#include <QVector>

class ScreenManager : public QObject
{
    Q_OBJECT
public:
    explicit ScreenManager(QObject *parent = nullptr);
    void addLabel(QLabel* label);
    void removeLabel(QLabel* label);
    void moveLabels(int distance);
    void moveRockets(int distance);
    void turnOn();
    void turnDown();
    void openBarrage();
    void closeBarrage();

    void giftShow(QLabel* label,const QString& kind);

    int getH_TimerID() const;
    int getV_TimerID() const;
    int getBarrageTimerID() const;

signals:

public slots:

private:
    QVector<QLabel*> labels;//只能存放指针，防止调用拷贝构造函数
    QVector<QLabel*> rockets;
    int h_TimerID;
    int v_TimerID;
    int barrageTimerID;//弹幕显示计时器
    // QObject interface
protected:
    void timerEvent(QTimerEvent *event);
};

#endif // SCREENMANAGER_H
