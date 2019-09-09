#ifndef TIMEELAPSED_H
#define TIMEELAPSED_H

#include <QString>
#include <QTime>

class TimeElapsed
{
public:
    TimeElapsed(const QString &info);
    ~TimeElapsed();

private:
    QString m_szInfo;
    QTime m_time;
};

#endif // TIMEELAPSED_H
