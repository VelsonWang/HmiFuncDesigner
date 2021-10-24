#include "TimerTask.h"
#include <QTime>
#include "RealTimeDB.h"
#include "Public/PublicFunction.h"
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
    id = RealTimeDB::instance()->tagId("$year");
    RealTimeDB::instance()->setTagData(id, uint16ToBytes(date.year()));

    // 系统变量--月
    id = RealTimeDB::instance()->tagId("$month");
    RealTimeDB::instance()->setTagData(id, uint16ToBytes(date.month()));

    // 系统变量--日
    id = RealTimeDB::instance()->tagId("$day");
    RealTimeDB::instance()->setTagData(id, uint16ToBytes(date.day()));

    // 系统变量--时
    id = RealTimeDB::instance()->tagId("$hour");
    RealTimeDB::instance()->setTagData(id, uint16ToBytes(time.hour()));

    // 系统变量--分
    id = RealTimeDB::instance()->tagId("$minute");
    RealTimeDB::instance()->setTagData(id, uint16ToBytes(time.minute()));

    // 系统变量--秒
    id = RealTimeDB::instance()->tagId("$second");
    RealTimeDB::instance()->setTagData(id, uint16ToBytes(time.second()));
}
