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
    QString name = ""; // 变量名称
    QString unit = ""; // 变量单位
    QString addrType = ""; // 地址类型
    QString addrOffset = "0"; // 地址偏移
    QString aAddrType2 = ""; // 地址类型2
    QString addrOffset2 = "0"; // 地址偏移2
    TTagDataType dataType = TYPE_VARIANT; // 数据类型
    int writeable = 1; // 读写类型  0-只读, 1-可写
    QString remark = ""; // 变量描述
    QString ownGroup = ""; // 变量所属组
    QString devType = ""; // 变量设备类型-设备协议名称, 内存变量-MEMORY, 系统变量-SYSTEM

public:
    Function m_Function; // 系统功能函数
};

#endif // RUNTIMETAG_H
