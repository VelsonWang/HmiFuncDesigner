#include "ModbusASCIIDevice.h"
#include "RealTimeDB.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QThread>
#include <QDebug>

ModbusASCIIDevice::ModbusASCIIDevice()
    : pComDevicePrivate(0)
{
    comPort_ = new ComPort();
    iFacePort = comPort_;
    mModbusAscii.SetPort(iFacePort); 
}

ModbusASCIIDevice::~ModbusASCIIDevice()
{  
    if(comPort_ != nullptr)
    {
        delete comPort_;
        comPort_ = nullptr;
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
QString ModbusASCIIDevice::GetDeviceName()
{
    if(pComDevicePrivate !=0 )
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
IOTag* ModbusASCIIDevice::FindIOTagByID(qint32 id)
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
bool ModbusASCIIDevice::WriteIOTag(IOTag* pTag)
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
                    mModbusAscii.WriteCoil(iDevAddress, iRegisterAddress + iOffset, val);
                }break;
                case TYPE_INT8:
                {
                    int val = pDBTagObject->GetWriteData().toInt();
                    writeBuf[0] = val & 0x01;
                    mModbusAscii.WriteMultipleCoils(iDevAddress, iRegisterAddress + iOffset, 8, 1, writeBuf);
                }break;
                case TYPE_UINT8:
                {
                    uint val = pDBTagObject->GetWriteData().toUInt();
                    writeBuf[0] = val & 0x01;
                    mModbusAscii.WriteMultipleCoils(iDevAddress, iRegisterAddress + iOffset, 8, 1, writeBuf);
                }break;
                case TYPE_INT16:
                {
                    int val = pDBTagObject->GetWriteData().toInt();
                    writeBuf[0] = val & 0x01;
                    writeBuf[1] = (val>>8) & 0x01;
                    mModbusAscii.WriteMultipleCoils(iDevAddress, iRegisterAddress + iOffset, 16, 2, writeBuf);
                }break;
                case TYPE_UINT16:
                {
                    uint val = pDBTagObject->GetWriteData().toUInt();
                    writeBuf[0] = val & 0x01;
                    writeBuf[1] = (val>>8) & 0x01;
                    mModbusAscii.WriteMultipleCoils(iDevAddress, iRegisterAddress + iOffset, 16, 2, writeBuf);
                }break;
                case TYPE_INT32:
                {
                    int val = pDBTagObject->GetWriteData().toInt();
                    writeBuf[0] = val & 0x01;
                    writeBuf[1] = (val>>8) & 0x01;
                    writeBuf[2] = (val>>16) & 0x01;
                    writeBuf[3] = (val>>24) & 0x01;
                    mModbusAscii.WriteMultipleCoils(iDevAddress, iRegisterAddress + iOffset, 32, 4, writeBuf);
                }break;
                case TYPE_UINT32:
                {
                    uint val = pDBTagObject->GetWriteData().toUInt();
                    writeBuf[0] = val & 0x01;
                    writeBuf[1] = (val>>8) & 0x01;
                    writeBuf[2] = (val>>16) & 0x01;
                    writeBuf[3] = (val>>24) & 0x01;
                    mModbusAscii.WriteMultipleCoils(iDevAddress, iRegisterAddress + iOffset, 32, 4, writeBuf);
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
                    mModbusAscii.WriteMultipleCoils(iDevAddress, iRegisterAddress + iOffset, 32, 4, writeBuf);
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
                    mModbusAscii.WriteHoldingRegister(iDevAddress, iRegisterAddress + iOffset, val);
                }break;
                case TYPE_UINT16:
                {
                    uint val = pDBTagObject->GetWriteData().toUInt();
                    mModbusAscii.WriteHoldingRegister(iDevAddress, iRegisterAddress + iOffset, val);
                }break;
                case TYPE_INT32:
                {
                    int val = pDBTagObject->GetWriteData().toInt();
                    mModbusAscii.WriteUIntToHoldingRegister(iDevAddress, iRegisterAddress + iOffset, val);
                }break;
                case TYPE_UINT32:
                {
                    uint val = pDBTagObject->GetWriteData().toUInt();
                    mModbusAscii.WriteUIntToHoldingRegister(iDevAddress, iRegisterAddress + iOffset, val);
                }break;
                case TYPE_FLOAT:
                {
                    float val = pDBTagObject->GetWriteData().toFloat();
                    mModbusAscii.WriteFloatToHoldingRegister(iDevAddress, iRegisterAddress + iOffset, val);
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
                    mModbusAscii.WriteMultipleHoldingRegister(iDevAddress, iRegisterAddress + iOffset, 4, writeBuf);
                }break;
                case TYPE_ASCII2CHAR:
                {
                    uint val = pDBTagObject->GetWriteData().toUInt();
                    mModbusAscii.WriteHoldingRegister(iDevAddress, iRegisterAddress + iOffset, val);
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
bool ModbusASCIIDevice::WriteIOTags()
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
bool ModbusASCIIDevice::ReadIOTag(IOTag* pTag)
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
                    if(mModbusAscii.ReadCoils(iDevAddress, iRegisterAddress + iOffset, 1, readBuf) == FAIL)
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
                    if(mModbusAscii.ReadCoils(iDevAddress, iRegisterAddress + iOffset, 8, readBuf) == FAIL)
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
                    if(mModbusAscii.ReadCoils(iDevAddress, iRegisterAddress + iOffset, 8, readBuf) == FAIL)
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
                    if(mModbusAscii.ReadCoils(iDevAddress, iRegisterAddress + iOffset, 16, readBuf) == FAIL)
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
                    if(mModbusAscii.ReadCoils(iDevAddress, iRegisterAddress + iOffset, 16, readBuf) == FAIL)
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
                    if(mModbusAscii.ReadCoils(iDevAddress, iRegisterAddress + iOffset, 32, readBuf) == FAIL)
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
                    if(mModbusAscii.ReadCoils(iDevAddress, iRegisterAddress + iOffset, 32, readBuf) == FAIL)
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
                    if(mModbusAscii.ReadCoils(iDevAddress, iRegisterAddress + iOffset, 32, readBuf) == FAIL)
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
                    if(mModbusAscii.ReadDiscreteInputs(iDevAddress, iRegisterAddress + iOffset, 1, readBuf) == FAIL)
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
                    if(mModbusAscii.ReadDiscreteInputs(iDevAddress, iRegisterAddress + iOffset, 8, readBuf) == FAIL)
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
                    if(mModbusAscii.ReadDiscreteInputs(iDevAddress, iRegisterAddress + iOffset, 8, readBuf) == FAIL)
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
                    if(mModbusAscii.ReadDiscreteInputs(iDevAddress, iRegisterAddress + iOffset, 16, readBuf) == FAIL)
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
                    if(mModbusAscii.ReadDiscreteInputs(iDevAddress, iRegisterAddress + iOffset, 16, readBuf) == FAIL)
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
                    if(mModbusAscii.ReadDiscreteInputs(iDevAddress, iRegisterAddress + iOffset, 32, readBuf) == FAIL)
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
                    if(mModbusAscii.ReadDiscreteInputs(iDevAddress, iRegisterAddress + iOffset, 32, readBuf) == FAIL)
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
                    if(mModbusAscii.ReadDiscreteInputs(iDevAddress, iRegisterAddress + iOffset, 32, readBuf) == FAIL)
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
                    if(mModbusAscii.ReadHoldingRegister(iDevAddress, iRegisterAddress + iOffset, 1, readBuf) == FAIL)
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
                    if(mModbusAscii.ReadHoldingRegister(iDevAddress, iRegisterAddress + iOffset, 1, readBuf) == FAIL)
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
                    if(mModbusAscii.ReadHoldingRegister(iDevAddress, iRegisterAddress + iOffset, 2, readBuf) == FAIL)
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
                    if(mModbusAscii.ReadHoldingRegister(iDevAddress, iRegisterAddress + iOffset, 2, readBuf) == FAIL)
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
                    if(mModbusAscii.ReadHoldingRegister(iDevAddress, iRegisterAddress + iOffset, 2, readBuf) == FAIL)
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
                    if(mModbusAscii.ReadHoldingRegister(iDevAddress, iRegisterAddress + iOffset, 4, readBuf) == FAIL)
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
                    if(mModbusAscii.ReadHoldingRegister(iDevAddress, iRegisterAddress + iOffset, 1, readBuf) == FAIL)
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
                    if(mModbusAscii.ReadReadInputRegister(iDevAddress, iRegisterAddress + iOffset, 1, readBuf) == FAIL)
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
                    if(mModbusAscii.ReadReadInputRegister(iDevAddress, iRegisterAddress + iOffset, 1, readBuf) == FAIL)
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
                    if(mModbusAscii.ReadReadInputRegister(iDevAddress, iRegisterAddress + iOffset, 2, readBuf) == FAIL)
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
                    if(mModbusAscii.ReadReadInputRegister(iDevAddress, iRegisterAddress + iOffset, 2, readBuf) == FAIL)
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
                    if(mModbusAscii.ReadReadInputRegister(iDevAddress, iRegisterAddress + iOffset, 2, readBuf) == FAIL)
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
                    if(mModbusAscii.ReadReadInputRegister(iDevAddress, iRegisterAddress + iOffset, 4, readBuf) == FAIL)
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
                    if(mModbusAscii.ReadReadInputRegister(iDevAddress, iRegisterAddress + iOffset, 1, readBuf) == FAIL)
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
bool ModbusASCIIDevice::ReadIOTags()
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
bool ModbusASCIIDevice::LoadData(SaveFormat saveFormat, QString fileName)
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




