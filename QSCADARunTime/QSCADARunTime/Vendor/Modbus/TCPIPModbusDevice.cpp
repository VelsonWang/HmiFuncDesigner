#include "TCPIPModbusDevice.h"
#include "NetPort.h"
#include "RealTimeDB.h"
#include "log4qt/logger.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QThread>

#include <QDebug>

#define BUFFER_SIZE    (256)
static unsigned char readBuf[BUFFER_SIZE];
static unsigned char writeBuf[BUFFER_SIZE];

static void ClearReadBuffer()
{
    for(int i=0; i<BUFFER_SIZE; i++)
        readBuf[i] = 0;
}

static void ClearWriteBuffer()
{
    for(int i=0; i<BUFFER_SIZE; i++)
        writeBuf[i] = 0;
}


TCPIPModbusDevice::TCPIPModbusDevice()
    : iFacePort(0),
      pNetDevicePrivate(0),
      miFailCnt(0)
{
    mReadList.clear();
    mWriteQueue.clear();
    NetPort *pNetPort = new NetPort();
    iFacePort = pNetPort;
    mTCPIPModbus.SetPort(iFacePort);
    mbIsRunning = false;
}

TCPIPModbusDevice::~TCPIPModbusDevice()
{
    Stop();

    qDeleteAll(mReadList);
    mReadList.clear();

    if(!mWriteQueue.isEmpty())
    {
        qDeleteAll(mWriteQueue);
        mWriteQueue.clear();
    }

    if(iFacePort != 0 )
    {
        delete iFacePort;
        iFacePort = 0;
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
    m_WriteMutex.lock();
    mWriteQueue.enqueue(pTag);
    m_WriteMutex.unlock();
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
IOTag* TCPIPModbusDevice::FindIOTagByID(qint32 id)
{
    for (int i = 0; i < mReadList.size(); ++i)
    {
        IOTag* pTag = mReadList.at(i);
        if(pTag->GetTagID() == id)
        {
            return pTag;
        }
    }
    return NULL;
}


/*
* 写变量
*/
bool TCPIPModbusDevice::WriteIOTag(IOTag* pTag)
{
    if(pTag->GetPermissionType() == READ_WRIE || pTag->GetPermissionType() == WRIE)
    {
        ClearWriteBuffer();
        ClearIOTagWriteBuffer(pTag);

        QString strRegisterArea = pTag->GetRegisterArea();
        int iDevAddress = pTag->GetDeviceAddress();
        int iRegisterAddress = pTag->GetRegisterAddress();
        int iOffset = pTag->GetOffset();
        int iMaxValue = pTag->GetMaxValue();
        int iMinValue = pTag->GetMinValue();
        int iInitializeValue = pTag->GetInitializeValue();
        float fScale = pTag->GetScale();
        int iInFrameAddress = pTag->GetInFrameAddress();
        DBTagObject *pDBTagObject = pTag->GetDBTagObject();

        if(strRegisterArea == "DO线圈")
        {
            switch(pTag->GetDataType())
            {
                case TYPE_VARIANT:
                {

                }break;
                case TYPE_BOOL:
                {
                    uint val = pDBTagObject->GetWriteData().toUInt();
                    mTCPIPModbus.WriteCoil(iDevAddress, iRegisterAddress + iOffset, val);
                }break;
                case TYPE_INT8:
                {
                    int val = pDBTagObject->GetWriteData().toInt();
                    writeBuf[0] = val & 0x01;
                    mTCPIPModbus.WriteMultipleCoils(iDevAddress, iRegisterAddress + iOffset, 8, 1, writeBuf);
                }break;
                case TYPE_UINT8:
                {
                    uint val = pDBTagObject->GetWriteData().toUInt();
                    writeBuf[0] = val & 0x01;
                    mTCPIPModbus.WriteMultipleCoils(iDevAddress, iRegisterAddress + iOffset, 8, 1, writeBuf);
                }break;
                case TYPE_INT16:
                {
                    int val = pDBTagObject->GetWriteData().toInt();
                    writeBuf[0] = val & 0x01;
                    writeBuf[1] = (val>>8) & 0x01;
                    mTCPIPModbus.WriteMultipleCoils(iDevAddress, iRegisterAddress + iOffset, 16, 2, writeBuf);
                }break;
                case TYPE_UINT16:
                {
                    uint val = pDBTagObject->GetWriteData().toUInt();
                    writeBuf[0] = val & 0x01;
                    writeBuf[1] = (val>>8) & 0x01;
                    mTCPIPModbus.WriteMultipleCoils(iDevAddress, iRegisterAddress + iOffset, 16, 2, writeBuf);
                }break;
                case TYPE_INT32:
                {
                    int val = pDBTagObject->GetWriteData().toInt();
                    writeBuf[0] = val & 0x01;
                    writeBuf[1] = (val>>8) & 0x01;
                    writeBuf[2] = (val>>16) & 0x01;
                    writeBuf[3] = (val>>24) & 0x01;
                    mTCPIPModbus.WriteMultipleCoils(iDevAddress, iRegisterAddress + iOffset, 32, 4, writeBuf);
                }break;
                case TYPE_UINT32:
                {
                    uint val = pDBTagObject->GetWriteData().toUInt();
                    writeBuf[0] = val & 0x01;
                    writeBuf[1] = (val>>8) & 0x01;
                    writeBuf[2] = (val>>16) & 0x01;
                    writeBuf[3] = (val>>24) & 0x01;
                    mTCPIPModbus.WriteMultipleCoils(iDevAddress, iRegisterAddress + iOffset, 32, 4, writeBuf);
                }break;
                case TYPE_FLOAT:
                {
                    union unionFloat
                    {
                        float ufloat;
                        unsigned char ubytes[4];
                    };
                    unionFloat uFloat;
                    uFloat.ufloat = pDBTagObject->GetWriteData().toFloat();
                    for(int i=0; i<4; i++)
                        writeBuf[i] = uFloat.ubytes[i];
                    mTCPIPModbus.WriteMultipleCoils(iDevAddress, iRegisterAddress + iOffset, 32, 4, writeBuf);
                }break;
            }
        }
        else if(strRegisterArea == "DI离散输入寄存器")
        {
        }
        else if(strRegisterArea == "AO保持寄存器")
        {
            switch(pTag->GetDataType())
            {
                case TYPE_INT16:
                {
                    int val = pDBTagObject->GetWriteData().toInt();
                    mTCPIPModbus.WriteHoldingRegister(iDevAddress, iRegisterAddress + iOffset, val);
                }break;
                case TYPE_UINT16:
                {
                    uint val = pDBTagObject->GetWriteData().toUInt();
                    mTCPIPModbus.WriteHoldingRegister(iDevAddress, iRegisterAddress + iOffset, val);
                }break;
                case TYPE_INT32:
                {
                    int val = pDBTagObject->GetWriteData().toInt();
                    mTCPIPModbus.WriteUIntToHoldingRegister(iDevAddress, iRegisterAddress + iOffset, val);
                }break;
                case TYPE_UINT32:
                {
                    uint val = pDBTagObject->GetWriteData().toUInt();
                    mTCPIPModbus.WriteUIntToHoldingRegister(iDevAddress, iRegisterAddress + iOffset, val);
                }break;
                case TYPE_FLOAT:
                {
                    float val = pDBTagObject->GetWriteData().toFloat();
                    mTCPIPModbus.WriteFloatToHoldingRegister(iDevAddress, iRegisterAddress + iOffset, val);
                }break;
                case TYPE_DOUBLE:
                {
                    double val = pDBTagObject->GetWriteData().toDouble();
                    union unionDouble
                    {
                        double udouble;
                        unsigned char ubytes[8];
                    };
                    unionDouble uDouble;
                    uDouble.udouble = val;
                    for(int i=0; i<8; i++)
                        writeBuf[i] = uDouble.ubytes[i];
                    mTCPIPModbus.WriteMultipleHoldingRegister(iDevAddress, iRegisterAddress + iOffset, 4, writeBuf);
                }break;
                case TYPE_ASCII2CHAR:
                {
                    uint val = pDBTagObject->GetWriteData().toUInt();
                    mTCPIPModbus.WriteHoldingRegister(iDevAddress, iRegisterAddress + iOffset, val);
                }break;
                case TYPE_STRING:
                {

                }break;
                case TYPE_BCD:
                {

                }break;
            }
        }
        else if(strRegisterArea == "AI输入寄存器")
        {
        }
        else
        {
        }
    }
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


/*
* 读变量
*/
bool TCPIPModbusDevice::ReadIOTag(IOTag* pTag)
{
    m_WriteMutex.lock();
    if(pTag->GetPermissionType() == READ_WRIE || pTag->GetPermissionType() == READ)
    {
        ClearReadBuffer();
        ClearIOTagReadBuffer(pTag);

        QString strRegisterArea = pTag->GetRegisterArea();
        int iDevAddress = pTag->GetDeviceAddress();
        int iRegisterAddress = pTag->GetRegisterAddress();
        int iOffset = pTag->GetOffset();
        int iMaxValue = pTag->GetMaxValue();
        int iMinValue = pTag->GetMinValue();
        int iInitializeValue = pTag->GetInitializeValue();
        float fScale = pTag->GetScale();
        int iInFrameAddress = pTag->GetInFrameAddress();
        DBTagObject *pDBTagObject = pTag->GetDBTagObject();

        if(strRegisterArea == "DO线圈")
        {
            switch(pTag->GetDataType())
            {
                case TYPE_BOOL:
                {
                    if(mTCPIPModbus.ReadCoils(iDevAddress, iRegisterAddress + iOffset, 1, readBuf) == FAIL)
                    {
                        m_WriteMutex.unlock();
                        return false;
                    }
                    pTag->pReadBuf[0] = readBuf[0];
                    QVariant value((uint)pTag->pReadBuf[0]);
                    if(mWriteQueue.indexOf(pTag) == -1)
                        pDBTagObject->SetData(value);
                }break;
                case TYPE_INT8:
                {
                    if(mTCPIPModbus.ReadCoils(iDevAddress, iRegisterAddress + iOffset, 8, readBuf) == FAIL)
                    {
                        m_WriteMutex.unlock();
                        return false;
                    }
                    pTag->pReadBuf[0] = readBuf[0];
                    QVariant value((int)pTag->pReadBuf[0]);
                    if(mWriteQueue.indexOf(pTag) == -1)
                        pDBTagObject->SetData(value);
                }break;
                case TYPE_UINT8:
                {
                    if(mTCPIPModbus.ReadCoils(iDevAddress, iRegisterAddress + iOffset, 8, readBuf) == FAIL)
                    {
                        m_WriteMutex.unlock();
                        return false;
                    }
                    pTag->pReadBuf[0] = readBuf[0];
                    QVariant value((uint)pTag->pReadBuf[0]);
                    if(mWriteQueue.indexOf(pTag) == -1)
                        pDBTagObject->SetData(value);
                }break;
                case TYPE_INT16:
                {
                    if(mTCPIPModbus.ReadCoils(iDevAddress, iRegisterAddress + iOffset, 16, readBuf) == FAIL)
                    {
                        m_WriteMutex.unlock();
                        return false;
                    }
                    for(int i=0; i<2; i++)
                        pTag->pReadBuf[i] = readBuf[i];
                    QVariant value((int)(pTag->pReadBuf[1]<<8|pTag->pReadBuf[0]));
                    if(mWriteQueue.indexOf(pTag) == -1)
                        pDBTagObject->SetData(value);
                }break;
                case TYPE_UINT16:
                {
                    if(mTCPIPModbus.ReadCoils(iDevAddress, iRegisterAddress + iOffset, 16, readBuf) == FAIL)
                    {
                        m_WriteMutex.unlock();
                        return false;
                    }
                    for(int i=0; i<2; i++)
                        pTag->pReadBuf[i] = readBuf[i];
                    QVariant value((uint)(pTag->pReadBuf[1]<<8|pTag->pReadBuf[0]));
                    if(mWriteQueue.indexOf(pTag) == -1)
                        pDBTagObject->SetData(value);
                }break;
                case TYPE_INT32:
                {
                    if(mTCPIPModbus.ReadCoils(iDevAddress, iRegisterAddress + iOffset, 32, readBuf) == FAIL)
                    {
                        m_WriteMutex.unlock();
                        return false;
                    }
                    for(int i=0; i<4; i++)
                        pTag->pReadBuf[i] = readBuf[i];
                    QVariant value((int)(pTag->pReadBuf[3]<<24|pTag->pReadBuf[2]<<16|pTag->pReadBuf[1]<<8|pTag->pReadBuf[0]));
                    if(mWriteQueue.indexOf(pTag) == -1)
                        pDBTagObject->SetData(value);
                }break;
                case TYPE_UINT32:
                {
                    if(mTCPIPModbus.ReadCoils(iDevAddress, iRegisterAddress + iOffset, 32, readBuf) == FAIL)
                    {
                        m_WriteMutex.unlock();
                        return false;
                    }
                    for(int i=0; i<4; i++)
                        pTag->pReadBuf[i] = readBuf[i];
                    QVariant value((uint)(pTag->pReadBuf[3]<<24|pTag->pReadBuf[2]<<16|pTag->pReadBuf[1]<<8|pTag->pReadBuf[0]));
                    if(mWriteQueue.indexOf(pTag) == -1)
                        pDBTagObject->SetData(value);
                }break;
                case TYPE_FLOAT:
                {
                    if(mTCPIPModbus.ReadCoils(iDevAddress, iRegisterAddress + iOffset, 32, readBuf) == FAIL)
                    {
                        m_WriteMutex.unlock();
                        return false;
                    }
                    for(int i=0; i<4; i++)
                        pTag->pReadBuf[i] = readBuf[i];

                    union unionFloat
                    {
                        float ufloat;
                        unsigned char ubytes[4];
                    };
                    unionFloat uFloat;
                    for(int i=0; i<4; i++)
                        uFloat.ubytes[i] = pTag->pReadBuf[i];
                    QVariant value(uFloat.ufloat);
                    if(mWriteQueue.indexOf(pTag) == -1)
                        pDBTagObject->SetData(value);
                }break;
            }
        }
        else if(strRegisterArea == "DI离散输入寄存器")
        {
            switch(pTag->GetDataType())
            {
                case TYPE_BOOL:
                {
                    if(mTCPIPModbus.ReadDiscreteInputs(iDevAddress, iRegisterAddress + iOffset, 1, readBuf) == FAIL)
                    {
                        m_WriteMutex.unlock();
                        return false;
                    }
                    pTag->pReadBuf[0] = readBuf[0];
                    QVariant value((uint)pTag->pReadBuf[0]);
                    if(mWriteQueue.indexOf(pTag) == -1)
                        pDBTagObject->SetData(value);
                }break;
                case TYPE_INT8:
                {
                    if(mTCPIPModbus.ReadDiscreteInputs(iDevAddress, iRegisterAddress + iOffset, 8, readBuf) == FAIL)
                    {
                        m_WriteMutex.unlock();
                        return false;
                    }
                    pTag->pReadBuf[0] = readBuf[0];
                    QVariant value((int)pTag->pReadBuf[0]);
                    if(mWriteQueue.indexOf(pTag) == -1)
                        pDBTagObject->SetData(value);
                }break;
                case TYPE_UINT8:
                {
                    if(mTCPIPModbus.ReadDiscreteInputs(iDevAddress, iRegisterAddress + iOffset, 8, readBuf) == FAIL)
                    {
                        m_WriteMutex.unlock();
                        return false;
                    }
                    pTag->pReadBuf[0] = readBuf[0];
                    QVariant value((uint)pTag->pReadBuf[0]);
                    if(mWriteQueue.indexOf(pTag) == -1)
                        pDBTagObject->SetData(value);
                }break;
                case TYPE_INT16:
                {
                    if(mTCPIPModbus.ReadDiscreteInputs(iDevAddress, iRegisterAddress + iOffset, 16, readBuf) == FAIL)
                    {
                        m_WriteMutex.unlock();
                        return false;
                    }
                    for(int i=0; i<2; i++)
                        pTag->pReadBuf[i] = readBuf[i];
                    QVariant value((int)(pTag->pReadBuf[1]<<8|pTag->pReadBuf[0]));
                    if(mWriteQueue.indexOf(pTag) == -1)
                        pDBTagObject->SetData(value);
                }break;
                case TYPE_UINT16:
                {
                    if(mTCPIPModbus.ReadDiscreteInputs(iDevAddress, iRegisterAddress + iOffset, 16, readBuf) == FAIL)
                    {
                        m_WriteMutex.unlock();
                        return false;
                    }
                    for(int i=0; i<2; i++)
                        pTag->pReadBuf[i] = readBuf[i];
                    QVariant value((uint)(pTag->pReadBuf[1]<<8|pTag->pReadBuf[0]));
                    if(mWriteQueue.indexOf(pTag) == -1)
                        pDBTagObject->SetData(value);
                }break;
                case TYPE_INT32:
                {
                    if(mTCPIPModbus.ReadDiscreteInputs(iDevAddress, iRegisterAddress + iOffset, 32, readBuf) == FAIL)
                    {
                        m_WriteMutex.unlock();
                        return false;
                    }
                    for(int i=0; i<4; i++)
                        pTag->pReadBuf[i] = readBuf[i];
                    QVariant value((int)(pTag->pReadBuf[3]<<24|pTag->pReadBuf[2]<<16|pTag->pReadBuf[1]<<8|pTag->pReadBuf[0]));
                    if(mWriteQueue.indexOf(pTag) == -1)
                        pDBTagObject->SetData(value);
                }break;
                case TYPE_UINT32:
                {
                    if(mTCPIPModbus.ReadDiscreteInputs(iDevAddress, iRegisterAddress + iOffset, 32, readBuf) == FAIL)
                    {
                        m_WriteMutex.unlock();
                        return false;
                    }
                    for(int i=0; i<4; i++)
                        pTag->pReadBuf[i] = readBuf[i];
                    QVariant value((uint)(pTag->pReadBuf[3]<<24|pTag->pReadBuf[2]<<16|pTag->pReadBuf[1]<<8|pTag->pReadBuf[0]));
                    if(mWriteQueue.indexOf(pTag) == -1)
                        pDBTagObject->SetData(value);
                }break;
                case TYPE_FLOAT:
                {
                    if(mTCPIPModbus.ReadDiscreteInputs(iDevAddress, iRegisterAddress + iOffset, 32, readBuf) == FAIL)
                    {
                        m_WriteMutex.unlock();
                        return false;
                    }
                    for(int i=0; i<4; i++)
                        pTag->pReadBuf[i] = readBuf[i];

                    union unionFloat
                    {
                        float ufloat;
                        unsigned char ubytes[4];
                    };
                    unionFloat uFloat;
                    for(int i=0; i<4; i++)
                        uFloat.ubytes[i] = pTag->pReadBuf[i];
                    QVariant value(uFloat.ufloat);
                    if(mWriteQueue.indexOf(pTag) == -1)
                        pDBTagObject->SetData(value);
                }break;
            }     
        }
        else if(strRegisterArea == "AO保持寄存器")
        {
            switch(pTag->GetDataType())
            {
                case TYPE_INT16:
                {
                    if(mTCPIPModbus.ReadHoldingRegister(iDevAddress, iRegisterAddress + iOffset, 1, readBuf) == FAIL)
                    {
                        m_WriteMutex.unlock();
                        return false;
                    }
                    for(int i=0; i<1; i++)
                    {
                        pTag->pReadBuf[i*2] = readBuf[i*2+1];
                        pTag->pReadBuf[i*2+1] = readBuf[i*2];
                    }
                    QVariant value((int)(pTag->pReadBuf[1]<<8|pTag->pReadBuf[0]));
                    if(mWriteQueue.indexOf(pTag) == -1)
                        pDBTagObject->SetData(value);
                }break;
                case TYPE_UINT16:
                {
                    if(mTCPIPModbus.ReadHoldingRegister(iDevAddress, iRegisterAddress + iOffset, 1, readBuf) == FAIL)
                    {
                        m_WriteMutex.unlock();
                        return false;
                    }
                    for(int i=0; i<1; i++)
                    {
                        pTag->pReadBuf[i*2] = readBuf[i*2+1];
                        pTag->pReadBuf[i*2+1] = readBuf[i*2];
                    }
                    QVariant value((uint)(pTag->pReadBuf[1]<<8|pTag->pReadBuf[0]));
                    if(mWriteQueue.indexOf(pTag) == -1)
                        pDBTagObject->SetData(value);
                }break;
                case TYPE_INT32:
                {
                    if(mTCPIPModbus.ReadHoldingRegister(iDevAddress, iRegisterAddress + iOffset, 2, readBuf) == FAIL)
                    {
                        m_WriteMutex.unlock();
                        return false;
                    }
                    for(int i=0; i<2; i++)
                    {
                        pTag->pReadBuf[i*2] = readBuf[i*2+1];
                        pTag->pReadBuf[i*2+1] = readBuf[i*2];
                    }
                    QVariant value((int)(pTag->pReadBuf[3]<<24|pTag->pReadBuf[2]<<16|pTag->pReadBuf[1]<<8|pTag->pReadBuf[0]));
                    if(mWriteQueue.indexOf(pTag) == -1)
                        pDBTagObject->SetData(value);
                }break;
                case TYPE_UINT32:
                {
                    if(mTCPIPModbus.ReadHoldingRegister(iDevAddress, iRegisterAddress + iOffset, 2, readBuf) == FAIL)
                    {
                        m_WriteMutex.unlock();
                        return false;
                    }
                    for(int i=0; i<2; i++)
                    {
                        pTag->pReadBuf[i*2] = readBuf[i*2+1];
                        pTag->pReadBuf[i*2+1] = readBuf[i*2];
                    }
                    QVariant value((uint)(pTag->pReadBuf[3]<<24|pTag->pReadBuf[2]<<16|pTag->pReadBuf[1]<<8|pTag->pReadBuf[0]));
                    if(mWriteQueue.indexOf(pTag) == -1)
                        pDBTagObject->SetData(value);
                }break;
                case TYPE_FLOAT:
                {
                    if(mTCPIPModbus.ReadHoldingRegister(iDevAddress, iRegisterAddress + iOffset, 2, readBuf) == FAIL)
                    {
                        m_WriteMutex.unlock();
                        return false;
                    }
                    for(int i=0; i<2; i++)
                    {
                        pTag->pReadBuf[i*2] = readBuf[i*2+1];
                        pTag->pReadBuf[i*2+1] = readBuf[i*2];
                    }
                    union unionFloat
                    {
                        float ufloat;
                        unsigned char ubytes[4];
                    };
                    unionFloat uFloat;
                    for(int i=0; i<4; i++)
                        uFloat.ubytes[i] = pTag->pReadBuf[i];
                    QVariant value(uFloat.ufloat);
                    if(mWriteQueue.indexOf(pTag) == -1)
                        pDBTagObject->SetData(value);
                }break;
                case TYPE_DOUBLE:
                {
                    if(mTCPIPModbus.ReadHoldingRegister(iDevAddress, iRegisterAddress + iOffset, 4, readBuf) == FAIL)
                    {
                        m_WriteMutex.unlock();
                        return false;
                    }
                    for(int i=0; i<4; i++)
                    {
                        pTag->pReadBuf[i*2] = readBuf[i*2+1];
                        pTag->pReadBuf[i*2+1] = readBuf[i*2];
                    }
                    union unionDouble
                    {
                        double udouble;
                        unsigned char ubytes[8];
                    };
                    unionDouble uDouble;
                    for(int i=0; i<8; i++)
                        uDouble.ubytes[i] = pTag->pReadBuf[i];
                    QVariant value(uDouble.udouble);
                    if(mWriteQueue.indexOf(pTag) == -1)
                        pDBTagObject->SetData(value);
                }break;
                case TYPE_ASCII2CHAR:
                {
                    if(mTCPIPModbus.ReadHoldingRegister(iDevAddress, iRegisterAddress + iOffset, 1, readBuf) == FAIL)
                    {
                        m_WriteMutex.unlock();
                        return false;
                    }
                    pTag->pReadBuf[0] = readBuf[0];
                    pTag->pReadBuf[1] = readBuf[1];
                    QVariant value((uint)(pTag->pReadBuf[1]<<8|pTag->pReadBuf[0]));
                    if(mWriteQueue.indexOf(pTag) == -1)
                        pDBTagObject->SetData(value);
                }break;
                case TYPE_STRING:
                {

                }break;
                case TYPE_BCD:
                {

                }break;
            }
        }
        else if(strRegisterArea == "AI输入寄存器")
        {
            switch(pTag->GetDataType())
            {
                case TYPE_INT16:
                {
                    if(mTCPIPModbus.ReadReadInputRegister(iDevAddress, iRegisterAddress + iOffset, 1, readBuf) == FAIL)
                    {
                        m_WriteMutex.unlock();
                        return false;
                    }
                    for(int i=0; i<1; i++)
                    {
                        pTag->pReadBuf[i*2] = readBuf[i*2+1];
                        pTag->pReadBuf[i*2+1] = readBuf[i*2];
                    }
                    QVariant value((int)(pTag->pReadBuf[1]<<8|pTag->pReadBuf[0]));
                    if(mWriteQueue.indexOf(pTag) == -1)
                        pDBTagObject->SetData(value);
                }break;
                case TYPE_UINT16:
                {
                    if(mTCPIPModbus.ReadReadInputRegister(iDevAddress, iRegisterAddress + iOffset, 1, readBuf) == FAIL)
                    {
                        m_WriteMutex.unlock();
                        return false;
                    }
                    for(int i=0; i<1; i++)
                    {
                        pTag->pReadBuf[i*2] = readBuf[i*2+1];
                        pTag->pReadBuf[i*2+1] = readBuf[i*2];
                    }
                    QVariant value((uint)(pTag->pReadBuf[1]<<8|pTag->pReadBuf[0]));
                    if(mWriteQueue.indexOf(pTag) == -1)
                        pDBTagObject->SetData(value);
                }break;
                case TYPE_INT32:
                {
                    if(mTCPIPModbus.ReadReadInputRegister(iDevAddress, iRegisterAddress + iOffset, 2, readBuf) == FAIL)
                    {
                        m_WriteMutex.unlock();
                        return false;
                    }
                    for(int i=0; i<2; i++)
                    {
                        pTag->pReadBuf[i*2] = readBuf[i*2+1];
                        pTag->pReadBuf[i*2+1] = readBuf[i*2];
                    }
                    QVariant value((int)(pTag->pReadBuf[3]<<24|pTag->pReadBuf[2]<<16|pTag->pReadBuf[1]<<8|pTag->pReadBuf[0]));
                    if(mWriteQueue.indexOf(pTag) == -1)
                        pDBTagObject->SetData(value);
                }break;
                case TYPE_UINT32:
                {
                    if(mTCPIPModbus.ReadReadInputRegister(iDevAddress, iRegisterAddress + iOffset, 2, readBuf) == FAIL)
                    {
                        m_WriteMutex.unlock();
                        return false;
                    }
                    for(int i=0; i<2; i++)
                    {
                        pTag->pReadBuf[i*2] = readBuf[i*2+1];
                        pTag->pReadBuf[i*2+1] = readBuf[i*2];
                    }
                    QVariant value((uint)(pTag->pReadBuf[3]<<24|pTag->pReadBuf[2]<<16|pTag->pReadBuf[1]<<8|pTag->pReadBuf[0]));
                    if(mWriteQueue.indexOf(pTag) == -1)
                        pDBTagObject->SetData(value);
                }break;
                case TYPE_FLOAT:
                {
                    if(mTCPIPModbus.ReadReadInputRegister(iDevAddress, iRegisterAddress + iOffset, 2, readBuf) == FAIL)
                    {
                        m_WriteMutex.unlock();
                        return false;
                    }
                    for(int i=0; i<2; i++)
                    {
                        pTag->pReadBuf[i*2] = readBuf[i*2+1];
                        pTag->pReadBuf[i*2+1] = readBuf[i*2];
                    }
                    union unionFloat
                    {
                        float ufloat;
                        unsigned char ubytes[4];
                    };
                    unionFloat uFloat;
                    for(int i=0; i<4; i++)
                        uFloat.ubytes[i] = pTag->pReadBuf[i];
                    QVariant value(uFloat.ufloat);
                    if(mWriteQueue.indexOf(pTag) == -1)
                        pDBTagObject->SetData(value);
                }break;
                case TYPE_DOUBLE:
                {
                    if(mTCPIPModbus.ReadReadInputRegister(iDevAddress, iRegisterAddress + iOffset, 4, readBuf) == FAIL)
                    {
                        m_WriteMutex.unlock();
                        return false;
                    }
                    for(int i=0; i<4; i++)
                    {
                        pTag->pReadBuf[i*2] = readBuf[i*2+1];
                        pTag->pReadBuf[i*2+1] = readBuf[i*2];
                    }
                    union unionFloat
                    {
                        double udouble;
                        unsigned char ubytes[8];
                    };
                    unionFloat uFloat;
                    for(int i=0; i<8; i++)
                        uFloat.ubytes[i] = pTag->pReadBuf[i];
                    QVariant value(uFloat.udouble);
                    if(mWriteQueue.indexOf(pTag) == -1)
                        pDBTagObject->SetData(value);
                }break;
                case TYPE_ASCII2CHAR:
                {
                    if(mTCPIPModbus.ReadReadInputRegister(iDevAddress, iRegisterAddress + iOffset, 1, readBuf) == FAIL)
                    {
                        m_WriteMutex.unlock();
                        return false;
                    }
                    pTag->pReadBuf[0] = readBuf[0];
                    pTag->pReadBuf[1] = readBuf[1];
                    QVariant value((uint)(pTag->pReadBuf[1]<<8|pTag->pReadBuf[0]));
                    if(mWriteQueue.indexOf(pTag) == -1)
                        pDBTagObject->SetData(value);
                }break;
                case TYPE_STRING:
                {

                }break;
                case TYPE_BCD:
                {

                }break;
            }
        }
        else
        {

        }
    }
    m_WriteMutex.unlock();
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
bool TCPIPModbusDevice::LoadData(SaveFormat saveFormat, QString fileName)
{
    pNetDevicePrivate = new NetDevicePrivate();
    if (pNetDevicePrivate->LoadData(saveFormat, fileName))
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




