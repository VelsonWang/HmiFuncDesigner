#include "MemoryMessageService.h"
#include "MessageCmd.h"
#include "Log.h"
#include "RealTimeDB.h"
#include <QBuffer>
#include <QDataStream>
#include <QMap>
#include <QDebug>


MemoryMessageService::MemoryMessageService() {
    sharedMemory_ = new QSharedMemory(this);
    sharedMemory_->setKey("HmiRunTime.MemoryMessageService");
}

void MemoryMessageService::run() {
    QSystemSemaphore semDo("HmiRunTime.MemoryMessageService.Semaphore.do", 0, QSystemSemaphore::Create);
    QSystemSemaphore semDone("HmiRunTime.MemoryMessageService.Semaphore.done", 0, QSystemSemaphore::Create);

    int size = 8 * 1024;

    // 创建共享内存段
    if (!sharedMemory_->create(size)) {
        LogError(QString("%1 Unable to create shared memory segment.").arg(sharedMemory_->errorString()));
        return;
    }

    if(!sharedMemory_->isAttached()) {
        if (!sharedMemory_->attach()) {
            LogError("Unable to attach to shared memory segment.");
            return;
        }
    }

    while(true) {
        semDo.acquire();

        QBuffer buffer;
        QDataStream in(&buffer);

        sharedMemory_->lock();
        buffer.setData((char*)sharedMemory_->constData(), sharedMemory_->size());
        buffer.open(QBuffer::ReadWrite);

        qint32 iOptCode = 0;
        in >> iOptCode;
        QMap<QString, QString> dat;
        in >> dat;

        if(iOptCode == MSG_CMD_WRITE_TAG) { // write data to tag
            // 写数据至RTDB
            QList<QString> listId = dat.keys();
            foreach(QString szId, listId) {
                QString szVal = dat[szId];
                if(szVal.isEmpty()) {
                    continue;
                }
                RealTimeDB::SetDataString(szId, szVal);
            }
            sharedMemory_->unlock();
            //qDebug() << "iOptCode: " << iOptCode << " szDat:" << dat;
        } else if(iOptCode == MSG_CMD_READ_TAG) { // read tag data
            sharedMemory_->unlock();

            QBuffer outBuffer;
            outBuffer.open(QBuffer::ReadWrite);
            QDataStream out(&outBuffer);

            iOptCode = MSG_CMD_READ_TAG;
            out << iOptCode;

            // 获取RTDB数据
            QList<QString> listId = dat.keys();
            foreach(QString szId, listId) {
                QString szVal = RealTimeDB::GetDataString(szId);
                dat[szId] = szVal;
            }
            out << dat;

            sharedMemory_->lock();
            char *to = (char*)sharedMemory_->data();
            const char *from = outBuffer.data().data();
            size = outBuffer.size();
            memcpy(to, from, qMin(sharedMemory_->size(), size));
            sharedMemory_->unlock();
            //qDebug() << "iOptCode: " << iOptCode << " szDat:" << dat;
            semDone.release(1);
        }
    }
}
