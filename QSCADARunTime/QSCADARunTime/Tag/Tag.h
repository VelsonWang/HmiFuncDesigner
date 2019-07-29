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
    explicit Tag(QObject *parent = nullptr);

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

class AlarmConfig
{
public:
    TAlarmType mTypeWhat;
    QMap<QString, QString> mArgsMap;
};



class IoDataTag : public Tag
{
public:
    //-------------IO连接-----------------//
    bool mIsIOConnect; // 是否IO连接
    bool mIsSelfVariable; // 是否内部变量
    bool mIsGlobal; // 全局属性
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
    qint32 mInFrameAddress; // 帧内地址

    //-------------报警-----------------//
    AlarmConfig mAlarm;  //报警

    //-------------存盘-----------------//
    bool mIsSaveDisk; // 是否存盘
    bool mIsGroupSaveDisk; // 是否按组存盘
    TArchiveFileMode mArchiveFileMode; // 存盘方式
    qint32 mArchiveFilePeriod; // 存盘周期
    bool mIsDataCkeckSaveDisk; // 是否判断存盘数据范围
    double mHighLimit; // 存盘数据范围上限
    double mLowLimit; // 存盘数据范围下限

public:
    void LoadData(const QJsonObject &json);

};


/////////////////////////////////////////////////

class TmpDataTag : public Tag
{
public:
    QString mActionScope;  //作用域

   //-------------数据属性-----------------//
    TPermissionType mPermissionType; // 读写类型
    double mMaxValue; // 最大值
    double mMinValue; // 最小值
    double mInitializeValue; // 初始值
    double mScale; // 变比

    //-------------报警-----------------//
    AlarmConfig mAlarm;  //报警

    //-------------存盘-----------------//
    bool mIsSaveDisk; // 是否存盘
    bool mIsGroupSaveDisk; // 是否按组存盘
    TArchiveFileMode mArchiveFileMode; // 存盘方式
    qint32 mArchiveFilePeriod; // 存盘周期
    bool mIsDataCkeckSaveDisk; // 是否判断存盘数据范围
    double mHighLimit; // 存盘数据范围上限
    double mLowLimit; // 存盘数据范围下限

public:
    void LoadData(const QJsonObject &json);
};


//////////////////////////////////////////////////

class SysDataTag : public Tag
{

public:
    void LoadData(const QJsonObject &json);

};



#endif // TAG_H
