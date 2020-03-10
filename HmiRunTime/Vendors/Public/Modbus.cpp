#include "Modbus.h"
#include "../../HmiRunTime/Public/PublicFunction.h"
#include "../../HmiRunTime/Vendor.h"

Modbus::Modbus(QObject *parent)
    : QObject(parent),
      iFacePort_(nullptr)
{
}

Modbus::~Modbus()
{

}


void Modbus::setPort(IPort *pPort)
{
    if(pPort != nullptr)
        iFacePort_ = pPort;
}


IPort *Modbus::getPort()
{
    return iFacePort_;
}


TModbus_CPUMEM Modbus::getCpuMem(const QString &szRegisterArea)
{
    if(szRegisterArea == QObject::tr("DO线圈"))
        return CM_0x;
    else if(szRegisterArea == QObject::tr("DI离散输入寄存器"))
        return CM_1x;
    else if(szRegisterArea == QObject::tr("AI输入寄存器"))
        return CM_3x;
    else if(szRegisterArea == QObject::tr("AO保持寄存器"))
        return CM_4x;
    else
        return CM_NON;
}

/**
 * @brief getFuncode
 * @details 获取功能码
 * @param pTag 变量对象
 * @param rw_flag 1-读, 2-写
 * @return 功能码
 */
quint8 Modbus::getFuncode(IOTag* pTag, TModbus_ReadWrite rw_flag)
{
    quint8 byRet = 0;
    TModbus_CPUMEM cm = getCpuMem(pTag->GetRegisterArea());

    if(rw_flag == FLAG_READ) {
        switch(cm) {
        case CM_0x:
            byRet = 0x01;
            break;
        case CM_1x:
            byRet = 0x02;
            break;
        case CM_3x:
            byRet = 0x04;
            break;
        case CM_4x:
            byRet = 0x03;
            break;
        default:
            break;
        }
    } else if(rw_flag == FLAG_WRITE) {
        switch(cm) {
        case CM_0x:
            if(pTag->GetDataTypeLength() == 1) {
                byRet = 0x05;
            } else {
                byRet = 0x0F;
            }
            break;
        case CM_1x:
            //不能写
            break;
        case CM_3x:
            //不能写
            break;
        case CM_4x:
            if(pTag->GetDataTypeLength() == 2) {
                byRet = 0x06;
            } else {
                byRet = 0x10;
            }
            break;
        default:
            break;
        }
    }
    return byRet;
}

/**
 * @brief Modbus::getRegNum
 * @details 获得寄存器个数
 * @param pTag pTag 变量对象
 * @return 寄存器个数
 */
quint16 Modbus::getRegNum(IOTag* pTag)
{
    quint16 byRet = 0;
    TModbus_CPUMEM cm = getCpuMem(pTag->GetRegisterArea());
    switch(cm) {
    case CM_0x:
    case CM_1x:
        if(pTag->GetDataType() == TYPE_BOOL)
            byRet = pTag->GetDataTypeLength();
        else
            byRet = pTag->GetDataTypeLength() * 8;
        break;
    case CM_3x:
    case CM_4x:
        byRet = pTag->GetDataTypeLength() / 2;
        break;
    default:
        break;
    }
    return byRet;
}

/**
 * @brief Modbus::insertBlockReadTagToReadList
 * @details 加入块读变量至待读变量标签列表
 */
void Modbus::insertBlockReadTagToReadList(QList<IOTag *> &listRead)
{
#define CM0X_MAX    (1024)
#define CM1X_MAX    (1024)
#define CM3X_MAX    (100)
#define CM4X_MAX    (100)

    QVector<IOTag*> vecCm0x;
    QVector<IOTag*> vecCm1x;
    QVector<IOTag*> vecCm3x;
    QVector<IOTag*> vecCm4x;

    for (int i = 0; i < listRead.size(); ++i) {
        IOTag* pTag = listRead.at(i);
        TModbus_CPUMEM cm = getCpuMem(pTag->GetRegisterArea());

        switch(cm) {
        case CM_0x:
            vecCm0x.append(pTag);
            break;
        case CM_1x:
            vecCm1x.append(pTag);
            break;
        case CM_3x:
            vecCm3x.append(pTag);
            break;
        case CM_4x:
            vecCm4x.append(pTag);
            break;
        default:
            break;
        }
    }

    if(vecCm0x.size() > 1) {
        int iMin = CM0X_MAX;
        int iMax = 0;
        for (int i = 0; i < vecCm0x.size(); ++i) {
            IOTag* pTag = vecCm0x.at(i);
            int iRegAddr = pTag->GetRegisterAddress() + pTag->GetOffset();
            if(iRegAddr > iMax) iMax = iRegAddr;
            if(iRegAddr < iMin) iMin = iRegAddr;
        }

		int iRange = (iMax - iMin) / CM0X_MAX;
		if ((iMax - iMin) % CM0X_MAX) iRange++;

        IOTag* pTag = vecCm0x.at(0);
        for(int i=0; i< iRange; i++) {
			int iStartAddr = iMin + i * CM0X_MAX;
			QString szTagId = QString("block_0x_%1").arg(i, 4, 16, QChar('0'));
			int iLen = iMax - iStartAddr + 1;

			if (iLen == 0)
			{
				continue;
			}
			else if (iLen > CM0X_MAX)
			{
				iLen = CM0X_MAX;
			}
			IOTag *pIOTag = NULL;
			int tMin, tMax;
            for (int i = 0; i < vecCm0x.size(); ++i) {
                IOTag* pChangeTag = vecCm0x.at(i);
                int iChangeTagAddr = pChangeTag->GetRegisterAddress() + pChangeTag->GetOffset();
                int iChangeTagLen = pChangeTag->GetDataTypeLength();
				if (((iChangeTagAddr >= iStartAddr) && (iChangeTagAddr < iStartAddr + iLen))
					|| ((iChangeTagAddr + iChangeTagLen >= iStartAddr) && (iChangeTagAddr + iChangeTagLen < iStartAddr + iLen))
					)
				{
					if (pIOTag == NULL)
					{
						tMin = iChangeTagAddr;
						tMax = iChangeTagAddr + iChangeTagLen;
						pIOTag = new IOTag();
						pIOTag->SetTagID(szTagId);
						pIOTag->SetDeviceName(pTag->GetDeviceName());
						pIOTag->SetPermissionType(pTag->GetPermissionType());
						pIOTag->SetDeviceAddress(pTag->GetDeviceAddress());
						pIOTag->SetRegisterArea(pTag->GetRegisterArea());
						pIOTag->SetDataType(TYPE_BYTES);
						pIOTag->SetOffset(0);
						pIOTag->SetMaxValue(1);
						pIOTag->SetMinValue(0);
						pIOTag->SetInitializeValue(0);
						pIOTag->SetScale(1);
						pIOTag->SetInFrameAddress(-1);
						pIOTag->setBlockReadTagId("block");
						pIOTag->setReadBlockReadTagSuccess(false);
						pIOTag->setBlockReadTag(Q_NULLPTR);
						listRead.push_front(pIOTag);
					}
					else
					{
						if (iChangeTagAddr < tMin)
						{
							tMin = iChangeTagAddr;
						}
						if (iChangeTagAddr + iChangeTagLen > tMax)
						{
							tMax = iChangeTagAddr + iChangeTagLen;
						}
					}
                    pChangeTag->setBlockReadTagId(szTagId);
                    pChangeTag->setReadBlockReadTagSuccess(false);
                    pChangeTag->setBlockReadTag(pIOTag);
                }
			}
			if (pIOTag)
			{
				iLen = tMax - tMin;
				int bufLen = iLen / 8;
				if (iLen % 8) bufLen++;
				pIOTag->SetRegisterAddress(tMin);
				pIOTag->SetTagBufferLength(bufLen);
			}
        }
    }

    if(vecCm1x.size() > 1) {
        int iMin = CM1X_MAX;
        int iMax = 0;
        for (int i = 0; i < vecCm1x.size(); ++i) {
            IOTag* pTag = vecCm1x.at(i);
            int iRegAddr = pTag->GetRegisterAddress() + pTag->GetOffset();
            if(iRegAddr > iMax) iMax = iRegAddr;
            if(iRegAddr < iMin) iMin = iRegAddr;
        }

		int iRange = (iMax - iMin) / CM1X_MAX;
		if ((iMax - iMin) % CM1X_MAX) iRange++;

        IOTag* pTag = vecCm1x.at(0);
        for(int i=0; i< iRange; i++) {
			int iStartAddr = iMin + i * CM1X_MAX;
			QString szTagId = QString("block_1x_%1").arg(i, 4, 16, QChar('0'));
			int iLen = iMax - iStartAddr + 1;

			if (iLen == 0)
			{
				continue;
			}
			else if (iLen > CM1X_MAX)
			{
				iLen = CM1X_MAX;
			}
			IOTag *pIOTag = NULL;
			int tMin, tMax;
            for (int i = 0; i < vecCm1x.size(); ++i) {
                IOTag* pChangeTag = vecCm1x.at(i);
                int iChangeTagAddr = pChangeTag->GetRegisterAddress() + pChangeTag->GetOffset();
                int iChangeTagLen = pChangeTag->GetDataTypeLength();
				if (((iChangeTagAddr >= iStartAddr) && (iChangeTagAddr < iStartAddr + iLen))
					|| ((iChangeTagAddr + iChangeTagLen >= iStartAddr) && (iChangeTagAddr + iChangeTagLen < iStartAddr + iLen))
					)
				{
					if (pIOTag == NULL)
					{
						tMin = iChangeTagAddr;
						tMax = iChangeTagAddr + iChangeTagLen;
						pIOTag = new IOTag();
						pIOTag->SetTagID(szTagId);
						pIOTag->SetDeviceName(pTag->GetDeviceName());
						pIOTag->SetPermissionType(pTag->GetPermissionType());
						pIOTag->SetDeviceAddress(pTag->GetDeviceAddress());
						pIOTag->SetRegisterArea(pTag->GetRegisterArea());
						pIOTag->SetDataType(TYPE_BYTES);
						pIOTag->SetOffset(0);
						pIOTag->SetMaxValue(1);
						pIOTag->SetMinValue(0);
						pIOTag->SetInitializeValue(0);
						pIOTag->SetScale(1);
						pIOTag->SetInFrameAddress(-1);
						pIOTag->setBlockReadTagId("block");
						pIOTag->setReadBlockReadTagSuccess(false);
						pIOTag->setBlockReadTag(Q_NULLPTR);
						listRead.push_front(pIOTag);
					}
					else
					{
						if (iChangeTagAddr < tMin)
						{
							tMin = iChangeTagAddr;
						}
						if (iChangeTagAddr + iChangeTagLen > tMax)
						{
							tMax = iChangeTagAddr + iChangeTagLen;
						}
					}
                    pChangeTag->setBlockReadTagId(szTagId);
                    pChangeTag->setReadBlockReadTagSuccess(false);
                    pChangeTag->setBlockReadTag(pIOTag);
                }
            }
			if (pIOTag)
			{
				iLen = tMax - tMin;
				int bufLen = iLen / 8;
				if (iLen % 8) bufLen++;
				pIOTag->SetRegisterAddress(tMin);
				pIOTag->SetTagBufferLength(bufLen);
			}
		}
    }

    if(vecCm3x.size() > 1) {
        int iMin = CM3X_MAX;
        int iMax = 0;
        for (int i = 0; i < vecCm3x.size(); ++i) {
            IOTag* pTag = vecCm3x.at(i);
			int iRegAddr = pTag->GetRegisterAddress() + pTag->GetOffset();
			int iRegLen = pTag->GetDataTypeLength();
			iRegLen = iRegLen / 2 + (iRegLen % 2);
			if (iRegAddr < iMin) iMin = iRegAddr;
			if (iRegAddr + iRegLen > iMax) iMax = iRegAddr + iRegLen;
		}

		int iRange = (iMax - iMin) / CM3X_MAX;
		if ((iMax - iMin) % CM3X_MAX) iRange++;

        IOTag* pTag = vecCm3x.at(0);
		for(int i=0; i< iRange; i++) {
			int iStartAddr = iMin + i * CM3X_MAX;
			QString szTagId = QString("block_3x_%1").arg(i, 4, 16, QChar('0'));
			int iLen = iMax - iStartAddr;

			if (iLen == 0)
			{
				continue;
			}
			else if (iLen > CM3X_MAX)
			{
				iLen = CM3X_MAX;
			}
            IOTag *pIOTag = NULL;
			int tMin, tMax;
			for (int i = 0; i < vecCm3x.size(); ++i) {
                IOTag* pChangeTag = vecCm3x.at(i);
                int iChangeTagAddr = pChangeTag->GetRegisterAddress() + pChangeTag->GetOffset();
				int iChangeTagLen = pChangeTag->GetDataTypeLength();
				iChangeTagLen = iChangeTagLen / 2 + iChangeTagLen % 2;
				if (((iChangeTagAddr >= iStartAddr) && (iChangeTagAddr < iStartAddr + iLen))
					|| ((iChangeTagAddr + iChangeTagLen >= iStartAddr) && (iChangeTagAddr + iChangeTagLen < iStartAddr + iLen))
					)
				{
					if (pIOTag == NULL)
					{
						tMin = iChangeTagAddr;
						tMax = iChangeTagAddr + iChangeTagLen;
						pIOTag = new IOTag();
						pIOTag->SetTagID(szTagId);
						pIOTag->SetDeviceName(pTag->GetDeviceName());
						pIOTag->SetPermissionType(pTag->GetPermissionType());
						pIOTag->SetDeviceAddress(pTag->GetDeviceAddress());
						pIOTag->SetRegisterArea(pTag->GetRegisterArea());
						pIOTag->SetDataType(TYPE_BYTES);
						pIOTag->SetOffset(0);
						pIOTag->SetMaxValue(65536);
						pIOTag->SetMinValue(0);
						pIOTag->SetInitializeValue(0);
						pIOTag->SetScale(1);
						pIOTag->SetInFrameAddress(-1);
						pIOTag->setBlockReadTagId("block");
						pIOTag->setReadBlockReadTagSuccess(false);
						pIOTag->setBlockReadTag(Q_NULLPTR);
						listRead.push_front(pIOTag);
					}
					else
					{
						if (iChangeTagAddr < tMin)
						{
							tMin = iChangeTagAddr;
						}
						if (iChangeTagAddr + iChangeTagLen > tMax)
						{
							tMax = iChangeTagAddr + iChangeTagLen;
						}
					}
					pChangeTag->setBlockReadTagId(szTagId);
                    pChangeTag->setReadBlockReadTagSuccess(false);
                    pChangeTag->setBlockReadTag(pIOTag);
                }
			}
			if (pIOTag)
			{
				iLen = tMax - tMin;
				int bufLen = iLen * 2;
				pIOTag->SetRegisterAddress(tMin);
				pIOTag->SetTagBufferLength(bufLen);
			}
		}
    }

    if(vecCm4x.size() > 1) {
        int iMin = CM4X_MAX;
        int iMax = 0;
		//寻找寄存器边界
        for (int i = 0; i < vecCm4x.size(); ++i) {
            IOTag* pTag = vecCm4x.at(i);
            int iRegAddr = pTag->GetRegisterAddress() + pTag->GetOffset();
			int iRegLen= pTag->GetDataTypeLength();
			iRegLen = iRegLen / 2 + (iRegLen % 2);
			if (iRegAddr < iMin) iMin = iRegAddr;
			if(iRegAddr + iRegLen > iMax) iMax = iRegAddr + iRegLen;
        }

        int iRange = (iMax - iMin) / CM4X_MAX;
        if((iMax - iMin) % CM4X_MAX) iRange++;

        IOTag* pTag = vecCm4x.at(0);
        for(int i=0; i< iRange; i++) {
			int iStartAddr = iMin + i * CM4X_MAX;
			QString szTagId = QString("block_4x_%1").arg(i, 4, 16, QChar('0'));
			int iLen = iMax - iStartAddr;

			if (iLen == 0)
			{
				continue;
			}
			else if (iLen > CM4X_MAX)
			{
				iLen = CM4X_MAX;
			}
			IOTag *pIOTag = NULL;
			int tMin, tMax;
			for (int k = 0; k < vecCm4x.size(); ++k) {
                IOTag* pChangeTag = vecCm4x.at(k);
                int iChangeTagAddr = pChangeTag->GetRegisterAddress() + pChangeTag->GetOffset();
                int iChangeTagLen = pChangeTag->GetDataTypeLength();
				iChangeTagLen = iChangeTagLen / 2 + iChangeTagLen % 2;
                if(  ((iChangeTagAddr >= iStartAddr) && (iChangeTagAddr < iStartAddr+iLen)) 
					|| ((iChangeTagAddr + iChangeTagLen >= iStartAddr) && (iChangeTagAddr + iChangeTagLen < iStartAddr + iLen))
					)
				{
					if (pIOTag == NULL)
					{
						tMin = iChangeTagAddr;
						tMax = iChangeTagAddr + iChangeTagLen;
						pIOTag = new IOTag();
						pIOTag->SetTagID(szTagId);
						pIOTag->SetDeviceName(pTag->GetDeviceName());
						pIOTag->SetPermissionType(pTag->GetPermissionType());
						pIOTag->SetDeviceAddress(pTag->GetDeviceAddress());
						pIOTag->SetRegisterArea(pTag->GetRegisterArea());
						pIOTag->SetDataType(TYPE_BYTES);
						pIOTag->SetOffset(0);
						pIOTag->SetMaxValue(65536);
						pIOTag->SetMinValue(0);
						pIOTag->SetInitializeValue(0);
						pIOTag->SetScale(1);
						pIOTag->SetInFrameAddress(-1);
						pIOTag->setBlockReadTagId("block");
						pIOTag->setReadBlockReadTagSuccess(false);
						pIOTag->setBlockReadTag(Q_NULLPTR);
						listRead.push_front(pIOTag);
					}
					else
					{
						if (iChangeTagAddr < tMin)
						{
							tMin = iChangeTagAddr;
						}
						if (iChangeTagAddr + iChangeTagLen > tMax)
						{
							tMax = iChangeTagAddr + iChangeTagLen;
						}
					}
                    pChangeTag->setBlockReadTagId(szTagId);
                    pChangeTag->setReadBlockReadTagSuccess(false);
                    pChangeTag->setBlockReadTag(pIOTag);
                }
            }
			if (pIOTag)
			{
				iLen = tMax - tMin;
				int bufLen = iLen * 2;
				pIOTag->SetRegisterAddress(tMin);
				pIOTag->SetTagBufferLength(bufLen);
			}
        }
    }
#if 0
    qDebug() << "------------------------------------";
    for (int i = 0; i < listRead.size(); ++i) {
        IOTag* pTag = listRead.at(i);
        qDebug() << pTag->GetTagID()
                 << pTag->getBlockReadTagId()
                 << pTag->GetRegisterArea()
                 << pTag->GetRegisterAddress()
                 << pTag->GetOffset()
                 << pTag->GetDataTypeLength();
    }
#endif
    vecCm0x.clear();
    vecCm1x.clear();
    vecCm3x.clear();
    vecCm4x.clear();
}



#if 0

例如一个数据int32=0x1234 5678，启用后：
8位逆序 int32=>0x21436587 +
16位低字节在前高字节在后 int32=>0x43 21 87 65 +
32位低字节在前高字节在后 int32=>0x8765 4321  +
64位低字节在前高字节在后 int32=>0x87654321

#endif


///
/// \brief Modbus::modbusChangeData
/// \details 依据规则修改缓冲区内数据
/// \param bAddr8 8位逆序
/// \param bAddr16 16位低字节在前高字节在后
/// \param bAddr32 32位低字节在前高字节在后
/// \param bAddr64 64位低字节在前高字节在后
/// \param pData 数据缓存
/// \param iLen 待改变的数据长度
///
void Modbus::modbusChangeData(bool bAddr8, bool bAddr16, bool bAddr32,
                              bool bAddr64, quint8* pData, quint32 iLen)
{
    quint8 buffer[32] = {0};
    memcpy((void *)buffer, (void *)pData, iLen);
    // 8位逆序
    if(bAddr8) {
        buffer[0] = byteSwitchHigh4bitLow4bit(buffer[0]);
        if(iLen >= 2) {
            buffer[1] = byteSwitchHigh4bitLow4bit(buffer[1]);
        }
        if(iLen >= 4) {
            buffer[2] = byteSwitchHigh4bitLow4bit(buffer[2]);
            buffer[3] = byteSwitchHigh4bitLow4bit(buffer[3]);
        }
        if(iLen >= 8) {
            buffer[4] = byteSwitchHigh4bitLow4bit(buffer[4]);
            buffer[5] = byteSwitchHigh4bitLow4bit(buffer[5]);
            buffer[6] = byteSwitchHigh4bitLow4bit(buffer[6]);
            buffer[7] = byteSwitchHigh4bitLow4bit(buffer[7]);
        }
    }
    // 16位低字节在前高字节在后
    if(bAddr16) {
        quint8 tmp = 0;
        if(iLen >= 2) {
            tmp = buffer[0];
            buffer[0] = buffer[1];
            buffer[1] = tmp;
        }
        if(iLen >= 4) {
            tmp = buffer[2];
            buffer[2] = buffer[3];
            buffer[3] = tmp;
        }
        if(iLen >= 8) {
            tmp = buffer[4];
            buffer[4] = buffer[5];
            buffer[5] = tmp;
            tmp = buffer[6];
            buffer[6] = buffer[7];
            buffer[7] = tmp;
        }
    }
    // 32位低字节在前高字节在后
    if(bAddr32) {
        quint8 tmp0 = 0;
        quint8 tmp1 = 0;
        if(iLen >= 4) {
            tmp0 = buffer[0];
            tmp1 = buffer[1];
            buffer[0] = buffer[2];
            buffer[1] = buffer[3];
            buffer[2] = tmp0;
            buffer[3] = tmp1;
        }
        if(iLen >= 8) {
            tmp0 = buffer[4];
            tmp1 = buffer[5];
            buffer[4] = buffer[6];
            buffer[5] = buffer[7];
            buffer[6] = tmp0;
            buffer[7] = tmp1;
        }
    }

    // 64位低字节在前高字节在后
    if(bAddr64) {
        quint8 tmp0 = 0;
        quint8 tmp1 = 0;
        quint8 tmp2 = 0;
        quint8 tmp3 = 0;
        if(iLen >= 8) {
            tmp0 = buffer[0];
            tmp1 = buffer[1];
            tmp2 = buffer[2];
            tmp3 = buffer[3];
            buffer[0] = buffer[4];
            buffer[1] = buffer[5];
            buffer[2] = buffer[6];
            buffer[3] = buffer[7];
            buffer[4] = tmp0;
            buffer[5] = tmp1;
            buffer[6] = tmp2;
            buffer[7] = tmp3;
        }
    }
    memcpy((void *)pData, (void *)buffer, iLen);
}


///
/// \brief Modbus::isStartAddrBit
/// \details 内存地址起始位是否为0
/// \param pObj 设备描述对象
/// \return true-内存地址起始位为0, false-内存地址起始位为1
///
bool Modbus::isStartAddrBit(void *pObj)
{
    Vendor* pVendorObj = (Vendor*)(pObj);
    bool bStartAddrBit = pVendorObj->m_pVendorPrivateObj->m_mapProperties.value("startAddrBit").toBool();
    return bStartAddrBit;
}

///
/// \brief Modbus::isWriteCoilFn
/// \details 写线圈功能码是否为15
/// \param pObj 设备描述对象
/// \return true-写线圈功能码为15, false-写线圈功能码为5
///
bool Modbus::isWriteCoilFn(void *pObj)
{
    Vendor* pVendorObj = (Vendor*)(pObj);
    bool bWriteCoilFn = pVendorObj->m_pVendorPrivateObj->m_mapProperties.value("writeCoilFn").toBool();
    return bWriteCoilFn;
}

///
/// \brief Modbus::isWriteRegFn
/// \details 写寄存器功能码为是否16
/// \param pObj 设备描述对象
/// \return true-写寄存器功能码为16, false-写寄存器功能码为6
///
bool Modbus::isWriteRegFn(void *pObj)
{
    Vendor* pVendorObj = (Vendor*)(pObj);
    bool bWriteRegFn = pVendorObj->m_pVendorPrivateObj->m_mapProperties.value("writeRegFn").toBool();
    return bWriteRegFn;
}

///
/// \brief Modbus::isAddr8
/// \details 是否8位逆序
/// \param pObj 设备描述对象
/// \return true-启用8位逆序, false-不启用8位逆序
///
bool Modbus::isAddr8(void *pObj)
{
    Vendor* pVendorObj = (Vendor*)(pObj);
    bool bAddr8 = pVendorObj->m_pVendorPrivateObj->m_mapProperties.value("addr8").toBool();
    return bAddr8;
}

///
/// \brief Modbus::isAddr16
/// \details 是否16位低字节在前高字节在后
/// \param pObj 设备描述对象
/// \return true-16位低字节在前高字节在后, false-16位高字节在前低字节在后
///
bool Modbus::isAddr16(void *pObj)
{
    Vendor* pVendorObj = (Vendor*)(pObj);
    bool bAddr16 = pVendorObj->m_pVendorPrivateObj->m_mapProperties.value("addr16").toBool();
    return bAddr16;
}

///
/// \brief Modbus::isAddr32
/// \details 是否32位低字节在前高字节在后
/// \param pObj 设备描述对象
/// \return true-32位低字节在前高字节在后, false-32位高字节在前低字节在后
///
bool Modbus::isAddr32(void *pObj)
{
    Vendor* pVendorObj = (Vendor*)(pObj);
    bool bAddr32 = pVendorObj->m_pVendorPrivateObj->m_mapProperties.value("addr32").toBool();
    return bAddr32;
}

///
/// \brief Modbus::isAddr64
/// \details 是否64位低字节在前高字节在后
/// \param pObj 设备描述对象
/// \return true-64位低字节在前高字节在后, false-64位高字节在前低字节在后
///
bool Modbus::isAddr64(void *pObj)
{
    Vendor* pVendorObj = (Vendor*)(pObj);
    bool bAddr64 = pVendorObj->m_pVendorPrivateObj->m_mapProperties.value("addr64").toBool();
    return bAddr64;
}




