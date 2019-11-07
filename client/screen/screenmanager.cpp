#include "screenmanager.h"
#include <QImage>
#include <QMessageBox>
#include <QTimerEvent>
#include <QDebug>

ScreenManager::ScreenManager(QObject *parent) : QObject(parent)
{
    //一个水平移动一个垂直移动
    h_TimerID = startTimer(50);
    v_TimerID = startTimer(300);
}

void ScreenManager::addLabel(QLabel* label)
{
    labels.append(label);
}

void ScreenManager::removeLabel(QLabel *label)
{
    labels.removeOne(label);
}

void ScreenManager::moveLabels(int distance)
{
    QPoint point;
    int x;
    int y;

    /**  超过屏幕范围删除label **/
    for (int i = 0;i < labels.size();i++) {
        QLabel* label = labels[i];
        point = label->pos();
        x = point.x() - distance;
        y = point.y();
        label->move(x,y);
//        qDebug()<<"moveLabel "<<label->text();
        if(x < -(label->width())){
            removeLabel(label);
            label->deleteLater();
//            qDebug()<<"removeLabel ";
        }
    }


}

void ScreenManager::moveRockets(int distance)
{
    QPoint point;
    int x;
    int y;

    /**  超过屏幕范围删除label **/
    for (int i = 0;i < rockets.size();i++) {
        QLabel* rocket = rockets[i];
        point = rocket->pos();
        x = point.x();
        y = point.y() - distance;
        rocket->move(x,y);
//        qDebug()<<"moveRocket "<<rocket->text();
        if(y < -(rocket->height())){
            rockets.removeOne(rocket);
            rocket->deleteLater();
//            qDebug()<<"removeRocket ";
        }
    }


}

void ScreenManager::turnOn()//显示弹幕可见
{   
    for (int i = 0;i < labels.size();i++) {
        labels[i]->show();
    }
}

void ScreenManager::turnDown()
{
    for (int i = 0;i < labels.size();i++) {
        labels[i]->hide();
    }
}

void ScreenManager::openBarrage()//开启弹幕
{
    barrageTimerID = startTimer(500);
}

void ScreenManager::closeBarrage()//关闭弹幕
{
    //关闭计时器，删除所有label
    killTimer(barrageTimerID);
    for (int i = 0; i < labels.size();i++) {
        delete labels[i];
    }
    labels.clear();
}

void ScreenManager::giftShow(QLabel *label, const QString &kind)
{//150 60  80  120

    if(kind == "plane"){
        QString fileName = ":/image/plane.png";
        QPixmap pixMap = QPixmap(fileName);
        QPixmap scaledPixMap = pixMap.scaled(150,60,Qt::KeepAspectRatio);//自动缩放
        label->setPixmap(scaledPixMap);
        label->show();
        labels.append(label);
    }else if (kind == "rocket") {
        QString fileName = ":/image/rocket.png";
        QPixmap pixMap = QPixmap(fileName);
        QPixmap scaledPixMap = pixMap.scaled(80,120,Qt::KeepAspectRatio);
        label->setPixmap(scaledPixMap);
        label->show();
        rockets.append(label);
}

}



int ScreenManager::getH_TimerID() const
{
    return h_TimerID;
}

int ScreenManager::getV_TimerID() const
{
    return v_TimerID;
}

int ScreenManager::getBarrageTimerID() const
{
    return barrageTimerID;
}

void ScreenManager::timerEvent(QTimerEvent *event)
{
    int id = event->timerId();
    if(id == h_TimerID){
        if(labels.size() != 0){
            moveLabels(2);
        }
    }else if (id == v_TimerID) {
        if(!rockets.isEmpty()){
            moveRockets(2);
        }
    }else if (id == barrageTimerID) {
        turnOn();
    }
}
