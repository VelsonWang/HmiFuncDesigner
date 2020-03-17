#include "SiemensS7_200Impl.h"
#include "DataPack.h"
#include "../../HmiRunTimeData/Public/PublicFunction.h"
#include "../../HmiRunTimeData/Vendor.h"
#include <string.h>
#include <QObject>
#include <QThread>

////////////////////////////////////////////////////////////////////////////////
// 数据交互过程:
// 1. 首先主机发出读写申请命令
// 2. PLC发出接收正确的响应
// 3. 主机发出确认命令
// 4. PLC回应给主机数据
////////////////////////////////////////////////////////////////////////////////


// 单机通讯功能码用 0x6C，双机通讯时各自串口独立时功能码用 0x6C
// 一个串口上接两台plc通讯时功能码用 0x5C 0x7C交替通讯
quint8 g_ucSimaticFC = 0x6C;

typedef enum _TCPUMEM
{
    CM_NON  =   0,
    CM_V    =   1,
    CM_I    =   2,
    CM_Q    =   3,
    CM_M    =   4,
    CM_T    =   5,
    CM_C    =   6,
    CM_HC   =   7,
    CM_AI   =   8,
    CM_AQ   =   9,
    CM_SM   =   10,
    CM_S    =   11
}TCPUMEM;

/////////////////////////////////////////////
// 主站设备地址编号
static quint16 g_wHmiAddress = 1;

// 读写共用协议头
typedef struct _TWriteReadData
{
    quint8 	bySD4;		// 开始定界符(68H)
    quint8 	byLE;		// 报文数据长度(DA到DU末)
    quint8 	byLER;		// 重复报文数据长度
    quint8	_bySD4;		// 开始定界符(68H)
    quint8 	byDA;		// 目的设备地址
    quint8 	bySA;		// 源设备地址
    quint8	byFC;		// 功能码
    quint8 	byDSAP;		// 目的服务存取点
    quint8 	bySSAP;		// 源服务存取点
    quint8 *pDU;		// 数据块
    quint8	byFCS;		// 校验码(DA到DU末数据的和, 取末字节值)
    quint8	byED;		// 结束定界符(16H)
} __attribute__ ((packed)) TWriteReadData, *PWriteReadData;

// 读写确认命令帧结构
typedef struct _TWriteReadAck
{
    quint8	bySD1;		// 开始定界符(10H)
    quint8	byDA;		// 目的(PLC)地址
    quint8	bySA;		// 源(HMI)地址
    quint8	byFC;		// 功能码(5CH)
    quint8 	byFCS;		// 校验和(bySA + byDA + byFC的末字节)
    quint8 	byED;		// 结束符号(16H)
}__attribute__ ((packed)) TWriteReadAck, *PWriteReadAck;
/*******************************************/

// 写申请数据定义
typedef struct _TWDataUnit
{
    quint32 dwReserved;		// 保留双字, 值为0
    quint16 wMsgLen;		// 消息长度, 从byOpType字段开始到变量地址的长度
    quint16 wDataLen;		// 数据长度, 从变量地址后到pVailableData字段的长度
    quint8 byOpType;		// 操作类型(04H:读  05H:写)
    quint8 byMsgGroup;		// 消息组数(一般使用1组消息)
    quint8 byMsgHead;		// 消息头(固定为12H)
    quint8 byMsgLen;		// 消息长度(从下一个字节到变量地址的长度)
    quint8 byMsgOp;			// 消息类型(固定为10H)
    quint8 byDataType;		// 01H bit, 02H byte, 04H word, 06H quint32
    quint16 wWriteDataLen;	// 读写数量(byDataType为单位, 最大不能超过DEH个字节)
    quint16 wAddrType;		// 地址类型(V范围为0001H, 其他为0000H)
    quint32 dwAddr;			// 变量地址

    /////////////////////////////////////////////////////
    quint8 byReserved;		// 保留字节, 值为0
    quint8 byVailableType;	// 有效数据类型(03H: bit  04H: 非bit)
    quint16 wVailableBitLen;	// 有效bit数(有效数据以bit为单位进行统计)
    quint8 * pVailableData;	// 有效数据,高位在前(如VW: 0x123存储为0x0123)
}__attribute__ ((packed)) TWDataUnit, *PWDataUnit;

static void Series_WDataUnit(PWDataUnit pWDataUnit)
{
    RecoverSelfData((quint8 *)&pWDataUnit->wMsgLen, 2);
    RecoverSelfData((quint8 *)&pWDataUnit->wDataLen, 2);
    RecoverSelfData((quint8 *)&pWDataUnit->wWriteDataLen, 2);
    RecoverSelfData((quint8 *)&pWDataUnit->wAddrType, 2);
    RecoverSelfData((quint8 *)&pWDataUnit->dwAddr, 4);
    RecoverSelfData((quint8 *)&pWDataUnit->wVailableBitLen, 2);
}

// 写返回数据定义
typedef struct _TWRetDataUnit
{
    quint32 dwReserved;		// 保留双字, 值为0
    quint16 wMsgLen;		// 消息长度, 从下一个字段开始到固定字符FF之前的长度
    quint16 wDataLen;		// 数据长度, 从固定字符FF到校验字节之前的长度,固定为1
    quint16 wRetFlag;		// 返回数据标记: 固定为0
    quint8 byOpType;		// 操作类型(04H:读  05H:写)
    quint8 byMsgGroup;		// 消息组数(一般使用1组消息)
    quint8 byMsgHead;		// 消息头(FF: 正确  05:  出错)
}__attribute__ ((packed)) TWRetDataUnit, *PWRetDataUnit;

static void Series_WRetDataUnit(PWRetDataUnit pWRetDataUnit) {
    RecoverSelfData((quint8 *)&pWRetDataUnit->wMsgLen, 2);
    RecoverSelfData((quint8 *)&pWRetDataUnit->wDataLen, 2);
    RecoverSelfData((quint8 *)&pWRetDataUnit->wRetFlag, 2);
}

// 读申请数据定义
typedef struct _TRDataUnit
{
    quint32 dwReserved;		// 保留双字, 值为0
    quint16 wMsgLen;		// 消息长度, 从byOpType字段开始到变量地址的长度
    quint16 wDataLen;		// 数据长度, 从变量地址后到校验和字段的长度(0)
    quint8 byOpType;		// 操作类型(04H:读  05H:写)
    quint8 byMsgGroup;		// 消息组数(一般使用1组消息)
    quint8 * pMsgData;		// 发送消息数据
}__attribute__ ((packed)) TRDataUnit, *PRDataUnit;

static void Series_RDataUnit(PRDataUnit pRDataUnit) {
    RecoverSelfData((quint8 *)&pRDataUnit->wMsgLen, 2);
    RecoverSelfData((quint8 *)&pRDataUnit->wDataLen, 2);
}

typedef struct _TRDataMsgUnit
{
    quint8 byMsgHead;		// 消息头(固定为12H)
    quint8 byMsgLen;		// 消息长度(从下一个字节到变量地址的长度)
    quint8 byMsgOp;			// 消息类型(固定为10H)
    quint8 byDataType;		// 01H: bit, 02H: byte, 04H: word, 06H: quint32
                            // 1E: C区, 1F: T区  20H: HC区
    quint16 wReadDataLen;	// 读写数量(byDataType为单位, 最大不能超过DEH个字节)
    quint16 wAddrType;		// 地址类型(V范围为0001H, 其他为0000H)
    quint32 dwAddr;			// 变量地址
}__attribute__ ((packed)) TRDataMsgUnit, *PRDataMsgUnit;

static void Series_RDataMsgUnit(PRDataMsgUnit pRDataMsgUnit) {
    RecoverSelfData((quint8 *)&pRDataMsgUnit->wReadDataLen, 2);
    RecoverSelfData((quint8 *)&pRDataMsgUnit->wAddrType, 2);
    RecoverSelfData((quint8 *)&pRDataMsgUnit->dwAddr, 4);
}


// 读返回数据定义
typedef struct _TRRetDataUnit
{
    quint32 dwReserved;		// 保留双字, 值为0
    quint16 wMsgLen;		// 消息长度, 从下一个字段开始到固定字符FF之前的长度
    quint16 wDataLen;		// 数据长度, 从固定字符FF到校验字节之前的长度,固定为1
    quint16 wRetFlag;		// 返回数据标记: 固定为0
    quint8 byOpType;		// 操作类型(04H:读  05H:写)
    quint8 byMsgGroup;		// 消息组数(一般使用1组消息)
}__attribute__ ((packed)) TRRetDataUnit, *PRRetDataUnit;

static void Series_RRetDataUnit(PRRetDataUnit pRRetDataUnit) {
    RecoverSelfData((quint8 *)&pRRetDataUnit->wMsgLen, 2);
    RecoverSelfData((quint8 *)&pRRetDataUnit->wDataLen, 2);
    RecoverSelfData((quint8 *)&pRRetDataUnit->wRetFlag, 2);
}

typedef struct _TRRetDataMsgUnit
{
    quint8 byMsgHead;		// 消息头(FF: 正确  05: 出错)
    quint8 byDataType;		// 03H: bit, 04H: byte,word,quint32;
                            // 09H: T,C,HC区数据
    quint16 wVailableBitLen;	// 有效数据长度(T,C,HC单位为BYTE,否则为bit)
}__attribute__ ((packed)) TRRetDataMsgUnit, *PRRetDataMsgUnit;

static void Series_RRetDataMsgUnit(PRRetDataMsgUnit pRRetDataMsgUnit) {
    RecoverSelfData((quint8 *)&pRRetDataMsgUnit->wVailableBitLen, 2);
}


// 最大传输帧长度
#define MAX_TRANS_DATA_LEN		249
// 命令帧长
#define MAX_WRITE_DATA_LEN		212
// 应答帧长
#define MAX_READ_DATA_LEN		222
// 写申请命令数据包长度
#define WRITE_REQ_LEN			26
// 写应答数据帧长度
#define WRITE_REPLAY_LEN		24

#define WRITE_DELAY  20
#define READ_DELAY   20
#define rotl(x,n) (((x)<<((int)(n))) | ((x)>>(32-(int)(n))))

static quint8 g_byFC = 0x5C;
static quint32 g_Count = 0x00;

static quint32 ReverseDwordByte(quint32 value)
{
    value = ((value & 0xFF00FF00) >> 8) | ((value & 0x00FF00FF) << 8);
    return rotl(value,16);
}


static quint32 GetBeginAddrAsCpuMem(TCPUMEM cm)
{
    quint32 dwRet = 0;

    switch(cm)
    {
    case CM_V:
        dwRet = 0x84000000;
        break;
    case CM_I:
        dwRet = 0x81000000;
        break;
    case CM_Q:
        dwRet= 0x82000000;
        break;
    case CM_M:
        dwRet = 0x83000000;
        break;
    case CM_SM:
        dwRet = 0x05000000;
        break;
    case CM_S:
        dwRet = 0x04000000;
        break;
    case CM_T:
        dwRet = 0x1F000000;
        break;
    case CM_C:
        dwRet = 0x1E000000;
        break;
    case CM_HC:
        dwRet = 0x20000000;
        break;
    case CM_AI:
        dwRet = 0x06000000;
        break;
    case CM_AQ:
        dwRet = 0x07000000;
        break;
    default:
        break;
    }

    return dwRet;
}


static TCPUMEM getCpuMem(const QString &szRegisterArea)
{
    if(szRegisterArea == QObject::tr("V"))
        return CM_V;
    else if(szRegisterArea == QObject::tr("I"))
        return CM_I;
    else if(szRegisterArea == QObject::tr("Q"))
        return CM_Q;
    else if(szRegisterArea == QObject::tr("M"))
        return CM_M;
    else if(szRegisterArea == QObject::tr("T"))
        return CM_T;
    else if(szRegisterArea == QObject::tr("C"))
        return CM_C;
    else if(szRegisterArea == QObject::tr("HC"))
        return CM_HC;
    else if(szRegisterArea == QObject::tr("AI"))
        return CM_AI;
    else if(szRegisterArea == QObject::tr("AQ"))
        return CM_AQ;
    else
        return CM_NON;
}


SiemensS7_200Impl::SiemensS7_200Impl() : iFacePort_(nullptr)
{
}


SiemensS7_200Impl::~SiemensS7_200Impl()
{
}


void SiemensS7_200Impl::setPort(IPort *pPort)
{
    if(pPort != nullptr)
        iFacePort_ = pPort;
}


IPort *SiemensS7_200Impl::getPort()
{
    return iFacePort_;
}


/**
 * @brief SiemensS7_200Impl::isCanWrite
 * @details 判断该变量定义属性是否可以写
 * @param pObj 设备描述对象
 * @param pTag 设备变量
 * @return false-不可写，true-可写
 */
bool SiemensS7_200Impl::isCanWrite(void* pObj, IOTag* pTag)
{
    (void)pObj;
    qint32 iDataLen = pTag->GetDataTypeLength();
    if((iDataLen > MAX_WRITE_DATA_LEN) || (iDataLen == 0))
        return false;

    if(getCpuMem(pTag->GetRegisterArea()) == CM_NON)
        return false;

    // T、C区的BOOL变量类型不能写
    if((getCpuMem(pTag->GetRegisterArea()) == CM_T || getCpuMem(pTag->GetRegisterArea()) == CM_C) &&
            pTag->GetDataType() == TYPE_BOOL)
        return false;

    // HC、AI区不能写
    if(getCpuMem(pTag->GetRegisterArea()) == CM_HC || getCpuMem(pTag->GetRegisterArea()) == CM_AI)
        return false;

    // SM0-SM29不能写
    if(getCpuMem(pTag->GetRegisterArea()) == CM_SM) {
        if((pTag->GetRegisterAddress() + pTag->GetOffset()) <= 29)
            return false;
    }

    return true;
}


/**
 * @brief SiemensS7_200Impl::writeData
 * @details 写一个变量值到plc设备
 * @param pObj 设备描述对象
 * @param pTag 设备变量
 * @return 0-失败, 1-成功
 */
int SiemensS7_200Impl::writeData(void* pObj, IOTag* pTag)
{
    Vendor* pVendorObj = (Vendor*)(pObj);
    int iCommTimeout = pVendorObj->m_pVendorPrivateObj->m_iCommTimeout;

    memset(pVendorObj->writeBuf, 0, sizeof(pVendorObj->writeBuf)/sizeof(quint8));
    memset(pVendorObj->readBuf, 0, sizeof(pVendorObj->readBuf)/sizeof(quint8));

    // 发送数据结构
    TWDataUnit stWDataUnit = {0x00, 0x0E, 0x00, 0x05, 0x01, 0x12, 0x0A, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, Q_NULLPTR};
    TWriteReadData stWriteData = {0x68, 0x00, 0x00, 0x68, 0x00, 0x00, 0x7C, 0x32, 0x01, Q_NULLPTR, 0x00, 0x16};
    TWriteReadAck stWriteAck = {0x10, 0x00, 0x00, 0x5C, 0x00, 0x16};

    // 被写数据指针
    quint8 *pVarVailableData = (quint8 *)pTag->pWriteBuf;

    TWriteReadData WriteRetData;
    TWRetDataUnit WRetDataUnit;

    quint16 wTemp= 0;
    quint32 dwTemp = 0;
    quint8 byTemp = 0;
    quint8 _byTemp = 0;
    quint8 aDatabuf[512];

    memset(aDatabuf, 0, 512);
    memset(pVendorObj->writeBuf, 0, MAX_TRANS_DATA_LEN);

    stWriteData.pDU = (quint8 *)&stWDataUnit;

    // 设置通信目的地址和源地址
    stWriteData.byDA = (quint8)pTag->GetDeviceAddress();
    stWriteData.bySA = (quint8)g_wHmiAddress;

    // 设置功能码 FC
    if(g_ucSimaticFC != 0x6c) {
        if(g_byFC == 0x5C) {
            stWriteData.byFC = 0x7C;
            g_byFC = 0x7C;
        } else {
            stWriteData.byFC = 0x5C;
            g_byFC = 0x5C;
        }
    } else {
        stWriteData.byFC = 0x6C;
    }

    g_Count++;
    if(g_Count >= 256) g_Count = 0x01;

    stWDataUnit.dwReserved = g_Count + (g_Count*256);
    stWDataUnit.dwReserved = ReverseDwordByte(stWDataUnit.dwReserved);

    // 填写数据块
    wTemp = 0;
    dwTemp = 0;

    stWDataUnit.wWriteDataLen = 0x01;
    stWDataUnit.byVailableType = 0x04;

    if(getCpuMem(pTag->GetRegisterArea()) == CM_T) {
        stWDataUnit.byDataType = 0x1F;
        stWDataUnit.wVailableBitLen = 8 * 5;
        wTemp = 5;
        dwTemp = pTag->GetRegisterAddress() + pTag->GetOffset();
        RecoverData(pVarVailableData, pVendorObj->writeBuf + 3, 2);
    } else if(getCpuMem(pTag->GetRegisterArea()) == CM_C) {
        stWDataUnit.byDataType = 0x1E;
        stWDataUnit.wVailableBitLen = 8 * 3;
        wTemp = 3;
        dwTemp = pTag->GetRegisterAddress() + pTag->GetOffset();
        RecoverData(pVarVailableData, pVendorObj->writeBuf + 1, 2);
    } else if(pTag->GetDataType() == TYPE_BOOL) {
        stWDataUnit.byDataType = 0x01;
        stWDataUnit.byVailableType = 0x03;
        stWDataUnit.wVailableBitLen = 1;
        wTemp = 1;
        dwTemp = pTag->GetRegisterAddress() * 8 + pTag->GetOffset();

        if(*(quint8 *)pVarVailableData) pVendorObj->writeBuf[0] = 0x01;
        else pVendorObj->writeBuf[0] = 0x00;
    } else if(pTag->GetDataType() == TYPE_INT8 || pTag->GetDataType() == TYPE_UINT8) {
        stWDataUnit.byDataType = 0x02;
        wTemp = (quint16)pTag->GetDataTypeLength();
        dwTemp = (pTag->GetRegisterAddress() + pTag->GetOffset()) * 8;
        stWDataUnit.wWriteDataLen = pTag->GetDataTypeLength();
        stWDataUnit.wVailableBitLen = 8 * pTag->GetDataTypeLength();
        memcpy(pVendorObj->writeBuf, pVarVailableData, stWDataUnit.wWriteDataLen);
    } else if(pTag->GetDataType() == TYPE_INT16 || pTag->GetDataType() == TYPE_UINT16) {
        stWDataUnit.byDataType = 0x04;
        wTemp = 2;
        dwTemp = (pTag->GetRegisterAddress() + pTag->GetOffset()) * 8;
        stWDataUnit.wVailableBitLen = 8 * 2;
        RecoverData(pVarVailableData, pVendorObj->writeBuf, 2);
    } else if(pTag->GetDataType() == TYPE_INT32 || pTag->GetDataType() == TYPE_UINT32 ||
            pTag->GetDataType() == TYPE_FLOAT) {
        stWDataUnit.byDataType = 0x06;
        wTemp = 4;
        dwTemp = (pTag->GetRegisterAddress() + pTag->GetOffset()) * 8;
        stWDataUnit.wVailableBitLen = 8 * 4;
        RecoverData(pVarVailableData, pVendorObj->writeBuf, 4);
    }

    // 设置数据长度参数
    stWriteData.byLE = 5 + WRITE_REQ_LEN + wTemp;
    stWriteData.byLER = stWriteData.byLE;
    stWDataUnit.wDataLen = 4 + wTemp;

    // 设置变量对应的plc内存地址
    stWDataUnit.wAddrType = 0x0000;
    if(getCpuMem(pTag->GetRegisterArea()) == CM_V)
        stWDataUnit.wAddrType = 0x0001;

    stWDataUnit.dwAddr = GetBeginAddrAsCpuMem((TCPUMEM)getCpuMem(pTag->GetRegisterArea()));
    if(stWDataUnit.dwAddr== 0)
        return 0;

    stWDataUnit.dwAddr += dwTemp;

    // 设置发送有效数据所在地址
    stWDataUnit.pVailableData = pVendorObj->writeBuf;

    // 写申请命令帧打包完毕，开始发送到PLC
    // 一边计算校验和
    dwTemp = 0;
    memcpy(aDatabuf, (quint8 *)&stWriteData, 9);

    // 序列化写请求包
    Series_WDataUnit(&stWDataUnit);
    memcpy(aDatabuf+9, (quint8 *)&stWDataUnit, 26);
    Series_WDataUnit(&stWDataUnit);

    memcpy(aDatabuf+35, stWDataUnit.pVailableData, wTemp);
    // 发送实际数值信息
    dwTemp += AddCheckSum(aDatabuf+4, 31+wTemp);

    stWriteData.byFCS = (quint8)(dwTemp & 0x000000FF);

    aDatabuf[35+wTemp] = (quint8)(dwTemp & 0x000000FF); // FC
    aDatabuf[36+wTemp] = 0x16;                          // ED

    if(getPort() != Q_NULLPTR)
        getPort()->write(aDatabuf, 37+wTemp, iCommTimeout);

    memset(pVendorObj->readBuf, 0, 512);

    // 等待plc应答
    int resultlen = 0;
    if(getPort() != Q_NULLPTR)
        resultlen = getPort()->read(pVendorObj->readBuf, 1, iCommTimeout);

    if(resultlen != 1)
        return 0;

    // 判断对方接收确认数据正确性
    if(pVendorObj->readBuf[0] == 0xE5 || pVendorObj->readBuf[0] == 0x10) {
        dwTemp = 0;
        do
        {
            dwTemp++;
            if(dwTemp > 30) return 0;

            // 设置功能码 FC
            if(g_ucSimaticFC != 0x6c) {
                if(g_byFC == 0x5C) {
                    stWriteAck.byFC = 0x7C;
                    g_byFC = 0x7C;
                } else {
                    stWriteAck.byFC = 0x5C;
                    g_byFC = 0x5C;
                }
            } else {
                stWriteAck.byFC = 0x6C;
            }
            QThread::msleep(WRITE_DELAY);
            // 发送写确认命令
            stWriteAck.bySA = (quint8)g_wHmiAddress;
            stWriteAck.byDA = (quint8)pTag->GetDeviceAddress();
            wTemp = stWriteAck.bySA + stWriteAck.byDA + stWriteAck.byFC;
            stWriteAck.byFCS = (wTemp & 0x00FF);

            // 序列化写确认包
            if(getPort() != Q_NULLPTR)
                getPort()->write((quint8 *)&stWriteAck, 6, iCommTimeout);

            memset(pVendorObj->readBuf, 0, 512);
            QThread::msleep(WRITE_DELAY);

            if(getPort() != Q_NULLPTR)
                resultlen = getPort()->read(pVendorObj->readBuf, 1, iCommTimeout);

            if(resultlen != 1)
                return 0;

        } while(pVendorObj->readBuf[0] == 0xE5);

        // 等待plc应答申请数据
        // 计算应答数据长度
        wTemp = WRITE_REPLAY_LEN;
        if(getPort() != Q_NULLPTR)
            resultlen = getPort()->read(pVendorObj->readBuf + 1, WRITE_REPLAY_LEN - 1, iCommTimeout);

        if(resultlen != (WRITE_REPLAY_LEN - 1))
            return 0;

        memcpy(&WriteRetData, pVendorObj->readBuf, 9);
        memcpy(&WRetDataUnit, pVendorObj->readBuf + 9, sizeof(TWRetDataUnit));
        // 将写应答数据结构序列化
        Series_WRetDataUnit(&WRetDataUnit);

        if((WriteRetData.byDA != (quint8)g_wHmiAddress) || (WriteRetData.bySA != (quint8)pTag->GetDeviceAddress()))
            return 0;

        // 判别申请数据
        byTemp = *(pVendorObj->readBuf + wTemp - 2);
        _byTemp = *(pVendorObj->readBuf + wTemp - 1);

        if((WriteRetData.bySD4 != 0x68) || (WriteRetData._bySD4 != 0x68)
                || (WriteRetData.byFC != 0x08) || (_byTemp != 0x16))
            return 0;

        // 检查校验和
        dwTemp = AddCheckSum(pVendorObj->readBuf + 4, wTemp - 4 - 2);
        if((dwTemp & 0xFF) != byTemp)
            return 0;

        if((WRetDataUnit.byMsgHead != 0xFF) || (WRetDataUnit.byOpType != 0x05))
            return 0;

        return 1;
    }

    return 0;
}


/**
 * @brief SiemensS7_200Impl::isCanRead
 * @details 判断该变量定义属性是否可以读
 * @param pObj 设备描述对象
 * @param pTag 设备变量
 * @return false-不可读，true-可读
 */
bool SiemensS7_200Impl::isCanRead(void* pObj, IOTag* pTag)
{
    (void)pObj;
    if(getCpuMem(pTag->GetRegisterArea()) == CM_NON)
        return false;

    return true;
}


/**
 * @brief SiemensS7_200Impl::readData
 * @details 从plc设备读一个变量
 * @param pObj 设备描述对象
 * @param pTag 设备变量
 * @return 0-失败,1-成功
 */
int SiemensS7_200Impl::readData(void* pObj, IOTag* pTag)
{
    Vendor* pVendorObj = (Vendor*)(pObj);
    int iCommTimeout = pVendorObj->m_pVendorPrivateObj->m_iCommTimeout;
    // 发送数据结构
    TRDataUnit stRDataUnit = {0x00, 0x00, 0x00, 0x04, 0x00, Q_NULLPTR};
    TRDataMsgUnit stRDataMsgUnit = {0x12, 0x0A, 0x10, 0x00, 0x00, 0x00, 0x00};
    TWriteReadData stReadData = {0x68, 0x00, 0x00, 0x68, 0x00, 0x00, 0x6C, 0x32, 0x01, Q_NULLPTR, 0x00, 0x16};
    TWriteReadAck stReadAck = {0x10, 0x00, 0x00, 0x5C, 0x00, 0x16};

    quint8 *pVailableRecData;
    TWriteReadData ReadRetData;
    TRRetDataUnit RRetDataUnit;
    TRRetDataMsgUnit RRetDataMsgUnit;

    quint16 wTemp = 0;
    quint16 wRDataLen = 0;
    quint8 byTemp = 0;
    quint8 _byTemp = 0;
    qint32 iTemp;
    quint32 dwTemp=0 ,nLen=0;

    memset(pVendorObj->writeBuf, 0, sizeof(pVendorObj->writeBuf)/sizeof(quint8));
    memset(pVendorObj->readBuf, 0, sizeof(pVendorObj->readBuf)/sizeof(quint8));

    stReadData.pDU = (quint8 *)&stRDataUnit;

    // 设置通信目的地址和源地址
    stReadData.byDA = (quint8)pTag->GetDeviceAddress();
    stReadData.bySA = (quint8)g_wHmiAddress;
    // 设置数据长度参数
    stReadData.byLE = 27;
    stReadData.byLER = stReadData.byLE;

    // 设置功能码 FC
    if(g_ucSimaticFC != 0x6c) {
        if(g_byFC == 0x5C) {
            stReadData.byFC = 0x7C;
            g_byFC = 0x7C;
        } else {
            stReadData.byFC = 0x5C;
            g_byFC = 0x5C;
        }
    } else {
        stReadData.byFC = 0x6C;
    }

    g_Count++;
    if(g_Count >= 256) g_Count = 0x01;

    stRDataUnit.dwReserved = g_Count + (g_Count*256);
    stRDataUnit.dwReserved = ReverseDwordByte(stRDataUnit.dwReserved);

    memcpy(pVendorObj->writeBuf, (quint8 *)&stReadData, 9);

    // 填写数据块
    stRDataUnit.wMsgLen = 14;
    stRDataUnit.byMsgGroup = 1;

    Series_RDataUnit(&stRDataUnit);
    memcpy(pVendorObj->writeBuf+9, (quint8 *)&stRDataUnit, 10);

    // 将分组消息组合放在pVendorObj->writeBuf缓冲区中
    wTemp = 0;
    iTemp = sizeof(TRDataMsgUnit);
    nLen = iTemp;
    dwTemp = 0;

    stRDataMsgUnit.wReadDataLen = 1;

    if(getCpuMem(pTag->GetRegisterArea()) == CM_T) {
        dwTemp = pTag->GetRegisterAddress() + pTag->GetOffset();
        stRDataMsgUnit.byDataType = 0x1F;
        wTemp += 5;
    } else if(getCpuMem(pTag->GetRegisterArea()) == CM_C) {
        dwTemp = pTag->GetRegisterAddress() + pTag->GetOffset();
        stRDataMsgUnit.byDataType = 0x1E;
        wTemp += 3;
    } else if(getCpuMem(pTag->GetRegisterArea()) == CM_HC) {
        dwTemp = pTag->GetRegisterAddress() + pTag->GetOffset();
        stRDataMsgUnit.byDataType = 0x20;
        wTemp += 5;
    } else if(pTag->GetDataType() == TYPE_BOOL) {
        dwTemp = pTag->GetRegisterAddress() * 8 + pTag->GetOffset();
        stRDataMsgUnit.byDataType = 0x01;
        wTemp += 1;
    } else if(pTag->GetDataType() == TYPE_INT8 || pTag->GetDataType() == TYPE_UINT8) {
        dwTemp = (pTag->GetRegisterAddress() + pTag->GetOffset()) * 8;
        stRDataMsgUnit.byDataType = 0x02;
        stRDataMsgUnit.wReadDataLen = pTag->GetDataTypeLength();
        wTemp += pTag->GetDataTypeLength();
    } else if(pTag->GetDataType() == TYPE_INT16 || pTag->GetDataType() == TYPE_UINT16) {
        dwTemp = (pTag->GetRegisterAddress() + pTag->GetOffset()) * 8;
        stRDataMsgUnit.byDataType = 0x04;
        wTemp += 2;
    } else if(pTag->GetDataType() == TYPE_INT32 || pTag->GetDataType() == TYPE_UINT32 ||
              pTag->GetDataType() == TYPE_FLOAT) {
        dwTemp = (pTag->GetRegisterAddress() + pTag->GetOffset()) * 8;
        stRDataMsgUnit.byDataType = 0x06;
        wTemp += 4;
    }

    // 设置变量对应的plc内存地址
    stRDataMsgUnit.wAddrType = 0x0000;
    if(getCpuMem(pTag->GetRegisterArea()) == CM_V)
        stRDataMsgUnit.wAddrType = 0x0001;

    stRDataMsgUnit.dwAddr = GetBeginAddrAsCpuMem((TCPUMEM)getCpuMem(pTag->GetRegisterArea()));
    stRDataMsgUnit.dwAddr += dwTemp;

    Series_RDataMsgUnit(&stRDataMsgUnit);
    memcpy(pVendorObj->writeBuf + 19, &stRDataMsgUnit, iTemp);

    // 记录返回有效数据长度
    wRDataLen = wTemp;

    dwTemp = 0;
    dwTemp += AddCheckSum(pVendorObj->writeBuf+4, 15+nLen);

    pVendorObj->writeBuf[19 + nLen] = (quint8)(dwTemp & 0xFF);  // FC
    pVendorObj->writeBuf[20 + nLen] = 0x16;                    // ED

    if(getPort() != Q_NULLPTR)
        getPort()->write(pVendorObj->writeBuf, nLen + 21, iCommTimeout);

    memset(pVendorObj->readBuf, 0, 512);
    int resultlen = 0;
    if(getPort() != Q_NULLPTR)
        resultlen = getPort()->read(pVendorObj->readBuf, 1, iCommTimeout);

    if(resultlen != 1)
        return 0;

    // 判断对方接收确认数据正确性
    if(pVendorObj->readBuf[0] == 0xE5 || pVendorObj->readBuf[0] == 0x10) {
        dwTemp = 0;
        do
        {
            dwTemp++;
            if(dwTemp > 30) return 0;

            // 设置功能码 FC
            if(g_ucSimaticFC != 0x6c) {
                if(g_byFC == 0x5C) {
                    stReadAck.byFC = 0x7C;
                    g_byFC = 0x7C;
                } else {
                    stReadAck.byFC = 0x5C;
                    g_byFC = 0x5C;
                }
            } else {
                stReadAck.byFC = 0x6C;
            }

            QThread::msleep(READ_DELAY);

            // 发送读确认命令
            stReadAck.bySA = (quint8)g_wHmiAddress;
            stReadAck.byDA = (quint8)pTag->GetDeviceAddress();
            wTemp = stReadAck.bySA + stReadAck.byDA + stReadAck.byFC;
            stReadAck.byFCS = (quint8)(wTemp & 0xFF);

            if(getPort() != Q_NULLPTR)
                getPort()->write((quint8 *)&stReadAck, 6, iCommTimeout);

            // 等待plc应答申请数据
            if(getPort() != Q_NULLPTR)
                resultlen = getPort()->read(pVendorObj->readBuf, 1, iCommTimeout);

            if(resultlen != 1) return 0;
        } while(pVendorObj->readBuf[0] == 0xE5);

        // 等待plc应答申请数据
        if(getPort() != Q_NULLPTR)
            resultlen = getPort()->read(pVendorObj->readBuf + 1, 3, iCommTimeout);

        if(resultlen != 3)
            return 0;

        byTemp = *(pVendorObj->readBuf + 1);

        if(getPort() != Q_NULLPTR)
            resultlen = getPort()->read(pVendorObj->readBuf + 4, (size_t)byTemp + 2, iCommTimeout);

        if(resultlen != (byTemp + 2))
            return 0;

        // 计算数据帧长度
        wTemp = (quint16)byTemp + 2 + 4;

        memcpy(&ReadRetData, pVendorObj->readBuf, 9);
        memcpy(&RRetDataUnit, pVendorObj->readBuf + 9, sizeof(TRRetDataUnit));
        pVailableRecData = pVendorObj->readBuf + 9 + sizeof(TRRetDataUnit);
        // 将写读应答数据包序列化
        Series_RRetDataUnit(&RRetDataUnit);

        if(ReadRetData.byDA != (quint8)g_wHmiAddress || ReadRetData.bySA != (quint8)pTag->GetDeviceAddress())
            return 0;

        if((RRetDataUnit.byOpType != 0x04) || (RRetDataUnit.byMsgGroup != 1))
            return 0;

        // 获取帧尾标记和校验和
        byTemp = *(pVendorObj->readBuf + wTemp - 2);
        _byTemp = *(pVendorObj->readBuf + wTemp - 1);

        if(ReadRetData.bySD4 != 0x68 || ReadRetData._bySD4 != 0x68 ||
           ReadRetData.byFC != 0x08 || _byTemp != 0x16)
            return 0;

        // 检查校验和
        dwTemp = AddCheckSum(pVendorObj->readBuf + 4, wTemp - 4 - 2);
        if((dwTemp & 0x00FF) != byTemp)
            return -2;

        if(*pVailableRecData != 0xFF)
            return 0;

        iTemp = sizeof(TRRetDataMsgUnit);
        memcpy(&RRetDataMsgUnit, pVailableRecData, iTemp);
        Series_RRetDataMsgUnit(&RRetDataMsgUnit);
        pVailableRecData += iTemp;

        wRDataLen = 0;
        byTemp = 0x04;

        if(getCpuMem(pTag->GetRegisterArea()) == CM_T) {
            byTemp = 0x09;
            wRDataLen = 5;
            if(pTag->GetDataType() == TYPE_BOOL) {
                if((*pVailableRecData & 0x02) == 0x02) pVendorObj->writeBuf[0] = 1;
                else pVendorObj->writeBuf[0] = 0;
            } else {
                RecoverData(pVailableRecData + 3, pVendorObj->writeBuf, 2);
            }
        } else if(getCpuMem(pTag->GetRegisterArea()) == CM_C) {
            byTemp = 0x09;
            wRDataLen = 3;
            if(pTag->GetDataType() == TYPE_BOOL) {
                if((*pVailableRecData & 0x10) == 0x10) pVendorObj->writeBuf[0] = 1;
                else pVendorObj->writeBuf[0] = 0;
            } else {
                RecoverData(pVailableRecData + 1, pVendorObj->writeBuf, 2);
            }
        } else if(getCpuMem(pTag->GetRegisterArea()) == CM_HC) {
            byTemp = 0x09;
            wRDataLen = 5;
            RecoverData(pVailableRecData + 1, pVendorObj->writeBuf, 4);
        } else if(pTag->GetDataType() == TYPE_BOOL) {
            byTemp = 0x03;
            wRDataLen = 1;
            if(*pVailableRecData) pVendorObj->writeBuf[0] = 1;
            else pVendorObj->writeBuf[0] = 0;
        } else if(pTag->GetDataType() == TYPE_INT8 || pTag->GetDataType() == TYPE_UINT8) {
            wRDataLen = pTag->GetDataTypeLength() * 8;
            memcpy(pVendorObj->writeBuf, pVailableRecData, pTag->GetDataTypeLength());
        } else if(pTag->GetDataType() == TYPE_INT16 || pTag->GetDataType() == TYPE_UINT16) {
            wRDataLen = 2 * 8;
            RecoverData(pVailableRecData, pVendorObj->writeBuf, 2);
        } else if(pTag->GetDataType() == TYPE_INT32 || pTag->GetDataType() == TYPE_UINT32 ||
                  pTag->GetDataType() == TYPE_FLOAT) {
            wRDataLen = 4 * 8;
            RecoverData(pVailableRecData, pVendorObj->writeBuf, 4);
        }

        // 拷贝数据到变量数据存放区
        wTemp = (quint16)pTag->GetDataTypeLength();
        memcpy(pTag->pReadBuf, pVendorObj->writeBuf, wTemp);

        // 消息组数据指针移动到下一组消息头
        if((wRDataLen & 0x07) == 0) wRDataLen = wRDataLen / 8;
        if((wRDataLen & 0x01) == 0x01) wRDataLen += 1;

        pVailableRecData += wRDataLen;

        return 1;
    }

    return 0;
}


///
/// \brief SiemensS7_200Impl::convertIOTagBytesToNativeBytes
/// \details 变量字节序转换为当前主机字节序
/// \param pObj 设备描述对象
/// \param pTag 变量描述对象
/// \return true-成功, false-失败
///
bool SiemensS7_200Impl::convertIOTagBytesToNativeBytes(void* pObj, IOTag* pTag)
{
    (void)pObj;
    (void)pTag;
    return true;
}


