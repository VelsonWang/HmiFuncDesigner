#ifndef TAG_H
#define TAG_H

#include <QObject>
#include <QMap>
#include "public.h"
#include "Function.h"

class Tag : public QObject
{
    Q_OBJECT
public:
    explicit Tag(QObject *parent = Q_NULLPTR);

public:
    qint32 mLength; // 长度
    QString mId; // 变量ID
    TTagDataType mType;  // 变量数据类型
    QString mName;  //名称
    QString mDescription;  //描述
    QString mUnit;  //单位
    QString mProjectConverter;  //工程转换
    QString mComments;  //备注

public:
    Function m_Function; // 系统功能函数
};

////////////////////////////////////////////

class IoDataTag : public Tag
{
public:
    //-------------IO连接-----------------//
    QString mDeviceName; // 设备名称
    TPermissionType mPermissionType; // 读写类型
    qint32 mDeviceAddress; // 设备地址
    QString mRegisterArea; // 寄存器区
    qint32 mRegisterAddress; // 寄存器地址
    TTagDataType mDataType; //数据类型
    qint32 mOffset; // 偏移地址
    double mMaxValue; // 最大值
    double mMinValue; // 最小值
    double mInitializeValue; // 初始值
    double mScale; // 变比
};


/////////////////////////////////////////////////

class TmpDataTag : public Tag
{
public:
    double mMaxValue; // 最大值
    double mMinValue; // 最小值
    double mInitializeValue; // 初始值     
};


//////////////////////////////////////////////////

class SysDataTag : public Tag
{
};



#endif // TAG_H
