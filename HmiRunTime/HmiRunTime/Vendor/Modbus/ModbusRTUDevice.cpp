#include "ModbusRTUDevice.h"
#include "RealTimeDB.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QThread>
#include <cstring>

#include <QDebug>


ModbusRTUDevice::ModbusRTUDevice()
    : pComDevicePrivate_(Q_NULLPTR)
{
    comPort_ = new ComPort();
    iFacePort = comPort_;
    modbusRTU_.setPort(iFacePort);
}

ModbusRTUDevice::~ModbusRTUDevice()
{
    if(comPort_ != Q_NULLPTR) {
        delete comPort_;
        comPort_ = Q_NULLPTR;
    }

    if(pComDevicePrivate_ != Q_NULLPTR) {
        delete pComDevicePrivate_;
        pComDevicePrivate_ = Q_NULLPTR;
    }
}



/*
* 获取设备名称
*/
QString ModbusRTUDevice::GetDeviceName()
{
    if(pComDevicePrivate_ != Q_NULLPTR)
        return pComDevicePrivate_->m_sDeviceName;
    return "";
}

/*
* 打开设备
*/
bool ModbusRTUDevice::Open()
{
    return true;
}


/*
* 设备初始化
*/
void ModbusRTUDevice::Initialize()
{

}


/*
* 添加设备变量至变量列表
*/
void ModbusRTUDevice::AddIOTagToDeviceTagList(IOTag* pTag)
{
    mReadList.append(pTag);
}


/*
* 添加设备变量至变量写队列
*/
void ModbusRTUDevice::AddIOTagToDeviceTagWriteQueue(IOTag* pTag)
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
IOTag* ModbusRTUDevice::FindIOTagByID(const QString &id)
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
 * @brief ModbusRTUDevice::BeforeWriteIOTag
 * @details 写变量前处理
 * @param pTag 变量
 * @return
 */
bool ModbusRTUDevice::BeforeWriteIOTag(IOTag* pTag) {
    Q_UNUSED(pTag)
    return true;
}

/*
* 写变量
*/
bool ModbusRTUDevice::WriteIOTag(IOTag* pTag)
{
    if(pTag->GetPermissionType() == READ_WRIE || pTag->GetPermissionType() == WRIE) {
        ClearWriteBuffer();
        ClearIOTagWriteBuffer(pTag);

        if(!modbusRTU_.isCanWrite(pTag))
            return false;

        BeforeWriteIOTag(pTag);

        if(modbusRTU_.writeData(pTag) != 1)
            return false;

        AfterWriteIOTag(pTag);
    }
    return true;
}

/**
 * @brief ModbusRTUDevice::AfterWriteIOTag
 * @details 写变量后处理
 * @param pTag 变量
 * @return
 */
bool ModbusRTUDevice::AfterWriteIOTag(IOTag* pTag) {
    Q_UNUSED(pTag)
    return true;
}

/*
* 写变量列表
*/
bool ModbusRTUDevice::WriteIOTags()
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
 * @brief ModbusRTUDevice::BeforeReadIOTag
 * @details 读变量前处理
 * @param pTag 变量
 * @return
 */
bool ModbusRTUDevice::BeforeReadIOTag(IOTag* pTag) {
    Q_UNUSED(pTag)
    return true;
}

/*
* 读变量
*/
bool ModbusRTUDevice::ReadIOTag(IOTag* pTag)
{
	QMutexLocker locker(&m_WriteMutex);
    if(pTag->GetPermissionType() == READ_WRIE || pTag->GetPermissionType() == READ) {
        ClearReadBuffer();
        ClearIOTagReadBuffer(pTag);

        DBTagObject *pDBTagObject = pTag->GetDBTagObject();

        if(!modbusRTU_.isCanRead(pTag))
            return false;

        BeforeReadIOTag(pTag);

        if(pTag->getBlockReadTagId() == "block") { // 块读取变量
            if(modbusRTU_.readData(pTag) != 1) {
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

                    TModbus_CPUMEM cm = modbusRTU_.getCpuMem(pTag->GetRegisterArea());
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
                    if(modbusRTU_.readData(pTag) != 1)
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
 * @brief ModbusRTUDevice::AfterReadIOTag
 * @details 读变量后处理
 * @param pTag 变量
 * @return
 */
bool ModbusRTUDevice::AfterReadIOTag(IOTag* pTag)
{
    Q_UNUSED(pTag)
    return true;
}


/*
* 读变量列表
*/
bool ModbusRTUDevice::ReadIOTags()
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
            if(pComDevicePrivate_ != Q_NULLPTR) {
                if(pComDevicePrivate_->m_iFrameTimePeriod > 0)
                    QThread::msleep(static_cast<unsigned long>(pComDevicePrivate_->m_iFrameTimePeriod));
            }
        } else {
            IOTag* pBlockReadTag = pTag->getBlockReadTag();
            if(pBlockReadTag != Q_NULLPTR) {
                if(!pBlockReadTag->isReadBlockReadTagSuccess()) {
                    if(pComDevicePrivate_ != Q_NULLPTR) {
                        if(pComDevicePrivate_->m_iFrameTimePeriod > 0)
                            QThread::msleep(static_cast<unsigned long>(pComDevicePrivate_->m_iFrameTimePeriod));
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
bool ModbusRTUDevice::IsRunning()
{
    return mbIsRunning;
}


/*
* 启动
*/
void ModbusRTUDevice::Start()
{
    // 加入块读变量至待读变量标签列表
    modbusRTU_.insertBlockReadTagToReadList(mReadList);
    mbIsRunning = true;
}


/*
* 停止
*/
void ModbusRTUDevice::Stop()
{
    mbIsRunning = false;
}


/*
* 循环处理
*/
void ModbusRTUDevice::DoLoop()
{
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
bool ModbusRTUDevice::IsRestart()
{
    return true;
}


/*
* 是否在线
*/
bool ModbusRTUDevice::IsOnLine()
{
    return (miFailCnt < 5);
}


/*
* 关闭设备
*/
bool ModbusRTUDevice::Close()
{
    return true;
}

/*
* 获取端口名称
*/
QString ModbusRTUDevice::GetPortName()
{
    return pComDevicePrivate_->m_sPortNumber;
}


/*
* 从文件读取配置数据
*/
bool ModbusRTUDevice::LoadData(const QString &devName)
{
    pComDevicePrivate_ = new ComDevicePrivate();
    if (pComDevicePrivate_->LoadData(devName))
    {
        QStringList comArgs;
        comArgs << QString().number(pComDevicePrivate_->m_iBaudrate);
        comArgs << QString().number(pComDevicePrivate_->m_iDatabit);
        comArgs << pComDevicePrivate_->m_sVerifybit;
        comArgs << QString().number(pComDevicePrivate_->m_fStopbit);

        if(!iFacePort->open(pComDevicePrivate_->m_sPortNumber, comArgs))
        {
            qWarning("ComPort open fail!");
            return false;
        }
        return true;
    }
    return false;
}



