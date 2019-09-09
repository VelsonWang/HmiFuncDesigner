#include "MitsubishiDevice.h"
#include "RealTimeDB.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QThread>

#include <QDebug>


MitsubishiDevice::MitsubishiDevice()
    : pComDevicePrivate(0) {
    comPort_ = new ComPort();
    iFacePort = comPort_;
    mitsubishi_.setPort(iFacePort);
}

MitsubishiDevice::~MitsubishiDevice() {
    if(comPort_ != nullptr) {
        delete comPort_;
        comPort_ = nullptr;
    }

    if(pComDevicePrivate != 0) {
        delete pComDevicePrivate;
        pComDevicePrivate = 0;
    }
}



/*
* 获取设备名称
*/
QString MitsubishiDevice::GetDeviceName() {
    if(pComDevicePrivate !=0 )
        return pComDevicePrivate->m_sDeviceName;
    return "";
}

/*
* 打开设备
*/
bool MitsubishiDevice::Open() {
    return true;
}


/*
* 设备初始化
*/
void MitsubishiDevice::Initialize() {

}


/*
* 添加设备变量至变量列表
*/
void MitsubishiDevice::AddIOTagToDeviceTagList(IOTag* pTag) {
    mReadList.append(pTag);
}


/*
* 添加设备变量至变量写队列
*/
void MitsubishiDevice::AddIOTagToDeviceTagWriteQueue(IOTag* pTag) {
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
IOTag* MitsubishiDevice::FindIOTagByID(const QString &id) {
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
 * @brief MitsubishiDevice::BeforeWriteIOTag
 * @details 写变量前处理
 * @param pTag 变量
 * @return
 */
bool MitsubishiDevice::BeforeWriteIOTag(IOTag* pTag) {
    Q_UNUSED(pTag)
    return true;
}

/*
* 写变量
*/
bool MitsubishiDevice::WriteIOTag(IOTag* pTag) {
    if(pTag->GetPermissionType() == READ_WRIE || pTag->GetPermissionType() == WRIE) {
        ClearWriteBuffer();
        ClearIOTagWriteBuffer(pTag);

        if(!mitsubishi_.isCanWrite(pTag))
            return false;

        BeforeWriteIOTag(pTag);

        if(mitsubishi_.writeData(pTag) != 1)
            return false;

        AfterWriteIOTag(pTag);
    }
    return true;
}


/**
 * @brief MitsubishiDevice::AfterWriteIOTag
 * @details 写变量后处理
 * @param pTag 变量
 * @return
 */
bool MitsubishiDevice::AfterWriteIOTag(IOTag* pTag) {
    Q_UNUSED(pTag)
    return true;
}


/*
* 写变量列表
*/
bool MitsubishiDevice::WriteIOTags() {
    while (!mWriteQueue.isEmpty()) {
        IOTag* pTag = mWriteQueue.dequeue();
        WriteIOTag(pTag);
    }
    return true;
}


/**
 * @brief MitsubishiDevice::BeforeReadIOTag
 * @details 读变量前处理
 * @param pTag 变量
 * @return
 */
bool MitsubishiDevice::BeforeReadIOTag(IOTag* pTag) {
    Q_UNUSED(pTag)
    return true;
}


/*
* 读变量
*/
bool MitsubishiDevice::ReadIOTag(IOTag* pTag) {
    QMutexLocker locker(&m_WriteMutex);
    if(pTag->GetPermissionType() == READ_WRIE || pTag->GetPermissionType() == READ) {
        ClearReadBuffer();
        ClearIOTagReadBuffer(pTag);

        DBTagObject *pDBTagObject = pTag->GetDBTagObject();

        if(!mitsubishi_.isCanRead(pTag))
            return false;

        BeforeReadIOTag(pTag);

        if(mitsubishi_.readData(pTag) != 1)
            return false;

        pDBTagObject->SetData(pTag->pReadBuf);

        AfterReadIOTag(pTag);
    }
    return true;
}


/**
 * @brief MitsubishiDevice::AfterReadIOTag
 * @details 读变量后处理
 * @param pTag 变量
 * @return
 */
bool MitsubishiDevice::AfterReadIOTag(IOTag* pTag) {
    Q_UNUSED(pTag)
    return true;
}


/*
* 读变量列表
*/
bool MitsubishiDevice::ReadIOTags() {
    for (int i = 0; i < mReadList.size(); ++i) {
        if(!mWriteQueue.isEmpty())
            break;

        IOTag* pTag = mReadList.at(i);

        if(ReadIOTag(pTag)) {
            miFailCnt = 0;
        } else {
            miFailCnt++;
        }

        if(pComDevicePrivate != 0) {
            if(pComDevicePrivate->m_iFrameTimePeriod > 0)
                QThread::msleep(pComDevicePrivate->m_iFrameTimePeriod);
        }
    }
    return true;
}


/*
* 是否运行
*/
bool MitsubishiDevice::IsRunning() {
    return mbIsRunning;
}


/*
* 启动
*/
void MitsubishiDevice::Start() {
    mbIsRunning = true;
}


/*
* 停止
*/
void MitsubishiDevice::Stop() {
    mbIsRunning = false;
}


/*
* 循环处理
*/
void MitsubishiDevice::DoLoop() {
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
bool MitsubishiDevice::IsRestart() {
    return true;
}


/*
* 是否在线
*/
bool MitsubishiDevice::IsOnLine() {
    return (miFailCnt < 5);
}


/*
* 关闭设备
*/
bool MitsubishiDevice::Close() {
    return true;
}

/*
* 获取端口名称
*/
QString MitsubishiDevice::GetPortName() {
    return pComDevicePrivate->m_sPortNumber;
}


/*
* 从文件读取配置数据
*/
bool MitsubishiDevice::LoadData(const QString &devName) {
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





