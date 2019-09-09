#ifndef TIMERTASK_H
#define TIMERTASK_H

#include <QTimer>
#include <QObject>


class TimerTask : public QObject
{
    Q_OBJECT

public:
    explicit TimerTask(QObject *parent = 0);
    ~TimerTask();

private:
    QTimer *timer;

private slots:
    void timerUpdate();
};

#endif // TIMERTASK_H
