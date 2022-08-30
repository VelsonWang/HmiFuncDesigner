#include "TimerTask.h"
#include <QTime>
#include "realtimedb.h"
#include "publicfunction.h"
#include <QDebug>


TimerTask::TimerTask(QObject *parent) : QObject(parent)
{
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(timerUpdate()));
    timer->start(1000);
}

TimerTask::~TimerTask()
{
    if(timer) {
        if(timer->isActive()) {
            timer->stop();
        }
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
    quint64 id = 0;

    // 系统变量--年
    id = RealTimeDB::instance()->tagId("year");
    quint8 buffer[2];
    memset((void *)buffer, 0, 2);
    uint16ToBytes(date.year(), buffer);
    RealTimeDB::instance()->setTagData(id, buffer, 2);

    // 系统变量--月
    id = RealTimeDB::instance()->tagId("month");
    memset((void *)buffer, 0, 2);
    uint16ToBytes(date.month(), buffer);
    RealTimeDB::instance()->setTagData(id, buffer, 2);

    // 系统变量--日
    id = RealTimeDB::instance()->tagId("day");
    memset((void *)buffer, 0, 2);
    uint16ToBytes(date.day(), buffer);
    RealTimeDB::instance()->setTagData(id, buffer, 2);

    // 系统变量--时
    id = RealTimeDB::instance()->tagId("hour");
    memset((void *)buffer, 0, 2);
    uint16ToBytes(time.hour(), buffer);
    RealTimeDB::instance()->setTagData(id, buffer, 2);

    // 系统变量--分
    id = RealTimeDB::instance()->tagId("minute");
    memset((void *)buffer, 0, 2);
    uint16ToBytes(time.minute(), buffer);
    RealTimeDB::instance()->setTagData(id, buffer, 2);

    // 系统变量--秒
    id = RealTimeDB::instance()->tagId("second");
    memset((void *)buffer, 0, 2);
    uint16ToBytes(time.second(), buffer);
    RealTimeDB::instance()->setTagData(id, buffer, 2);
}
