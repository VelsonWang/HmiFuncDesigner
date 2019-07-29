#include "MessageQueue.h"

MessageQueue* MessageQueue::msgQueue_ = nullptr;
QMutex MessageQueue::mutex_;


MessageQueue::MessageQueue()
{
    resSemaphore_ = new QSemaphore(1);
    msgMgr_ = new TMsgManager();
}


MessageQueue::~MessageQueue()
{
    if(resSemaphore_ != nullptr) {
        delete resSemaphore_;
        resSemaphore_ = nullptr;
    }

    if(msgMgr_ != nullptr) {
        delete msgMgr_;
        msgMgr_ = nullptr;
    }
}



MessageQueue* MessageQueue::getInstance()
{
    QMutexLocker locker(&mutex_);
    if(msgQueue_ == nullptr) {
        msgQueue_ = new MessageQueue();
        ::atexit(deleteInstance);
    }

    return msgQueue_;
}

void MessageQueue::deleteInstance()
{
    if(msgQueue_ != nullptr) {
        delete msgQueue_;
        msgQueue_ = nullptr;
    }
}

/**
 * @brief MessageQueue::registMsgQueue
 * @details 注册消息队列
 * @param queueid 消息队列ID
 * @return true false
 */
bool MessageQueue::registMsgQueue(const int& queueid)
{
    TMsgQueue queue;
    TMsgManager::iterator it;
    bool ret = false;

    resSemaphore_->acquire();

    it = msgMgr_->find(queueid);
    if(it == msgMgr_->end()) {
        msgMgr_->insert(queueid,queue);
        ret = true;
    }

    resSemaphore_->release();

    return ret;
}


/**
 * @brief MessageQueue::removeMsgQueue
 * @details 移除消息队列
 * @param queueid 消息队列ID
 * @return true false
 */
bool MessageQueue::removeMsgQueue(const int& queueid)
{
    TMsgManager::iterator it;
    bool ret = false;

    resSemaphore_->acquire();

    it = msgMgr_->find(queueid);
    if(it != msgMgr_->end()) {
        msgMgr_->erase(it);
        ret = true;
    }

    resSemaphore_->release();

    return ret;
}


/**
 * @brief MessageQueue::sendMsg
 * @details 发送消息
 * @param queueid 消息队列ID
 * @param data 消息体
 * @return true false
 */
bool MessageQueue::sendMsg(const int& queueid, const TMsgData& data)
{
    TMsgManager::iterator it;
    bool ret = false;

    resSemaphore_->acquire();

    it = msgMgr_->find(queueid);
    if(it != msgMgr_->end()) {
        it.value().enqueue(data);
        ret = true;
    }

    resSemaphore_->release();

    return ret;
}


/**
 * @brief MessageQueue::readMsg
 * @details 读取消息队列消息
 * @param queueid 消息队列ID
 * @param data 消息体
 * @return true false
 */
bool MessageQueue::readMsg(const int& queueid, TMsgData& data)
{
    TMsgManager::iterator it;
    bool ret = false;

    resSemaphore_->acquire();

    it = msgMgr_->find(queueid);
    if(it != msgMgr_->end())
    {
        if(it.value().size() > 0) {
            data = it.value().dequeue();
        } else {
            ret = false;
        }
    } else {
        ret = false;
    }

    resSemaphore_->release();

    return ret;
}




