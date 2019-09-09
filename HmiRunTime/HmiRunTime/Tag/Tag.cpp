
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QtDebug>

#include "Tag.h"


Tag::Tag(QObject *parent) : QObject(parent)
{

}


void IoDataTag::LoadData(const QJsonObject &json)
{
    mId = json["sID"].toString();
    mName = json["sName"].toString();
    mDescription = json["sDescription"].toString();
    mUnit = json["sUnit"].toString();
    mProjectConverter = json["sProjectConverter"].toString();
    mComments = json["sComments"].toString();

    //-------------IO连接-----------------//
    QString sIOConnect = json["sIOConnect"].toString();
    QStringList listIOConnect;
    listIOConnect = sIOConnect.split(QRegExp(","));

    if(listIOConnect.size() == 15)
    {
        if(listIOConnect.at(0) == "1")
            mIsIOConnect = true;
        else
            mIsIOConnect = false;

        if(listIOConnect.at(1) == "1")
            mIsSelfVariable = true;
        else
            mIsSelfVariable = false;

        if(listIOConnect.at(2) == "1")
            mIsGlobal = true;
        else
            mIsGlobal = false;

        mDeviceName = listIOConnect.at(3);

        if(listIOConnect.at(4) == "只读")
        {
            mPermissionType = READ;
        }
        else if(listIOConnect.at(4) == "只写")
        {
            mPermissionType = WRIE;
        }
        else if(listIOConnect.at(4) == "读写")
        {
            mPermissionType = READ_WRIE;
        }
        else
        {
            mPermissionType = READ;
        }

        QString tmp = listIOConnect.at(5);
        mDeviceAddress = tmp.toInt();

        tmp = listIOConnect.at(6);
        mMaxValue = tmp.toDouble();

        mRegisterArea = listIOConnect.at(7);

        tmp = listIOConnect.at(8);
        mMinValue = tmp.toDouble();

        tmp = listIOConnect.at(9);
        mRegisterAddress = tmp.toInt();

        tmp = listIOConnect.at(10);
        mInitializeValue = tmp.toDouble();

        tmp = listIOConnect.at(11);
        if(tmp == "Bit1开关量")
        {
            mType = mDataType = TYPE_BOOL;
            mLength = 1;
        }
        else if(tmp == "Char8位有符号数")
        {
            mType = mDataType = TYPE_INT8;
            mLength = 1;
        }

        else if(tmp == "Byte8位无符号数")
        {
            mType = mDataType = TYPE_UINT8;
            mLength = 1;
        }
        else if(tmp == "Short16位有符号数")
        {
            mType = mDataType = TYPE_INT16;
            mLength = 2;
        }
        else if(tmp == "Word16位无符号数")
        {
            mType = mDataType = TYPE_UINT16;
            mLength = 2;
        }
        else if(tmp == "ASCII2个字符")
        {
            mType = mDataType = TYPE_ASCII2CHAR;
            mLength = 2;
        }
        else if(tmp == "Long32位有符号数")
        {
            mType = mDataType = TYPE_INT32;
            mLength = 4;
        }
        else if(tmp == "Dword32位无符号数")
        {
            mType = mDataType = TYPE_UINT32;
            mLength = 4;
        }
        else if(tmp == "Float单精度浮点数")
        {
            mType = mDataType = TYPE_FLOAT;
            mLength = 4;
        }
        else if(tmp == "String字符串")
        {
            mType = mDataType = TYPE_STRING;
            mLength = 256;
        }
        else if(tmp == "Double双精度浮点数")
        {
            mType = mDataType = TYPE_DOUBLE;
            mLength = 8;
        }
        else if(tmp == "BCD")
        {
            mType = mDataType = TYPE_BCD;
            mLength = 8;
        }
        else if(tmp == "LongLong64位有符号数")
        {
            mType = mDataType = TYPE_INT64;
            mLength = 8;
        }
        else if(tmp == "DwordDword64位无符号数")
        {
            mType = mDataType = TYPE_UINT64;
            mLength = 8;
        }

        tmp = listIOConnect.at(12);
        mScale = tmp.toDouble();

        tmp = listIOConnect.at(13);
        mOffset = tmp.toInt();

        tmp = listIOConnect.at(14);
        mInFrameAddress = tmp.toInt();
    }

    //-------------报警-----------------//
    QString sDataType = json["sDataType"].toString();
    QString sAlarm = json["sAlarm"].toString();
    QStringList listAlarm;
    listAlarm = sAlarm.split(QRegExp(","));
    if(sDataType == "模拟量" && listAlarm.size() == 17)
    {
        mAlarm.mTypeWhat = TYPE_ALARM_AI;
        mAlarm.mArgsMap["IsAlarm"] = listAlarm.at(0);

        mAlarm.mArgsMap["Mode"] = listAlarm.at(1);
        mAlarm.mArgsMap["HShow"] = listAlarm.at(2);
        mAlarm.mArgsMap["HHShow"] = listAlarm.at(3);
        mAlarm.mArgsMap["LShow"] = listAlarm.at(4);
        mAlarm.mArgsMap["LLShow"] = listAlarm.at(5);
        mAlarm.mArgsMap["POffsetShow"] = listAlarm.at(6);
        mAlarm.mArgsMap["NOffsetShow"] = listAlarm.at(7);
        mAlarm.mArgsMap["FixValue"] = listAlarm.at(8);

        mAlarm.mArgsMap["Attribute"] = listAlarm.at(9);
        mAlarm.mArgsMap["HValue"] = listAlarm.at(10);
        mAlarm.mArgsMap["HHValue"] = listAlarm.at(11);
        mAlarm.mArgsMap["LValue"] = listAlarm.at(12);
        mAlarm.mArgsMap["LLValue"] = listAlarm.at(13);
        mAlarm.mArgsMap["POffsetValue"] = listAlarm.at(14);
        mAlarm.mArgsMap["NOffsetValue"] = listAlarm.at(15);
        mAlarm.mArgsMap["Level"] = listAlarm.at(16);
    }
    if(sDataType == "数字量" && listAlarm.size() == 9)
    {
        mAlarm.mTypeWhat = TYPE_ALARM_DI;
        mAlarm.mArgsMap["IsAlarm"] = listAlarm.at(0);
        mAlarm.mArgsMap["Mode"] = listAlarm.at(1);
        mAlarm.mArgsMap["Description"] = listAlarm.at(2);
        mAlarm.mArgsMap["FalseShow"] = listAlarm.at(3);
        mAlarm.mArgsMap["TrueShow"] = listAlarm.at(4);
        mAlarm.mArgsMap["NormalValue"] = listAlarm.at(5);
        mAlarm.mArgsMap["TrueValue"] = listAlarm.at(6);
        mAlarm.mArgsMap["Level"] = listAlarm.at(7);
        mAlarm.mArgsMap["Attribute"] = listAlarm.at(8);
    }

    //-------------存盘-----------------//
    QString sArchiveFile = json["sArchiveFile"].toString();
    QStringList listArchiveFile;
    listArchiveFile = sArchiveFile.split(QRegExp(","));
    if(listArchiveFile.size() == 7)
    {
        if(listArchiveFile.at(0) == "1")
            mIsSaveDisk = true;
        else
            mIsSaveDisk = false;

        if(listArchiveFile.at(1) == "1")
            mIsGroupSaveDisk = true;
        else
            mIsGroupSaveDisk = false;

        if(listArchiveFile.at(2) == "周期存盘")
            mArchiveFileMode = MODE_PERIOD;
        else if(listArchiveFile.at(2) == "变位存盘")
            mArchiveFileMode = MODE_BIT_VALUE_CHANGED;
        else if(listArchiveFile.at(2) == "整点存盘")
            mArchiveFileMode = MODE_EQUAL_VALUE;
        else
            mArchiveFileMode = MODE_PERIOD;

        QString tmp1 = listArchiveFile.at(2);
        mArchiveFilePeriod = tmp1.toInt();

        if(listArchiveFile.at(4) == "1")
            mIsDataCkeckSaveDisk = true;
        else
            mIsDataCkeckSaveDisk = false;

        tmp1 = listArchiveFile.at(5);
        if(tmp1 == "") mHighLimit = 0;
        else mHighLimit = tmp1.toDouble();

        tmp1 = listArchiveFile.at(6);
        if(tmp1 == "") mLowLimit = 0;
        else mLowLimit = tmp1.toDouble();

    }

    m_Function.LoadFuncObjects(mProjectConverter);
}

void TmpDataTag::LoadData(const QJsonObject &json)
{
    mId = json["sID"].toString();
    mType = TYPE_STRING;
    mName = json["sName"].toString();
    mDescription = json["sDescription"].toString();
    mUnit = json["sUnit"].toString();
    mProjectConverter = json["sProjectConverter"].toString();
    mComments = json["sComments"].toString();

    mActionScope = json["sActionScope"].toString();  //作用域

    //-------------数据属性-----------------//
    QString sDataAttribute = json["sDataAttribute"].toString();
    QStringList listDataAttribute;
    listDataAttribute = sDataAttribute.split(QRegExp(","));
    if(listDataAttribute.size() == 5)
    {
        if(listDataAttribute.at(0) == "只读")
        {
            mPermissionType = READ;
        }
        else if(listDataAttribute.at(0) == "只写")
        {
            mPermissionType = WRIE;
        }
        else if(listDataAttribute.at(0) == "读写")
        {
            mPermissionType = READ_WRIE;
        }
        else
        {
            mPermissionType = READ;
        }

        QString tmp = listDataAttribute.at(1);
        if(tmp == "") mMaxValue = 0;
        else mMaxValue = tmp.toDouble();

        tmp = listDataAttribute.at(2);
        if(tmp == "") mMinValue = 0;
        else mMinValue = tmp.toDouble();

        tmp = listDataAttribute.at(3);
        if(tmp == "") mInitializeValue = 0;
        else mInitializeValue = tmp.toDouble();

        tmp = listDataAttribute.at(4);
        if(tmp == "") mScale = 0;
        else mScale = tmp.toDouble();
    }

    //-------------报警-----------------//
    QString sDataType = json["sDataType"].toString();
    QString sAlarm = json["sAlarm"].toString();
    QStringList listAlarm;
    listAlarm = sAlarm.split(QRegExp(","));
    if(sDataType == "模拟量" && listAlarm.size() == 17)
    {
        mAlarm.mTypeWhat = TYPE_ALARM_AI;
        mAlarm.mArgsMap["IsAlarm"] = listAlarm.at(0);

        mAlarm.mArgsMap["Mode"] = listAlarm.at(1);
        mAlarm.mArgsMap["HShow"] = listAlarm.at(2);
        mAlarm.mArgsMap["HHShow"] = listAlarm.at(3);
        mAlarm.mArgsMap["LShow"] = listAlarm.at(4);
        mAlarm.mArgsMap["LLShow"] = listAlarm.at(5);
        mAlarm.mArgsMap["POffsetShow"] = listAlarm.at(6);
        mAlarm.mArgsMap["NOffsetShow"] = listAlarm.at(7);
        mAlarm.mArgsMap["FixValue"] = listAlarm.at(8);

        mAlarm.mArgsMap["Attribute"] = listAlarm.at(9);
        mAlarm.mArgsMap["HValue"] = listAlarm.at(10);
        mAlarm.mArgsMap["HHValue"] = listAlarm.at(11);
        mAlarm.mArgsMap["LValue"] = listAlarm.at(12);
        mAlarm.mArgsMap["LLValue"] = listAlarm.at(13);
        mAlarm.mArgsMap["POffsetValue"] = listAlarm.at(14);
        mAlarm.mArgsMap["NOffsetValue"] = listAlarm.at(15);
        mAlarm.mArgsMap["Level"] = listAlarm.at(16);
    }
    if(sDataType == "数字量" && listAlarm.size() == 9)
    {
        mAlarm.mTypeWhat = TYPE_ALARM_DI;
        mAlarm.mArgsMap["IsAlarm"] = listAlarm.at(0);
        mAlarm.mArgsMap["Mode"] = listAlarm.at(1);
        mAlarm.mArgsMap["Description"] = listAlarm.at(2);
        mAlarm.mArgsMap["FalseShow"] = listAlarm.at(3);
        mAlarm.mArgsMap["TrueShow"] = listAlarm.at(4);
        mAlarm.mArgsMap["NormalValue"] = listAlarm.at(5);
        mAlarm.mArgsMap["TrueValue"] = listAlarm.at(6);
        mAlarm.mArgsMap["Level"] = listAlarm.at(7);
        mAlarm.mArgsMap["Attribute"] = listAlarm.at(8);
    }

    //-------------存盘-----------------//
    QString sArchiveFile = json["sArchiveFile"].toString();
    QStringList listArchiveFile;
    listArchiveFile = sArchiveFile.split(QRegExp(","));
    if(listArchiveFile.size() == 7)
    {
        if(listArchiveFile.at(0) == "1")
            mIsSaveDisk = true;
        else
            mIsSaveDisk = false;

        if(listArchiveFile.at(1) == "1")
            mIsGroupSaveDisk = true;
        else
            mIsGroupSaveDisk = false;

        if(listArchiveFile.at(2) == "周期存盘")
            mArchiveFileMode = MODE_PERIOD;
        else if(listArchiveFile.at(2) == "变位存盘")
            mArchiveFileMode = MODE_BIT_VALUE_CHANGED;
        else if(listArchiveFile.at(2) == "整点存盘")
            mArchiveFileMode = MODE_EQUAL_VALUE;
        else
            mArchiveFileMode = MODE_PERIOD;

        QString tmp1 = listArchiveFile.at(2);
        mArchiveFilePeriod = tmp1.toInt();

        if(listArchiveFile.at(4) == "1")
            mIsDataCkeckSaveDisk = true;
        else
            mIsDataCkeckSaveDisk = false;

        tmp1 = listArchiveFile.at(5);
        if(tmp1 == "") mHighLimit = 0;
        else mHighLimit = tmp1.toDouble();

        tmp1 = listArchiveFile.at(6);
        if(tmp1 == "") mLowLimit = 0;
        else mLowLimit = tmp1.toDouble();

    }

    m_Function.LoadFuncObjects(mProjectConverter);
}

void SysDataTag::LoadData(const QJsonObject &json)
{
    mId = json["sID"].toString();
    mType = TYPE_STRING;
    mName = json["sName"].toString();
    mDescription = json["sDescription"].toString();
    mUnit = json["sUnit"].toString();
    mProjectConverter = json["sProjectConverter"].toString();
    mComments = json["sComments"].toString();

    m_Function.LoadFuncObjects(mProjectConverter);
}




