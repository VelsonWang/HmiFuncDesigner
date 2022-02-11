#ifndef MESSAGEQUEUE_H
#define MESSAGEQUEUE_H

#include <QByteArray>
#include <QMap>
#include <QMutex>
#include <QMutexLocker>
#include <QQueue>
#include <QSemaphore>
#include <stdlib.h>

// 消息体
typedef struct tagMsgData
{
    int msgid;
    QByteArray data;
}TMsgData;

// 消息队列
typedef QQueue<TMsgData> TMsgQueue;

// 消息队列容器
typedef QMap<int, TMsgQueue> TMsgManager;



class MessageQueue
{
public:
    static MessageQueue* getInstance();
    static void deleteInstance();

    bool registMsgQueue(const int& queueid);
    bool removeMsgQueue(const int& queueid);

    bool sendMsg(const int& queueid, const TMsgData& data);
    bool readMsg(const int& queueid, TMsgData& data);

private:
    MessageQueue();
    virtual ~MessageQueue();
    MessageQueue(const MessageQueue &) = delete;
    MessageQueue &operator=(const MessageQueue &) = delete;

private:
    static MessageQueue* msgQueue_;
    static QMutex mutex_;
    QSemaphore* resSemaphore_;
    TMsgManager*  msgMgr_;
};


#endif // MESSAGEQUEUE_H
