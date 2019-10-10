#include "TCPIPModbusDevice.h"
#include "RealTimeDB.h"
#include "log4qt/logger.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QThread>

#include <QDebug>


TCPIPModbusDevice::TCPIPModbusDevice()
    : pNetDevicePrivate_(Q_NULLPTR)
{
    netPort_ = new NetPort();
    iFacePort = netPort_;
    TCPIPModbus_.setPort(iFacePort);
}

TCPIPModbusDevice::~TCPIPModbusDevice()
{
    if(netPort_ != Q_NULLPTR) {
        delete netPort_;
        netPort_ = Q_NULLPTR;
    }
    if(pNetDevicePrivate_ != Q_NULLPTR) {
        delete pNetDevicePrivate_;
        pNetDevicePrivate_ = Q_NULLPTR;
    }
}



/*
* 获取设备名称
*/
QString TCPIPModbusDevice::GetDeviceName()
{
    if(pNetDevicePrivate_ != Q_NULLPTR)
        return pNetDevicePrivate_->m_sDeviceName;
    return "";
}

/*
* 打开设备
*/
bool TCPIPModbusDevice::Open()
{
    return true;
}


/*
* 设备初始化
*/
void TCPIPModbusDevice::Initialize()
{

}


/*
* 添加设备变量至变量列表
*/
void TCPIPModbusDevice::AddIOTagToDeviceTagList(IOTag* pTag)
{
    mReadList.append(pTag);
}


/*
* 添加设备变量至变量写队列
*/
void TCPIPModbusDevice::AddIOTagToDeviceTagWriteQueue(IOTag* pTag)
{
	QMutexLocker locker(&m_WriteMutex);
    mWriteQueue.enqueue(pTag);
}

static void ClearIOTagReadBuffer(IOTag* pTag)
{
    for(int i=0; i<pTag->mLength; i++)
        pTag->pReadBuf[i] = 0;
}

static void ClearIOTagWriteBuffer(IOTag* pTag)
{
    for(int i=0; i<pTag->mLength; i++)
        pTag->pWriteBuf[i] = 0;
}


/*
* 查找设备变量
*/
IOTag* TCPIPModbusDevice::FindIOTagByID(const QString &id)
{
    for (int i = 0; i < mReadList.size(); ++i) {
        IOTag* pTag = mReadList.at(i);
        if(pTag->GetTagID() == id) {
            return pTag;
        }
    }
    return nullptr;
}

/**
 * @brief TCPIPModbusDevice::BeforeWriteIOTag
 * @details 写变量前处理
 * @param pTag 变量
 * @return
 */
bool TCPIPModbusDevice::BeforeWriteIOTag(IOTag* pTag)
{
    Q_UNUSED(pTag)
    return true;
}


/*
* 写变量
*/
bool TCPIPModbusDevice::WriteIOTag(IOTag* pTag)
{
    if(pTag->GetPermissionType() == READ_WRIE || pTag->GetPermissionType() == WRIE) {
        ClearWriteBuffer();
        ClearIOTagWriteBuffer(pTag);

        if(!TCPIPModbus_.isCanWrite(pTag))
            return false;

        BeforeWriteIOTag(pTag);

        if(TCPIPModbus_.writeData(pTag) != 1)
            return false;

        AfterWriteIOTag(pTag);
    }
    return true;
}


/**
 * @brief TCPIPModbusDevice::AfterWriteIOTag
 * @details 写变量后处理
 * @param pTag 变量
 * @return
 */
bool TCPIPModbusDevice::AfterWriteIOTag(IOTag* pTag)
{
    Q_UNUSED(pTag)
    return true;
}


/*
* 写变量列表
*/
bool TCPIPModbusDevice::WriteIOTags()
{
    while (!mWriteQueue.isEmpty()) {
        if(!mbIsRunning)
            return false;

        IOTag* pTag = mWriteQueue.dequeue();
        WriteIOTag(pTag);
    }
    return true;
}


/**
 * @brief TCPIPModbusDevice::BeforeReadIOTag
 * @details 读变量前处理
 * @param pTag 变量
 * @return
 */
bool TCPIPModbusDevice::BeforeReadIOTag(IOTag* pTag)
{
    Q_UNUSED(pTag)
    return true;
}


/*
* 读变量
*/
bool TCPIPModbusDevice::ReadIOTag(IOTag* pTag)
{
    QMutexLocker locker(&m_WriteMutex);
    if(pTag->GetPermissionType() == READ_WRIE || pTag->GetPermissionType() == READ) {
        ClearReadBuffer();
        ClearIOTagReadBuffer(pTag);

        DBTagObject *pDBTagObject = pTag->GetDBTagObject();

        if(!TCPIPModbus_.isCanRead(pTag))
            return false;

        BeforeReadIOTag(pTag);

        if(pTag->getBlockReadTagId() == "block") { // 块读取变量
            if(TCPIPModbus_.readData(pTag) != 1) {
                pTag->setReadBlockReadTagSuccess(false);
                return false;
            }
            pTag->setReadBlockReadTagSuccess(true);
        } else { // 单一变量读取操作
            IOTag* pBlockReadTag = pTag->getBlockReadTag();
            if(pBlockReadTag != Q_NULLPTR) {
                // 块读取变量读取成功, 变量直接拷贝数据, 否则直接读取单一变量
                if(pBlockReadTag->isReadBlockReadTagSuccess()) {
                    int iBlockRegAddr = pBlockReadTag->GetRegisterAddress() + pBlockReadTag->GetOffset();
                    int iBlockDataTypeLength = pBlockReadTag->GetDataTypeLength();

                    int iRegAddr = pTag->GetRegisterAddress() + pTag->GetOffset();
                    int iDataTypeLength = pTag->GetDataTypeLength();

                    TModbus_CPUMEM cm = TCPIPModbus_.getCpuMem(pTag->GetRegisterArea());
                    if(cm == CM_0x || cm == CM_1x) {
                        int iByteAddr = (iRegAddr-iBlockRegAddr)/8;
                        int iBitAddr = (iRegAddr-iBlockRegAddr)%8;
                        quint8 buf[1] = {0};
                        memcpy((void *)&buf[0], (const void *)&pBlockReadTag->pReadBuf[iByteAddr], 1);
                        pTag->pReadBuf[0] = (buf[0] >> iBitAddr) & 0x01;
                    } else if(cm == CM_3x || cm == CM_4x) {
                        int iByteAddr = (iRegAddr-iBlockRegAddr)*2;
                        memcpy((void *)&pTag->pReadBuf[0], (const void *)&pBlockReadTag->pReadBuf[iByteAddr], iDataTypeLength);
                    }

                } else {
                    if(TCPIPModbus_.readData(pTag) != 1)
                        return false;
                }
            }
            if(pDBTagObject != Q_NULLPTR)
                pDBTagObject->SetData(pTag->pReadBuf);
        }

        AfterReadIOTag(pTag);
    }
    return true;
}


/**
 * @brief TCPIPModbusDevice::AfterReadIOTag
 * @details 读变量后处理
 * @param pTag 变量
 * @return
 */
bool TCPIPModbusDevice::AfterReadIOTag(IOTag* pTag) {
    Q_UNUSED(pTag)
    return true;
}


/*
* 读变量列表
*/
bool TCPIPModbusDevice::ReadIOTags()
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

        if(pTag->getBlockReadTagId() == "block") { // 块读取变量
            if(pNetDevicePrivate_ != Q_NULLPTR) {
                if(pNetDevicePrivate_->m_iFrameTimePeriod > 0)
                    QThread::msleep(static_cast<unsigned long>(pNetDevicePrivate_->m_iFrameTimePeriod));
            }
        } else {
            IOTag* pBlockReadTag = pTag->getBlockReadTag();
            if(pBlockReadTag != Q_NULLPTR) {
                if(!pBlockReadTag->isReadBlockReadTagSuccess()) {
                    if(pNetDevicePrivate_ != Q_NULLPTR) {
                        if(pNetDevicePrivate_->m_iFrameTimePeriod > 0)
                            QThread::msleep(static_cast<unsigned long>(pNetDevicePrivate_->m_iFrameTimePeriod));
                    }
                }
            }
        }
    }
    return true;
}


/*
* 是否运行
*/
bool TCPIPModbusDevice::IsRunning()
{
    return mbIsRunning;
}


/*
* 启动
*/
void TCPIPModbusDevice::Start()
{
    // 加入块读变量至待读变量标签列表
    TCPIPModbus_.insertBlockReadTagToReadList(mReadList);
    mbIsRunning = true;
}


/*
* 停止
*/
void TCPIPModbusDevice::Stop()
{
    mbIsRunning = false;
}


/*
* 循环处理
*/
void TCPIPModbusDevice::DoLoop()
{
    WriteIOTags();
    ReadIOTags();
}


/*
* 是重启运行
*/
bool TCPIPModbusDevice::IsRestart()
{
    return true;
}


/*
* 是否在线
*/
bool TCPIPModbusDevice::IsOnLine()
{
    return (miFailCnt < 5);
}


/*
* 关闭设备
*/
bool TCPIPModbusDevice::Close()
{
    return true;
}

/*
* 获取端口名称
*/
QString TCPIPModbusDevice::GetPortName()
{   
    return QString("%1:%2").arg(pNetDevicePrivate_->m_sIpAddress).arg(pNetDevicePrivate_->m_iPort);
}


/*
* 从文件读取配置数据
*/
bool TCPIPModbusDevice::LoadData(const QString &devName)
{
    pNetDevicePrivate_ = new NetDevicePrivate();
    if (pNetDevicePrivate_->LoadData(devName))
    {
        QStringList netArgs;
        netArgs << pNetDevicePrivate_->m_sIpAddress;
        netArgs << QString().number(pNetDevicePrivate_->m_iPort);

        if(!iFacePort->open("Net", netArgs))
        {
            Log4Qt::Logger::logger("Run_Logger")->error("NetPort open fail!");
            qWarning("NetPort open fail!");
            return false;
        }
        return true;
    }
    return false;
}




