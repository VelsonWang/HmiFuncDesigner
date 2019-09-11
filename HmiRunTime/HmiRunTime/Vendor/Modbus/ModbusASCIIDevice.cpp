#include "ModbusASCIIDevice.h"
#include "RealTimeDB.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QThread>
#include <QDebug>

ModbusASCIIDevice::ModbusASCIIDevice()
    : pComDevicePrivate(Q_NULLPTR) {
    comPort_ = new ComPort();
    iFacePort = comPort_;
    modbusAscii_.setPort(iFacePort);
}

ModbusASCIIDevice::~ModbusASCIIDevice()
{  
    if(comPort_ != nullptr)
    {
        delete comPort_;
        comPort_ = nullptr;
    }

    if(pComDevicePrivate != Q_NULLPTR)
    {
        delete pComDevicePrivate;
        pComDevicePrivate = Q_NULLPTR;
    }
}



/*
* 获取设备名称
*/
QString ModbusASCIIDevice::GetDeviceName()
{
    if(pComDevicePrivate != Q_NULLPTR)
        return pComDevicePrivate->m_sDeviceName;
    return "";
}

/*
* 打开设备
*/
bool ModbusASCIIDevice::Open()
{
    return true;
}


/*
* 设备初始化
*/
void ModbusASCIIDevice::Initialize()
{

}


/*
* 添加设备变量至变量列表
*/
void ModbusASCIIDevice::AddIOTagToDeviceTagList(IOTag* pTag)
{
    mReadList.append(pTag);
}


/*
* 添加设备变量至变量写队列
*/
void ModbusASCIIDevice::AddIOTagToDeviceTagWriteQueue(IOTag* pTag)
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
IOTag* ModbusASCIIDevice::FindIOTagByID(const QString &id)
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
 * @brief ModbusASCIIDevice::BeforeWriteIOTag
 * @details 写变量前处理
 * @param pTag 变量
 * @return
 */
bool ModbusASCIIDevice::BeforeWriteIOTag(IOTag* pTag) {
    Q_UNUSED(pTag)
    return true;
}

/*
* 写变量
*/
bool ModbusASCIIDevice::WriteIOTag(IOTag* pTag)
{
    if(pTag->GetPermissionType() == READ_WRIE || pTag->GetPermissionType() == WRIE) {
        ClearWriteBuffer();
        ClearIOTagWriteBuffer(pTag);

        if(!modbusAscii_.isCanWrite(pTag))
            return false;

        BeforeWriteIOTag(pTag);

        if(modbusAscii_.writeData(pTag) != 1)
            return false;

        AfterWriteIOTag(pTag);
    }
    return true;
}

/**
 * @brief ModbusASCIIDevice::AfterWriteIOTag
 * @details 写变量后处理
 * @param pTag 变量
 * @return
 */
bool ModbusASCIIDevice::AfterWriteIOTag(IOTag* pTag) {
    Q_UNUSED(pTag)
    return true;
}

/*
* 写变量列表
*/
bool ModbusASCIIDevice::WriteIOTags()
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
 * @brief ModbusASCIIDevice::BeforeReadIOTag
 * @details 读变量前处理
 * @param pTag 变量
 * @return
 */
bool ModbusASCIIDevice::BeforeReadIOTag(IOTag* pTag) {
    Q_UNUSED(pTag)
    return true;
}

/*
* 读变量
*/
bool ModbusASCIIDevice::ReadIOTag(IOTag* pTag)
{
	QMutexLocker locker(&m_WriteMutex);
    if(pTag->GetPermissionType() == READ_WRIE || pTag->GetPermissionType() == READ) {
        ClearReadBuffer();
        ClearIOTagReadBuffer(pTag);

        DBTagObject *pDBTagObject = pTag->GetDBTagObject();

        if(!modbusAscii_.isCanRead(pTag))
            return false;

        BeforeReadIOTag(pTag);

        if(modbusAscii_.readData(pTag) != 1)
            return false;

        pDBTagObject->SetData(pTag->pReadBuf);

        AfterReadIOTag(pTag);
    }
    return true;
}

/**
 * @brief ModbusASCIIDevice::AfterReadIOTag
 * @details 读变量后处理
 * @param pTag 变量
 * @return
 */
bool ModbusASCIIDevice::AfterReadIOTag(IOTag* pTag) {
    Q_UNUSED(pTag)
    return true;
}

/*
* 读变量列表
*/
bool ModbusASCIIDevice::ReadIOTags()
{
    for (int i = 0; i < mReadList.size(); ++i) {
        if(!mWriteQueue.isEmpty())
            break;

        if(!mbIsRunning)
            return false;

        IOTag* pTag = mReadList.at(i);

        if(ReadIOTag(pTag))
        {
            miFailCnt = 0;
        }
        else
        {
            miFailCnt++;
        }

        if(pComDevicePrivate != Q_NULLPTR)
        {
            if(pComDevicePrivate->m_iFrameTimePeriod > 0)
                QThread::msleep(pComDevicePrivate->m_iFrameTimePeriod);
        }
    }
    return true;
}


/*
* 是否运行
*/
bool ModbusASCIIDevice::IsRunning()
{
    return mbIsRunning;
}


/*
* 启动
*/
void ModbusASCIIDevice::Start()
{
    mbIsRunning = true;
}


/*
* 停止
*/
void ModbusASCIIDevice::Stop()
{
    mbIsRunning = false;
}


/*
* 循环处理
*/
void ModbusASCIIDevice::DoLoop()
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
bool ModbusASCIIDevice::IsRestart()
{
    return true;
}


/*
* 是否在线
*/
bool ModbusASCIIDevice::IsOnLine()
{
    return (miFailCnt < 5);
}


/*
* 关闭设备
*/
bool ModbusASCIIDevice::Close()
{
    return true;
}

/*
* 获取端口名称
*/
QString ModbusASCIIDevice::GetPortName()
{
    return pComDevicePrivate->m_sPortNumber;
}


/*
* 从文件读取配置数据
*/
bool ModbusASCIIDevice::LoadData(const QString &devName)
{
    pComDevicePrivate = new ComDevicePrivate();
    if (pComDevicePrivate->LoadData(devName))
    {
        QStringList comArgs;
        comArgs << QString().number(pComDevicePrivate->m_iBaudrate);
        comArgs << QString().number(pComDevicePrivate->m_iDatabit);
        comArgs << pComDevicePrivate->m_sVerifybit;
        comArgs << QString().number(pComDevicePrivate->m_fStopbit);

        if(!iFacePort->open(pComDevicePrivate->m_sPortNumber, comArgs))
        {
            qWarning("ComPort open fail!");
            return false;
        }
        return true;
    }
    return false;
}




