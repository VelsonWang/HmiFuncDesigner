#include "ModbusRTUDevice.h"
#include "RealTimeDB.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QThread>

#include <QDebug>


ModbusRTUDevice::ModbusRTUDevice()
    : pComDevicePrivate_(nullptr)
{
    comPort_ = new ComPort();
    iFacePort = comPort_;
    modbusRTU_.setPort(iFacePort);
}

ModbusRTUDevice::~ModbusRTUDevice()
{
    if(comPort_ != nullptr) {
        delete comPort_;
        comPort_ = nullptr;
    }

    if(pComDevicePrivate_ != nullptr) {
        delete pComDevicePrivate_;
        pComDevicePrivate_ = nullptr;
    }
}



/*
* 获取设备名称
*/
QString ModbusRTUDevice::GetDeviceName()
{
    if(pComDevicePrivate_ != nullptr)
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
    for (int i = 0; i < mReadList.size(); ++i)
    {
        IOTag* pTag = mReadList.at(i);
        if(pTag->GetTagID() == id)
        {
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
    while (!mWriteQueue.isEmpty())
    {
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

        if(modbusRTU_.readData(pTag) != 1)
            return false;

        pDBTagObject->SetData(pTag->pReadBuf);

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
bool ModbusRTUDevice::AfterReadIOTag(IOTag* pTag) {
    Q_UNUSED(pTag)
    return true;
}


/*
* 读变量列表
*/
bool ModbusRTUDevice::ReadIOTags()
{
    for (int i = 0; i < mReadList.size(); ++i)
    {
        if(!mWriteQueue.isEmpty())
            break;

        IOTag* pTag = mReadList.at(i);

        if(ReadIOTag(pTag))
        {
            miFailCnt = 0;
        }
        else
        {
            miFailCnt++;
        }

        if(pComDevicePrivate_ !=0 )
        {
            if(pComDevicePrivate_->m_iFrameTimePeriod > 0)
                QThread::msleep(pComDevicePrivate_->m_iFrameTimePeriod);
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





