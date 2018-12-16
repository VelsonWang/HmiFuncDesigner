#include "TimerTask.h"
#include <QTime>
#include "RealTimeDB.h"
#include <QDebug>


TimerTask::TimerTask(QObject *parent) :
    QObject(parent)
{
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(timerUpdate()));
    timer->start(1000);
}

TimerTask::~TimerTask()
{
    if(timer)
    {
         if(timer->isActive())
              timer->stop();
         delete timer;
         timer = NULL;
    }
}


/*
* define in Tag.cpp
* TTagType type
* TYPE_IO
* TYPE_TMP
* TYPE_SYSTEM
*/
extern qint32 make_id(TTagType type, qint32 cfgid, qint32 ownPage);


void TimerTask::timerUpdate()
{
    QDateTime datetime = QDateTime::currentDateTime();
    QDate date = datetime.date();
    QTime time = datetime.time();
    qint32 id;
    // 系统变量--年
    id = make_id(TYPE_SYSTEM, 1, 0);
    //qDebug()<<QString::number(date.year());
    RealTimeDB::SetTypeAndData(id, TYPE_UINT16, QVariant((qint16)date.year()));

    // 系统变量--月
    id = make_id(TYPE_SYSTEM, 2, 0);
    //qDebug()<<QString::number(date.month());
    RealTimeDB::SetTypeAndData(id, TYPE_UINT16, QVariant((qint16)date.month()));

    // 系统变量--日
    id = make_id(TYPE_SYSTEM, 3, 0);
    //qDebug()<<QString::number(date.day());
    RealTimeDB::SetTypeAndData(id, TYPE_UINT16, QVariant((qint16)date.day()));

    // 系统变量--时
    id = make_id(TYPE_SYSTEM, 4, 0);
    //qDebug()<<QString::number(time.hour());
    RealTimeDB::SetTypeAndData(id, TYPE_UINT16, QVariant((qint16)time.hour()));

    // 系统变量--分
    id = make_id(TYPE_SYSTEM, 5, 0);
    //qDebug()<<QString::number(time.minute());
    RealTimeDB::SetTypeAndData(id, TYPE_UINT16, QVariant((qint16)time.minute()));

    // 系统变量--秒
    id = make_id(TYPE_SYSTEM, 6, 0);
    //qDebug()<<QString::number(time.second());
    RealTimeDB::SetTypeAndData(id, TYPE_UINT16, QVariant((qint16)time.second()));

}
