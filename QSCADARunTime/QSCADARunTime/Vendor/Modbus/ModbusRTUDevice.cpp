#include "ModbusRTUDevice.h"
#include "ComPort.h"
#include "RealTimeDB.h"
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


ModbusRTUDevice::ModbusRTUDevice()
    : iFacePort(0),
      pComDevicePrivate(0),
      miFailCnt(0)
{
    mReadList.clear();
    mWriteQueue.clear();
    ComPort *pComPort = new ComPort();
    iFacePort = pComPort;
    mModbusRTU.SetPort(iFacePort);
    mbIsRunning = false;
}

ModbusRTUDevice::~ModbusRTUDevice()
{
    Stop();

    qDeleteAll(mReadList);
    mReadList.clear();

    if(!mWriteQueue.isEmpty())
    {
        qDeleteAll(mWriteQueue);
        mWriteQueue.clear();
    }

    if(iFacePort !=0 )
    {
        delete iFacePort;
        iFacePort = 0;
    }
    if(pComDevicePrivate !=0 )
    {
        delete pComDevicePrivate;
        pComDevicePrivate = 0;
    }
}



/*
* 获取设备名称
*/
QString ModbusRTUDevice::GetDeviceName()
{
    if(pComDevicePrivate !=0 )
        return pComDevicePrivate->m_sDeviceName;
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
IOTag* ModbusRTUDevice::FindIOTagByID(qint32 id)
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
bool ModbusRTUDevice::WriteIOTag(IOTag* pTag)
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
                case TYPE_BOOL:
                {
                    uint val = pDBTagObject->GetWriteData().toUInt();
                    mModbusRTU.WriteCoil(iDevAddress, iRegisterAddress + iOffset, val);
                }break;
                case TYPE_INT8:
                {
                    int val = pDBTagObject->GetWriteData().toInt();
                    writeBuf[0] = val & 0x01;
                    mModbusRTU.WriteMultipleCoils(iDevAddress, iRegisterAddress + iOffset, 8, 1, writeBuf);
                }break;
                case TYPE_UINT8:
                {
                    uint val = pDBTagObject->GetWriteData().toUInt();
                    writeBuf[0] = val & 0x01;
                    mModbusRTU.WriteMultipleCoils(iDevAddress, iRegisterAddress + iOffset, 8, 1, writeBuf);
                }break;
                case TYPE_INT16:
                {
                    int val = pDBTagObject->GetWriteData().toInt();
                    writeBuf[0] = val & 0x01;
                    writeBuf[1] = (val>>8) & 0x01;
                    mModbusRTU.WriteMultipleCoils(iDevAddress, iRegisterAddress + iOffset, 16, 2, writeBuf);
                }break;
                case TYPE_UINT16:
                {
                    uint val = pDBTagObject->GetWriteData().toUInt();
                    writeBuf[0] = val & 0x01;
                    writeBuf[1] = (val>>8) & 0x01;
                    mModbusRTU.WriteMultipleCoils(iDevAddress, iRegisterAddress + iOffset, 16, 2, writeBuf);
                }break;
                case TYPE_INT32:
                {
                    int val = pDBTagObject->GetWriteData().toInt();
                    writeBuf[0] = val & 0x01;
                    writeBuf[1] = (val>>8) & 0x01;
                    writeBuf[2] = (val>>16) & 0x01;
                    writeBuf[3] = (val>>24) & 0x01;
                    mModbusRTU.WriteMultipleCoils(iDevAddress, iRegisterAddress + iOffset, 32, 4, writeBuf);
                }break;
                case TYPE_UINT32:
                {
                    uint val = pDBTagObject->GetWriteData().toUInt();
                    writeBuf[0] = val & 0x01;
                    writeBuf[1] = (val>>8) & 0x01;
                    writeBuf[2] = (val>>16) & 0x01;
                    writeBuf[3] = (val>>24) & 0x01;
                    mModbusRTU.WriteMultipleCoils(iDevAddress, iRegisterAddress + iOffset, 32, 4, writeBuf);
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
                    mModbusRTU.WriteMultipleCoils(iDevAddress, iRegisterAddress + iOffset, 32, 4, writeBuf);
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
                    mModbusRTU.WriteHoldingRegister(iDevAddress, iRegisterAddress + iOffset, val);
                }break;
                case TYPE_UINT16:
                {
                    uint val = pDBTagObject->GetWriteData().toUInt();
                    mModbusRTU.WriteHoldingRegister(iDevAddress, iRegisterAddress + iOffset, val);
                }break;
                case TYPE_INT32:
                {
                    int val = pDBTagObject->GetWriteData().toInt();
                    mModbusRTU.WriteUIntToHoldingRegister(iDevAddress, iRegisterAddress + iOffset, val);
                }break;
                case TYPE_UINT32:
                {
                    uint val = pDBTagObject->GetWriteData().toUInt();
                    mModbusRTU.WriteUIntToHoldingRegister(iDevAddress, iRegisterAddress + iOffset, val);
                }break;
                case TYPE_FLOAT:
                {
                    float val = pDBTagObject->GetWriteData().toFloat();
                    mModbusRTU.WriteFloatToHoldingRegister(iDevAddress, iRegisterAddress + iOffset, val);
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
                    mModbusRTU.WriteMultipleHoldingRegister(iDevAddress, iRegisterAddress + iOffset, 4, writeBuf);
                }break;
                case TYPE_ASCII2CHAR:
                {
                    uint val = pDBTagObject->GetWriteData().toUInt();
                    mModbusRTU.WriteHoldingRegister(iDevAddress, iRegisterAddress + iOffset, val);
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
bool ModbusRTUDevice::WriteIOTags()
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
bool ModbusRTUDevice::ReadIOTag(IOTag* pTag)
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
                    if(mModbusRTU.ReadCoils(iDevAddress, iRegisterAddress + iOffset, 1, readBuf) == FAIL)
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
                    if(mModbusRTU.ReadCoils(iDevAddress, iRegisterAddress + iOffset, 8, readBuf) == FAIL)
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
                    if(mModbusRTU.ReadCoils(iDevAddress, iRegisterAddress + iOffset, 8, readBuf) == FAIL)
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
                    if(mModbusRTU.ReadCoils(iDevAddress, iRegisterAddress + iOffset, 16, readBuf) == FAIL)
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
                    if(mModbusRTU.ReadCoils(iDevAddress, iRegisterAddress + iOffset, 16, readBuf) == FAIL)
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
                    if(mModbusRTU.ReadCoils(iDevAddress, iRegisterAddress + iOffset, 32, readBuf) == FAIL)
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
                    if(mModbusRTU.ReadCoils(iDevAddress, iRegisterAddress + iOffset, 32, readBuf) == FAIL)
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
                    if(mModbusRTU.ReadCoils(iDevAddress, iRegisterAddress + iOffset, 32, readBuf) == FAIL)
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
                    if(mModbusRTU.ReadDiscreteInputs(iDevAddress, iRegisterAddress + iOffset, 1, readBuf) == FAIL)
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
                    if(mModbusRTU.ReadDiscreteInputs(iDevAddress, iRegisterAddress + iOffset, 8, readBuf) == FAIL)
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
                    if(mModbusRTU.ReadDiscreteInputs(iDevAddress, iRegisterAddress + iOffset, 8, readBuf) == FAIL)
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
                    if(mModbusRTU.ReadDiscreteInputs(iDevAddress, iRegisterAddress + iOffset, 16, readBuf) == FAIL)
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
                    if(mModbusRTU.ReadDiscreteInputs(iDevAddress, iRegisterAddress + iOffset, 16, readBuf) == FAIL)
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
                    if(mModbusRTU.ReadDiscreteInputs(iDevAddress, iRegisterAddress + iOffset, 32, readBuf) == FAIL)
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
                    if(mModbusRTU.ReadDiscreteInputs(iDevAddress, iRegisterAddress + iOffset, 32, readBuf) == FAIL)
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
                    if(mModbusRTU.ReadDiscreteInputs(iDevAddress, iRegisterAddress + iOffset, 32, readBuf) == FAIL)
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
                    if(mModbusRTU.ReadHoldingRegister(iDevAddress, iRegisterAddress + iOffset, 1, readBuf) == FAIL)
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
                    if(mModbusRTU.ReadHoldingRegister(iDevAddress, iRegisterAddress + iOffset, 1, readBuf) == FAIL)
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
                    if(mModbusRTU.ReadHoldingRegister(iDevAddress, iRegisterAddress + iOffset, 2, readBuf) == FAIL)
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
                    if(mModbusRTU.ReadHoldingRegister(iDevAddress, iRegisterAddress + iOffset, 2, readBuf) == FAIL)
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
                    if(mModbusRTU.ReadHoldingRegister(iDevAddress, iRegisterAddress + iOffset, 2, readBuf) == FAIL)
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
                    if(mModbusRTU.ReadHoldingRegister(iDevAddress, iRegisterAddress + iOffset, 4, readBuf) == FAIL)
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
                    if(mModbusRTU.ReadHoldingRegister(iDevAddress, iRegisterAddress + iOffset, 1, readBuf) == FAIL)
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
                    if(mModbusRTU.ReadReadInputRegister(iDevAddress, iRegisterAddress + iOffset, 1, readBuf) == FAIL)
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
                    if(mModbusRTU.ReadReadInputRegister(iDevAddress, iRegisterAddress + iOffset, 1, readBuf) == FAIL)
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
                    if(mModbusRTU.ReadReadInputRegister(iDevAddress, iRegisterAddress + iOffset, 2, readBuf) == FAIL)
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
                    if(mModbusRTU.ReadReadInputRegister(iDevAddress, iRegisterAddress + iOffset, 2, readBuf) == FAIL)
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
                    if(mModbusRTU.ReadReadInputRegister(iDevAddress, iRegisterAddress + iOffset, 2, readBuf) == FAIL)
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
                    if(mModbusRTU.ReadReadInputRegister(iDevAddress, iRegisterAddress + iOffset, 4, readBuf) == FAIL)
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
                    if(mModbusRTU.ReadReadInputRegister(iDevAddress, iRegisterAddress + iOffset, 1, readBuf) == FAIL)
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

        if(pComDevicePrivate !=0 )
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
    return pComDevicePrivate->m_sPortNumber;
}


/*
* 从文件读取配置数据
*/
bool ModbusRTUDevice::LoadData(SaveFormat saveFormat, QString fileName)
{
    pComDevicePrivate = new ComDevicePrivate();
    if (pComDevicePrivate->LoadData(saveFormat, fileName))
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





