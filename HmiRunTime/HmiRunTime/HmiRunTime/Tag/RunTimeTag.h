#ifndef RUNTIMETAG_H
#define RUNTIMETAG_H

#include <QObject>
#include <QMap>
#include "public.h"
#include "Function.h"

class RunTimeTag : public QObject
{
    Q_OBJECT
public:
    explicit RunTimeTag(QObject *parent = Q_NULLPTR);
    RunTimeTag(const RunTimeTag &obj);
    RunTimeTag &operator=(const RunTimeTag &obj);
    void copyObject(const RunTimeTag &obj);
    void copyFromTag(RunTimeTag obj);
    ~RunTimeTag();

public:
    ////////////////////<基本信息>//////////////////////////

    int id = 0; // 变量ID
    QString m_szName = ""; // 变量名称
    QString m_szUnit = ""; // 变量单位
    QString m_szAddrType = ""; // 地址类型
    QString m_szAddrOffset = "0"; // 地址偏移
    QString m_szAddrType2 = ""; // 地址类型2
    QString m_szAddrOffset2 = "0"; // 地址偏移2
    int dataType = 0; // 数据类型
    int m_iWriteable = 1; // 读写类型  0-只读, 1-可写
    QString m_szRemark = ""; // 变量描述
    QString m_szOwnGroup = ""; // 变量所属组
    QString m_szDevType = ""; // 变量设备类型-设备协议名称, 内存变量-MEMORY, 系统变量-SYSTEM

public:
    Function m_Function; // 系统功能函数
};


#ifndef TAG_H
#define TAG_H

#include "global.h"
#include "list.h"
#include "map.h"
#include "moduletampersetting.h"

#define TAG_BUFFER_SIZE    (8)
#define TAG_PACKAGEREAD_START    (60000)

//-----------------------------------------------------------------------

// 变量数据类型
enum _TDataType {
    TYPE_NONE = 0x00,
    TYPE_BOOL = 0x01,
    TYPE_INT8 = 0x02,
    TYPE_UINT8 = 0x03,
    TYPE_INT16 = 0x04,
    TYPE_UINT16 = 0x05,
    TYPE_INT32 = 0x06,
    TYPE_UINT32 = 0x07,
    TYPE_FLOAT32 = 0x08,
    TYPE_DOUBLE = 0x09,
    TYPE_BCD8 = 0x0A,
    TYPE_BCD16 = 0x0B,
    TYPE_BCD32 = 0x0C,

    TYPE_BYTES = 0x64,

    TYPE_RESERVED = 255 // 预留
};
typedef enum _TDataType TDataType;


//-----------------------------------------------------------------------

typedef struct _TTag TTag;
typedef struct _TTag *PTag;

// 读取变量缓冲区更新
typedef void (* PFValueBufferUpdated)(PTag pTag);
// 检查是否需要触发告警
typedef void (* PFCheckWarnings)(PTag pTag);

struct _TTag {
    PIPubObj pPubObj;
    PFValueBufferUpdated pfValueBufferUpdated; // 读取变量缓冲区更新
    PFCheckWarnings pfCheckWarnings; // 检查是否需要触发告警

    ////////////////////<基本信息>//////////////////////////

    INT32 iID; //变量ID
    INT8S szName[16]; // 变量名称
    INT8S szUnit[16]; // 变量单位

    INT8S szAddrType[16]; // 地址类型
    INT32 iAddrOffset; // 地址偏移
    INT32 iAddrByteOffset; // 地址字节偏移
    INT8U iAddrBitOffset; // 地址位偏移

    INT8S szAddrType2[16]; // 地址类型2
    INT32 iAddrOffset2; // 地址偏移2
    INT32 iAddrByteOffset2; // 地址字节偏移
    INT8U iAddrBitOffset2; // 地址位偏移

    TDataType iDataType; // 数据类型
    INT32 iWriteable; // 读写类型	0-只读, 1-可写
    INT8S szRemark[64]; // 变量描述
    INT8S szOwnGroup[16]; // 变量所属组

    ////////////////////<线性变换>//////////////////////////

    BOOLEAN bLineScale; // 启用线性换算
    INT8S szInputMin[16]; // 输入最小值
    INT8S szInputMax[16]; // 输入最大值
    INT8S szOutputMin[16]; // 输出最小值
    INT8S szOutputMax[16]; // 输出最大值
    INT8S iOutputDataType; // 输出数据类型

    ////////////////////<报警配置>//////////////////////////

    struct list_head listConfigWarnings;

    ////////////////////<存储配置>//////////////////////////

    INT32 iSaveType; // 存储方式 0-不存储; 1-周期存储; 2-变化存储; 3-条件存储
    INT8S szSaveValue[64]; // 数值或表达式

    ////////////////////<安全配置>//////////////////////////

    BOOLEAN bEnSafeLevel; // TRUE-启用安全配置; FALSE-关闭安全配置
    INT32 iSafeLevel; // 安全类别 0,1-预留; 2-专家类; 3-维护类; 4-普通类

    //-----------------------------------------------------

    INT8U readBuf[TAG_BUFFER_SIZE]; // 变量读取值缓存区
    INT8U readBufOld[TAG_BUFFER_SIZE]; // 变量前一次读取值缓存区
    INT8U writeBuf[TAG_BUFFER_SIZE]; // 变量写入值缓存区
    BOOLEAN bWriteToDevice; // 变量值需要写入仪器设备

    INT32 iPackageReadID; // 组包读变量ID
    INT32 iAddrOffsetInPackage; // 在组包读变量内地址偏移

    BOOLEAN bValueChanged; // TRUE-变量值已经改变; FALSE-变量值未改变
    BOOLEAN bWarnings; // TRUE-变量值已经改变; FALSE-变量值未改变
    BOOLEAN bOldWarnings; // TRUE-变量值已经改变; FALSE-变量值未改变

    struct list_head list;
    struct list_head listInPackage;
};



// 创建变量描述对象
extern PVOID Tag_CreateObject(void);
// 获取变量数据长度
extern INT32 Tag_GetDataTypeLength(PTag pTag);
// 获取指定数据长度
extern INT32 getDataTypeLength(TDataType iDataType);
// 获取变量数据缓存长度
extern INT32 Tag_GetDataTypeBufferLength(PTag pTag);
// 获取变量线性变换输出数据类型长度
extern INT32 Tag_GetScaleOutputDataTypeLength(PTag pTag);

//------------------------------------------------------

struct _TPackageReadTag {
    TTag tag;

    BOOLEAN bPackageReadOk; // 组包读变量通信读取成功
    BOOLEAN bPackageInUsed; // 启用组包读变量
    INT8U pPackageReadBuf[512]; // 组包读取值缓存区
    INT32U iPackageBufSize; // 组包读取值缓存区大小
    INT32U iPackageSize; // 组包读取值大小
    BOOLEAN bLittleEndian; // 组包读变量数据是否小端数据
};

typedef struct _TPackageReadTag TPackageReadTag;
typedef struct _TPackageReadTag *PPackageReadTag;

// 创建变量描述对象
extern PVOID PackageReadTag_CreateObject(void);

//------------------------------------------------------

struct _TPackageRead {
    PIPubObj pPubObj;
    INT8S szAddrType[16]; // 地址类别
    INT32U iAddrOffsetMin; // 地址偏移量最小值
    INT32U iAddrOffsetMax; // 地址偏移量最大值
    struct list_head listPackageTags; // 组包读变量列表
    struct list_head listTags; // 读变量列表
    struct list_head list;
};

typedef struct _TPackageRead TPackageReadObj;
typedef struct _TPackageRead *PPackageReadObj;

// 组包读取描述对象
extern PVOID PackageRead_CreateObject(void);

//-------------------------------------------------------



// 查找组包读变量列表是否存在地址类型
extern BOOLEAN isAddrTypeExistInPackageReadObj(struct list_head *listFind, PINT8S pszAddrType);
// 查找指定地址类型的组包读取描述对象
extern PPackageReadObj getPackageReadObjByAddrType(struct list_head *listFind, PINT8S pszAddrType);
// 设备描述对象组包读变量列表增加组包读变量列表对象
extern void packageReadAddAddrType(PVOID pObj);
// 字符串转指定类型值
extern void Tag_StringToValue(TDataType iDataType, PINT8U pBuf, PINT8S pData);
// 设置变量写入值(数值字符数据)
extern void Tag_SetWriteBuffer(PTag pTag, TDataType iDataType, PINT8S pData);
// 设置变量写入值(指定数据类型的字节数据)
extern void Tag_SetDataToWriteBuffer(PTag pTag, TDataType iDataType, PINT8U pData);
// 指定类型值转字符串
extern PINT8S Tag_ValueToString(TDataType iDataType, PINT8U pBuf);
// 获取变量值字符串
extern PINT8S Tag_GetValueString(PTag pTag);
// 获取上一采集周期变量值字符串
extern PINT8S Tag_GetOldValueString(PTag pTag);


#endif // TAG_H



#endif // RUNTIMETAG_H
