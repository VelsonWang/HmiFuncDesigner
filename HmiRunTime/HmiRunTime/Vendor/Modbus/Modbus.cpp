#include "Modbus.h"
#include "../../Public/PublicFunction.h"

Modbus::Modbus(QObject *parent)
    : QObject(parent),
      iFacePort_(nullptr) {
    memset(readDataBuffer_, 0, sizeof(readDataBuffer_)/sizeof(quint8));
    memset(writeDataBuffer_, 0, sizeof(writeDataBuffer_)/sizeof(quint8));
}

Modbus::~Modbus() {

}


void Modbus::setPort(IPort *pPort) {
    if(pPort != nullptr)
        iFacePort_ = pPort;
}


IPort *Modbus::getPort() {
    return iFacePort_;
}


TModbus_CPUMEM Modbus::getCpuMem(const QString &szRegisterArea) {
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
quint8 Modbus::getFuncode(IOTag* pTag, TModbus_ReadWrite rw_flag) {
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
quint16 Modbus::getRegNum(IOTag* pTag) {
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

    //qDebug() << "------------------------------------";
    //for (int i = 0; i < listRead.size(); ++i) {
    //    IOTag* pTag = listRead.at(i);
    //    qDebug() << pTag->GetTagID()
    //             << pTag->getBlockReadTagId()
    //             << pTag->GetRegisterArea()
    //             << pTag->GetRegisterAddress()
    //             << pTag->GetOffset()
    //             << pTag->GetDataTypeLength();
    //}

    vecCm0x.clear();
    vecCm1x.clear();
    vecCm3x.clear();
    vecCm4x.clear();
}



