#ifndef MEMORYMESSAGESERVICE_H
#define MEMORYMESSAGESERVICE_H

#include <QThread>
#include <QSharedMemory>
#include <QSystemSemaphore>

class MemoryMessageService : public QThread
{
public:
    MemoryMessageService();

protected:
    void run() override;

private:
    QSharedMemory *sharedMemory_;
};

#endif // MEMORYMESSAGESERVICE_H
