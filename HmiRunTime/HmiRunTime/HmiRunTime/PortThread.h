#ifndef PORTTHREAD_H
#define PORTTHREAD_H

#include <QObject>
#include <QQueue>
#include <QList>
#include <QThread>
#include <QSemaphore>
#include "Vendor.h"

enum __ThreadState
{
	__Thread_Stop,
	__Thread_Runing,
};

class PortThread : public QThread
{
    Q_OBJECT
public:
    PortThread(QString name);
    ~PortThread();
    void AddVendor(Vendor *pVendor);
    // 是否运行
    bool IsRunning();
    // 启动
    void Start();
    // 停止
    void Stop();
	__ThreadState state;
protected:
    void run();

private:
    bool mbIsRunning;
    QList<Vendor *> m_VendorList;
    QString m_name;
};

#endif // PORTTHREAD_H
