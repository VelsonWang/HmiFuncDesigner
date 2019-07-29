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
         timer = nullptr;
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
    QString szTagID = "";
    // 系统变量--年
    szTagID = "sys.1";
    RealTimeDB::SetTypeAndData(szTagID, TYPE_UINT16, QVariant((qint16)date.year()));

    // 系统变量--月
    szTagID = "sys.2";
    RealTimeDB::SetTypeAndData(szTagID, TYPE_UINT16, QVariant((qint16)date.month()));

    // 系统变量--日
    szTagID = "sys.3";
    RealTimeDB::SetTypeAndData(szTagID, TYPE_UINT16, QVariant((qint16)date.day()));

    // 系统变量--时
    szTagID = "sys.4";
    RealTimeDB::SetTypeAndData(szTagID, TYPE_UINT16, QVariant((qint16)time.hour()));

    // 系统变量--分
    szTagID = "sys.5";
    RealTimeDB::SetTypeAndData(szTagID, TYPE_UINT16, QVariant((qint16)time.minute()));

    // 系统变量--秒
    szTagID = "sys.6";
    RealTimeDB::SetTypeAndData(szTagID, TYPE_UINT16, QVariant((qint16)time.second()));
}
