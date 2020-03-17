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
    if(timer) {
         if(timer->isActive()) timer->stop();
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
    TAny year;
    year.t_uint16 = static_cast<quint16>(date.year());
    RealTimeDB::instance()->SetTypeAndData(szTagID, TYPE_UINT16, year);

    // 系统变量--月
    szTagID = "sys.2";
    TAny month;
    month.t_uint16 = static_cast<quint16>(date.month());
    RealTimeDB::instance()->SetTypeAndData(szTagID, TYPE_UINT16, month);

    // 系统变量--日
    szTagID = "sys.3";
    TAny day;
    day.t_uint16 = static_cast<quint16>(date.day());
    RealTimeDB::instance()->SetTypeAndData(szTagID, TYPE_UINT16, day);

    // 系统变量--时
    szTagID = "sys.4";
    TAny hour;
    hour.t_uint16 = static_cast<quint16>(time.hour());
    RealTimeDB::instance()->SetTypeAndData(szTagID, TYPE_UINT16, hour);

    // 系统变量--分
    szTagID = "sys.5";
    TAny minute;
    minute.t_uint16 = static_cast<quint16>(time.minute());
    RealTimeDB::instance()->SetTypeAndData(szTagID, TYPE_UINT16, minute);

    // 系统变量--秒
    szTagID = "sys.6";
    TAny second;
    second.t_uint16 = static_cast<quint16>(time.second());
    RealTimeDB::instance()->SetTypeAndData(szTagID, TYPE_UINT16, second);
}
