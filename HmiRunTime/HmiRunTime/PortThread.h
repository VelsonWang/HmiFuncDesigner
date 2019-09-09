#ifndef PORTTHREAD_H
#define PORTTHREAD_H

#include <QObject>
#include <QQueue>
#include <QList>
#include <QThread>
#include <QSemaphore>
#include "IVendor.h"

class PortThread : public QThread
{
    Q_OBJECT
public:
    PortThread(QString name);
    ~PortThread();
    void AddVendor(IVendor *pVendor);
    // 是否运行
    bool IsRunning();
    // 启动
    void Start();
    // 停止
    void Stop();

protected:
    void run();

private:
    bool mbIsRunning;
    QList<IVendor *> m_VendorList;
    QString m_name;
};

#endif // PORTTHREAD_H
