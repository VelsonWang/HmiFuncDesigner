#include "TimeElapsed.h"
#include <QDebug>

TimeElapsed::TimeElapsed(const QString &info)
    : m_szInfo(info)
{
    QString szInfo = QString("start do %1")
            .arg(m_szInfo);
    qDebug() << szInfo;
    m_time.restart();
}


TimeElapsed::~TimeElapsed()
{
    QString szInfo = QString("do %1 use time: %2 ms")
            .arg(m_szInfo)
            .arg(QString::number(m_time.elapsed()));
    qDebug() << szInfo;
}


