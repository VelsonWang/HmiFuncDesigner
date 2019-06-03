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




