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
    : pNetDevicePrivate(0)
{
    netPort_ = new NetPort();
    iFacePort = netPort_;
    TCPIPModbus_.setPort(iFacePort);
}

TCPIPModbusDevice::~TCPIPModbusDevice()
{
    if(netPort_ != nullptr)
    {
        delete netPort_;
        netPort_ = nullptr;
    }
    if(pNetDevicePrivate != 0 )
    {
        delete pNetDevicePrivate;
        pNetDevicePrivate = 0;
    }
}



/*
* 获取设备名称
*/
QString TCPIPModbusDevice::GetDeviceName()
{
    if(pNetDevicePrivate !=0 )
        return pNetDevicePrivate->m_sDeviceName;
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
 * @brief TCPIPModbusDevice::BeforeWriteIOTag
 * @details 写变量前处理
 * @param pTag 变量
 * @return
 */
bool TCPIPModbusDevice::BeforeWriteIOTag(IOTag* pTag) {
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
bool TCPIPModbusDevice::AfterWriteIOTag(IOTag* pTag) {
    Q_UNUSED(pTag)
    return true;
}


/*
* 写变量列表
*/
bool TCPIPModbusDevice::WriteIOTags()
{
    while (!mWriteQueue.isEmpty())
    {
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
bool TCPIPModbusDevice::BeforeReadIOTag(IOTag* pTag) {
    Q_UNUSED(pTag)
    return true;
}


/*
* 读变量
*/
bool TCPIPModbusDevice::ReadIOTag(IOTag* pTag) {
	QMutexLocker locker(&m_WriteMutex);
    if(pTag->GetPermissionType() == READ_WRIE || pTag->GetPermissionType() == READ) {
        ClearReadBuffer();
        ClearIOTagReadBuffer(pTag);

        DBTagObject *pDBTagObject = pTag->GetDBTagObject();

        if(!TCPIPModbus_.isCanRead(pTag))
            return false;

        BeforeReadIOTag(pTag);

        if(TCPIPModbus_.readData(pTag) != 1)
            return false;

        pDBTagObject->SetData(pTag->pReadBuf);

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

        if(pNetDevicePrivate !=0 )
        {
            if(pNetDevicePrivate->m_iFrameTimePeriod > 0)
                QThread::msleep(pNetDevicePrivate->m_iFrameTimePeriod);
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
    return QString("%1:%2").arg(pNetDevicePrivate->m_sIpAddress).arg(pNetDevicePrivate->m_iPort);
}


/*
* 从文件读取配置数据
*/
bool TCPIPModbusDevice::LoadData(const QString &devName)
{
    pNetDevicePrivate = new NetDevicePrivate();
    if (pNetDevicePrivate->LoadData(devName))
    {
        QStringList netArgs;
        netArgs << pNetDevicePrivate->m_sIpAddress;
        netArgs << QString().number(pNetDevicePrivate->m_iPort);

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




