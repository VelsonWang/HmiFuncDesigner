#ifndef REALTIMEDB_H
#define REALTIMEDB_H

#include <QObject>
#include <QHash>
#include <QVariant>
#include <QTimer>
#include <QMutex>
#include "public.h"
#include "sharedlibglobal.h"

class SHAREDLIB_EXPORT RunTimeTag : public QObject
{
    Q_OBJECT
public:
    explicit RunTimeTag(QObject *parent = NULL);
    RunTimeTag(const RunTimeTag &obj);
    RunTimeTag &operator=(const RunTimeTag &obj);
    void copyObject(const RunTimeTag &obj);
    void copyFromTag(RunTimeTag obj);
    ~RunTimeTag();
    bool fromString(const QString &val, bool syncToVendor = false);
    QString toString();
    QString minString();
    QString maxString();
    bool toBool();

    inline void updateVendorData(const QByteArray &dat)
    {
        this->dataFromVendor.replace(0, dat.length(), dat);
    }

public:
    ////////////////////<基本信息>//////////////////////////

    int id = 0; // 变量ID
    QString name = ""; // 变量名称
    QString unit = ""; // 变量单位
    QString addrType = ""; // 地址类型
    int addrOffset = 0; // 地址偏移
    QString aAddrType2 = ""; // 地址类型2
    int addrOffset2 = 0; // 地址偏移2
    TTagDataType dataType = TYPE_VARIANT; // 数据类型
    TWriteAble writeable = CAN_WRITE; // 读写类型  0-只读, 1-可写
    QString remark = ""; // 变量描述
    QString ownGroup = ""; // 变量所属组
    QString devType = ""; // 变量设备类型-设备协议名称, 内存变量-MEMORY, 系统变量-SYSTEM

    int bufLength = 0; // 变量数据缓存区大小
    QByteArray dataFromVendor; // 变量读取值缓存区 VIP: 注意线程安全问题
    QByteArray dataToVendor; // 变量写入值缓存区 VIP: 注意线程安全问题
};

// 处理编辑端生成的id字符
// id:  "设备变量_4x1[7]"
inline int tagId(const QString &id)
{
    QString tmp = id;
    if(tmp.endsWith("]")) {
        tmp.chop(1);
    }
    int pos = tmp.indexOf("[");
    if(pos > 0) {
        return tmp.remove(0, pos + 1).toInt();
    }

    return id.toInt();
}


class SHAREDLIB_EXPORT RealTimeDB : public QObject
{
    Q_OBJECT
public:
    static RealTimeDB *instance();

    void setTagData(quint64 id, QByteArray val, bool syncToVendor = false);
    void setTagData(quint64 id, const QString &dat, bool syncToVendor = false);
    QByteArray tagData(quint64 id);

    RunTimeTag* tag(quint64 id);

    quint64 tagId(const QString &name);

    void debug();

private:
    explicit RealTimeDB(QObject *parent = nullptr);
    ~RealTimeDB();

signals:

public slots:

public:
    QHash<quint64, RunTimeTag * > rtdb;

private:
    static QMutex m_mutex;

};


#endif // REALTIMEDB_H
