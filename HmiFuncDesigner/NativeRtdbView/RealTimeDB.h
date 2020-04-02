#ifndef REALTIMEDB_H
#define REALTIMEDB_H

#include <QObject>
#include <QString>
#include "ShareMemory.h"

//------------------------------------------------------------------------------

//////////////////////////////////////////
/// \brief The eTagType enum
///
enum eTagDataType
{
    TYPE_VARIANT = 0,
    TYPE_BOOL,
    TYPE_INT8,
    TYPE_UINT8,
    TYPE_INT16,
    TYPE_UINT16,
    TYPE_INT32,
    TYPE_UINT32,
    TYPE_INT64,
    TYPE_UINT64,
    TYPE_FLOAT,
    TYPE_DOUBLE,
    TYPE_ASCII2CHAR,
    TYPE_STRING,
    TYPE_BCD,
    TYPE_BYTES
};
typedef enum eTagDataType TTagDataType;

//------------------------------------------------------------------------------

typedef union any
{
#define MAX_LEN    (64)
    quint8 t_bool;
    qint8 t_int8;
    quint8 t_uint8;
    qint16 t_int16;
    quint16 t_uint16;
    qint32 t_int32;
    quint32 t_uint32;
    qint64 t_int64;
    quint64 t_uint64;
    float t_float32;
    double t_float64;
    quint16 t_ascii2char;
    char t_string[MAX_LEN]; // TODO 字符串长度待定
    quint32 t_bcd;
    quint8 t_bytes[MAX_LEN];

    bool operator==(const any &rhs) {
        for (int i=0; i<MAX_LEN; i++)
            if(t_bytes[i] != rhs.t_bytes[i]) return false;
        return true;
    }

    bool operator!=(const any &rhs) {
        for (int i=0; i<MAX_LEN; i++)
            if(t_bytes[i] != rhs.t_bytes[i]) return true;
        return false;
    }

#undef MAX_LEN
} __attribute__ ((packed)) TAny;

typedef struct tagDBTagObject
{
    char szID[32]; // 变量ID
    quint8 iType; // 变量类型 字节 字 双字等
    quint8 iPermission; // 读写权限
    qint32 mLength; // 长度
    quint8 byWriteToVendor; // 标记需要写入PLC，仪器等设备
    quint8 byUpdateFromVendor; // 标记已从PLC，仪器等设备采集更新
    TAny rData; // 变量数据(read from device)
    TAny wData; // 变量数据(write to device)
} __attribute__ ((packed)) TDBTagObject, *PDBTagObject;

//------------------------------------------------------------------------------


class RealTimeDB : public QObject
{
    Q_OBJECT
public:
    static RealTimeDB *instance() {
        static RealTimeDB rtdb;
        return &rtdb;
    }

private:
    explicit RealTimeDB(QObject *parent = nullptr);
    ~RealTimeDB();

signals:

public slots:

public:
    QString GetDataString(const QString &id);
    void SetDataString(const QString &id, const QString &dat);

    ///
    /// \brief RealTimeDB::getEmptyDBTagObject
    /// \details 从共享内查找指定ID的DBTagObject对象
    /// \return DBTagObject对象
    ///
    PDBTagObject getDBTagObject(const QString &szID);

public:
    bool m_memStatus = true;

private:
    Sharememory m_rtdbSharememory;
    PDBTagObject m_pDBTagObjectBaseAddr = Q_NULLPTR;
};

#endif // REALTIMEDB_H
