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

        BeforeWriteIOTag(pTag);

        if(strRegisterArea == "DO线圈")
        {
            switch(pTag->GetDataType())
            {
                case TYPE_BOOL:
                {
                    uint val = pDBTagObject->GetWriteData().toUInt();
                    modbusRTU_.WriteCoil(iDevAddress, iRegisterAddress + iOffset, val);
                }break;
                case TYPE_INT8:
                {
                    int val = pDBTagObject->GetWriteData().toInt();
                    writeBuf[0] = val & 0x01;
                    modbusRTU_.WriteMultipleCoils(iDevAddress, iRegisterAddress + iOffset, 8, 1, writeBuf);
                }break;
                case TYPE_UINT8:
                {
                    uint val = pDBTagObject->GetWriteData().toUInt();
                    writeBuf[0] = val & 0x01;
                    modbusRTU_.WriteMultipleCoils(iDevAddress, iRegisterAddress + iOffset, 8, 1, writeBuf);
                }break;
                case TYPE_INT16:
                {
                    int val = pDBTagObject->GetWriteData().toInt();
                    writeBuf[0] = val & 0x01;
                    writeBuf[1] = (val>>8) & 0x01;
                    modbusRTU_.WriteMultipleCoils(iDevAddress, iRegisterAddress + iOffset, 16, 2, writeBuf);
                }break;
                case TYPE_UINT16:
                {
                    uint val = pDBTagObject->GetWriteData().toUInt();
                    writeBuf[0] = val & 0x01;
                    writeBuf[1] = (val>>8) & 0x01;
                    modbusRTU_.WriteMultipleCoils(iDevAddress, iRegisterAddress + iOffset, 16, 2, writeBuf);
                }break;
                case TYPE_INT32:
                {
                    int val = pDBTagObject->GetWriteData().toInt();
                    writeBuf[0] = val & 0x01;
                    writeBuf[1] = (val>>8) & 0x01;
                    writeBuf[2] = (val>>16) & 0x01;
                    writeBuf[3] = (val>>24) & 0x01;
                    modbusRTU_.WriteMultipleCoils(iDevAddress, iRegisterAddress + iOffset, 32, 4, writeBuf);
                }break;
                case TYPE_UINT32:
                {
                    uint val = pDBTagObject->GetWriteData().toUInt();
                    writeBuf[0] = val & 0x01;
                    writeBuf[1] = (val>>8) & 0x01;
                    writeBuf[2] = (val>>16) & 0x01;
                    writeBuf[3] = (val>>24) & 0x01;
                    modbusRTU_.WriteMultipleCoils(iDevAddress, iRegisterAddress + iOffset, 32, 4, writeBuf);
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
                    modbusRTU_.WriteMultipleCoils(iDevAddress, iRegisterAddress + iOffset, 32, 4, writeBuf);
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
                    modbusRTU_.WriteHoldingRegister(iDevAddress, iRegisterAddress + iOffset, val);
                }break;
                case TYPE_UINT16:
                {
                    uint val = pDBTagObject->GetWriteData().toUInt();
                    modbusRTU_.WriteHoldingRegister(iDevAddress, iRegisterAddress + iOffset, val);
                }break;
                case TYPE_INT32:
                {
                    int val = pDBTagObject->GetWriteData().toInt();
                    modbusRTU_.WriteUIntToHoldingRegister(iDevAddress, iRegisterAddress + iOffset, val);
                }break;
                case TYPE_UINT32:
                {
                    uint val = pDBTagObject->GetWriteData().toUInt();
                    modbusRTU_.WriteUIntToHoldingRegister(iDevAddress, iRegisterAddress + iOffset, val);
                }break;
                case TYPE_FLOAT:
                {
                    float val = pDBTagObject->GetWriteData().toFloat();
                    modbusRTU_.WriteFloatToHoldingRegister(iDevAddress, iRegisterAddress + iOffset, val);
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
                    modbusRTU_.WriteMultipleHoldingRegister(iDevAddress, iRegisterAddress + iOffset, 4, writeBuf);
                }break;
                case TYPE_ASCII2CHAR:
                {
                    uint val = pDBTagObject->GetWriteData().toUInt();
                    modbusRTU_.WriteHoldingRegister(iDevAddress, iRegisterAddress + iOffset, val);
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

        BeforeReadIOTag(pTag);

        if(strRegisterArea == "DO线圈")
        {
            switch(pTag->GetDataType())
            {
                case TYPE_BOOL:
                {
                    if(modbusRTU_.ReadCoils(iDevAddress, iRegisterAddress + iOffset, 1, readBuf) == FAIL)
                    {
                        return false;
                    }
                    pTag->pReadBuf[0] = readBuf[0];
                    QVariant value((uint)pTag->pReadBuf[0]);
                    if(mWriteQueue.indexOf(pTag) == -1)
                        pDBTagObject->SetData(value);
                }break;
                case TYPE_INT8:
                {
                    if(modbusRTU_.ReadCoils(iDevAddress, iRegisterAddress + iOffset, 8, readBuf) == FAIL)
                    {
                        return false;
                    }
                    pTag->pReadBuf[0] = readBuf[0];
                    QVariant value((int)pTag->pReadBuf[0]);
                    if(mWriteQueue.indexOf(pTag) == -1)
                        pDBTagObject->SetData(value);
                }break;
                case TYPE_UINT8:
                {
                    if(modbusRTU_.ReadCoils(iDevAddress, iRegisterAddress + iOffset, 8, readBuf) == FAIL)
                    {
                        return false;
                    }
                    pTag->pReadBuf[0] = readBuf[0];
                    QVariant value((uint)pTag->pReadBuf[0]);
                    if(mWriteQueue.indexOf(pTag) == -1)
                        pDBTagObject->SetData(value);
                }break;
                case TYPE_INT16:
                {
                    if(modbusRTU_.ReadCoils(iDevAddress, iRegisterAddress + iOffset, 16, readBuf) == FAIL)
                    {
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
                    if(modbusRTU_.ReadCoils(iDevAddress, iRegisterAddress + iOffset, 16, readBuf) == FAIL)
                    {
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
                    if(modbusRTU_.ReadCoils(iDevAddress, iRegisterAddress + iOffset, 32, readBuf) == FAIL)
                    {
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
                    if(modbusRTU_.ReadCoils(iDevAddress, iRegisterAddress + iOffset, 32, readBuf) == FAIL)
                    {
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
                    if(modbusRTU_.ReadCoils(iDevAddress, iRegisterAddress + iOffset, 32, readBuf) == FAIL)
                    {
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
                    if(modbusRTU_.ReadDiscreteInputs(iDevAddress, iRegisterAddress + iOffset, 1, readBuf) == FAIL)
                    {
                        return false;
                    }
                    pTag->pReadBuf[0] = readBuf[0];
                    QVariant value((uint)pTag->pReadBuf[0]);
                    if(mWriteQueue.indexOf(pTag) == -1)
                        pDBTagObject->SetData(value);
                }break;
                case TYPE_INT8:
                {
                    if(modbusRTU_.ReadDiscreteInputs(iDevAddress, iRegisterAddress + iOffset, 8, readBuf) == FAIL)
                    {
                        return false;
                    }
                    pTag->pReadBuf[0] = readBuf[0];
                    QVariant value((int)pTag->pReadBuf[0]);
                    if(mWriteQueue.indexOf(pTag) == -1)
                        pDBTagObject->SetData(value);
                }break;
                case TYPE_UINT8:
                {
                    if(modbusRTU_.ReadDiscreteInputs(iDevAddress, iRegisterAddress + iOffset, 8, readBuf) == FAIL)
                    {
                        return false;
                    }
                    pTag->pReadBuf[0] = readBuf[0];
                    QVariant value((uint)pTag->pReadBuf[0]);
                    if(mWriteQueue.indexOf(pTag) == -1)
                        pDBTagObject->SetData(value);
                }break;
                case TYPE_INT16:
                {
                    if(modbusRTU_.ReadDiscreteInputs(iDevAddress, iRegisterAddress + iOffset, 16, readBuf) == FAIL)
                    {
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
                    if(modbusRTU_.ReadDiscreteInputs(iDevAddress, iRegisterAddress + iOffset, 16, readBuf) == FAIL)
                    {
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
                    if(modbusRTU_.ReadDiscreteInputs(iDevAddress, iRegisterAddress + iOffset, 32, readBuf) == FAIL)
                    {
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
                    if(modbusRTU_.ReadDiscreteInputs(iDevAddress, iRegisterAddress + iOffset, 32, readBuf) == FAIL)
                    {
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
                    if(modbusRTU_.ReadDiscreteInputs(iDevAddress, iRegisterAddress + iOffset, 32, readBuf) == FAIL)
                    {
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
                    if(modbusRTU_.ReadHoldingRegister(iDevAddress, iRegisterAddress + iOffset, 1, readBuf) == FAIL)
                    {
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
                    if(modbusRTU_.ReadHoldingRegister(iDevAddress, iRegisterAddress + iOffset, 1, readBuf) == FAIL)
                    {
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
                    if(modbusRTU_.ReadHoldingRegister(iDevAddress, iRegisterAddress + iOffset, 2, readBuf) == FAIL)
                    {
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
                    if(modbusRTU_.ReadHoldingRegister(iDevAddress, iRegisterAddress + iOffset, 2, readBuf) == FAIL)
                    {
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
                    if(modbusRTU_.ReadHoldingRegister(iDevAddress, iRegisterAddress + iOffset, 2, readBuf) == FAIL)
                    {
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
                    if(modbusRTU_.ReadHoldingRegister(iDevAddress, iRegisterAddress + iOffset, 4, readBuf) == FAIL)
                    {
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
                    if(modbusRTU_.ReadHoldingRegister(iDevAddress, iRegisterAddress + iOffset, 1, readBuf) == FAIL)
                    {
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
                    if(modbusRTU_.ReadReadInputRegister(iDevAddress, iRegisterAddress + iOffset, 1, readBuf) == FAIL)
                    {
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
                    if(modbusRTU_.ReadReadInputRegister(iDevAddress, iRegisterAddress + iOffset, 1, readBuf) == FAIL)
                    {
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
                    if(modbusRTU_.ReadReadInputRegister(iDevAddress, iRegisterAddress + iOffset, 2, readBuf) == FAIL)
                    {
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
                    if(modbusRTU_.ReadReadInputRegister(iDevAddress, iRegisterAddress + iOffset, 2, readBuf) == FAIL)
                    {
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
                    if(modbusRTU_.ReadReadInputRegister(iDevAddress, iRegisterAddress + iOffset, 2, readBuf) == FAIL)
                    {
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
                    if(modbusRTU_.ReadReadInputRegister(iDevAddress, iRegisterAddress + iOffset, 4, readBuf) == FAIL)
                    {
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
                    if(modbusRTU_.ReadReadInputRegister(iDevAddress, iRegisterAddress + iOffset, 1, readBuf) == FAIL)
                    {
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
bool ModbusRTUDevice::LoadData(SaveFormat saveFormat, QString fileName)
{
    pComDevicePrivate_ = new ComDevicePrivate();
    if (pComDevicePrivate_->LoadData(saveFormat, fileName))
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





