#include "S7_200Device.h"
#include "RealTimeDB.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QThread>

#include <QDebug>


S7_200Device::S7_200Device()
    : pComDevicePrivate(Q_NULLPTR) {
    comPort_ = new ComPort();
    iFacePort = comPort_;
    ppi_.setPort(iFacePort);
}

S7_200Device::~S7_200Device() {
    if(comPort_ != Q_NULLPTR) {
        delete comPort_;
        comPort_ = Q_NULLPTR;
    }

    if(pComDevicePrivate != Q_NULLPTR) {
        delete pComDevicePrivate;
        pComDevicePrivate = Q_NULLPTR;
    }
}



/*
* 获取设备名称
*/
QString S7_200Device::GetDeviceName() {
    if(pComDevicePrivate != Q_NULLPTR)
        return pComDevicePrivate->m_sDeviceName;
    return "";
}

/*
* 打开设备
*/
bool S7_200Device::Open() {
    return true;
}


/*
* 设备初始化
*/
void S7_200Device::Initialize() {

}


/*
* 添加设备变量至变量列表
*/
void S7_200Device::AddIOTagToDeviceTagList(IOTag* pTag) {
    mReadList.append(pTag);
}


/*
* 添加设备变量至变量写队列
*/
void S7_200Device::AddIOTagToDeviceTagWriteQueue(IOTag* pTag) {
	QMutexLocker locker(&m_WriteMutex);
    mWriteQueue.enqueue(pTag);
}

static void ClearIOTagReadBuffer(IOTag* pTag) {
    for(int i=0; i<pTag->mLength; i++)
        pTag->pReadBuf[i] = 0;
}

static void ClearIOTagWriteBuffer(IOTag* pTag) {
    for(int i=0; i<pTag->mLength; i++)
        pTag->pWriteBuf[i] = 0;
}


/*
* 查找设备变量
*/
IOTag* S7_200Device::FindIOTagByID(const QString &id) {
    for (int i = 0; i < mReadList.size(); ++i) {
        IOTag* pTag = mReadList.at(i);
        if(pTag->GetTagID() == id)
        {
            return pTag;
        }
    }
    return nullptr;
}


/**
 * @brief S7_200Device::BeforeWriteIOTag
 * @details 写变量前处理
 * @param pTag 变量
 * @return
 */
bool S7_200Device::BeforeWriteIOTag(IOTag* pTag) {
    Q_UNUSED(pTag)
    return true;
}

/*
* 写变量
*/
bool S7_200Device::WriteIOTag(IOTag* pTag) {
    if(pTag->GetPermissionType() == READ_WRIE || pTag->GetPermissionType() == WRIE) {
        ClearWriteBuffer();
        ClearIOTagWriteBuffer(pTag);

        if(!ppi_.isCanWrite(pTag))
            return false;

        BeforeWriteIOTag(pTag);

        if(ppi_.writeData(pTag) != 1)
            return false;

        AfterWriteIOTag(pTag);
    }
    return true;
}


/**
 * @brief S7_200Device::AfterWriteIOTag
 * @details 写变量后处理
 * @param pTag 变量
 * @return
 */
bool S7_200Device::AfterWriteIOTag(IOTag* pTag) {
    Q_UNUSED(pTag)
    return true;
}


/*
* 写变量列表
*/
bool S7_200Device::WriteIOTags() {
    while (!mWriteQueue.isEmpty()) {
        if(!mbIsRunning)
            return false;

        IOTag* pTag = mWriteQueue.dequeue();
        WriteIOTag(pTag);
    }
    return true;
}


/**
 * @brief S7_200Device::BeforeReadIOTag
 * @details 读变量前处理
 * @param pTag 变量
 * @return
 */
bool S7_200Device::BeforeReadIOTag(IOTag* pTag) {
    Q_UNUSED(pTag)
    return true;
}


/*
* 读变量
*/
bool S7_200Device::ReadIOTag(IOTag* pTag) {
    QMutexLocker locker(&m_WriteMutex);
    if(pTag->GetPermissionType() == READ_WRIE || pTag->GetPermissionType() == READ) {
        ClearReadBuffer();
        ClearIOTagReadBuffer(pTag);

        DBTagObject *pDBTagObject = pTag->GetDBTagObject();

        if(!ppi_.isCanRead(pTag))
            return false;

        BeforeReadIOTag(pTag);

        if(ppi_.readData(pTag) != 1)
            return false;

        pDBTagObject->SetData(pTag->pReadBuf);

        AfterReadIOTag(pTag);
    }
    return true;
}


/**
 * @brief S7_200Device::AfterReadIOTag
 * @details 读变量后处理
 * @param pTag 变量
 * @return
 */
bool S7_200Device::AfterReadIOTag(IOTag* pTag) {
    Q_UNUSED(pTag)
    return true;
}


/*
* 读变量列表
*/
bool S7_200Device::ReadIOTags()
{
    for (int i = 0; i < mReadList.size(); ++i) {
        if(!mWriteQueue.isEmpty())
            break;

        if(!mbIsRunning)
            return false;

        IOTag* pTag = mReadList.at(i);

        if(ReadIOTag(pTag)) {
            miFailCnt = 0;
        } else {
            miFailCnt++;
        }

        if(pComDevicePrivate != Q_NULLPTR) {
            if(pComDevicePrivate->m_iFrameTimePeriod > 0)
                QThread::msleep(static_cast<unsigned long>(pComDevicePrivate->m_iFrameTimePeriod));
        }
    }
    return true;
}


/*
* 是否运行
*/
bool S7_200Device::IsRunning() {
    return mbIsRunning;
}


/*
* 启动
*/
void S7_200Device::Start() {
    mbIsRunning = true;
}


/*
* 停止
*/
void S7_200Device::Stop() {
    mbIsRunning = false;
}


/*
* 循环处理
*/
void S7_200Device::DoLoop() {
    //qDebug()<< GetDeviceName() << " is start!";
    //while(mbIsRunning)
    {
        WriteIOTags();
        ReadIOTags();
    }
}


/*
* 是重启运行
*/
bool S7_200Device::IsRestart() {
    return true;
}


/*
* 是否在线
*/
bool S7_200Device::IsOnLine() {
    return (miFailCnt < 5);
}


/*
* 关闭设备
*/
bool S7_200Device::Close() {
    return true;
}

/*
* 获取端口名称
*/
QString S7_200Device::GetPortName() {
    return pComDevicePrivate->m_sPortNumber;
}


/*
* 从文件读取配置数据
*/
bool S7_200Device::LoadData(const QString &devName) {
    pComDevicePrivate = new ComDevicePrivate();
    if (pComDevicePrivate->LoadData(devName)) {
        QStringList comArgs;
        comArgs << QString().number(pComDevicePrivate->m_iBaudrate);
        comArgs << QString().number(pComDevicePrivate->m_iDatabit);
        comArgs << pComDevicePrivate->m_sVerifybit;
        comArgs << QString().number(pComDevicePrivate->m_fStopbit);

        if(!iFacePort->open(pComDevicePrivate->m_sPortNumber, comArgs)) {
            qWarning("ComPort open fail!");
            return false;
        }
        return true;
    }
    return false;
}





