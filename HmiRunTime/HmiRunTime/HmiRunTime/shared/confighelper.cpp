#include "confighelper.h"
#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <QSettings>
#include <QString>
#include <stdio.h>

/**
 * @brief ConfigHelper::setCfgStr 写字符配置值
 * @param sFileName ini文件名称
 * @param sSecName 组名称
 * @param sKey 键名称
 * @param sValue 设置值
 */
void ConfigHelper::setCfgStr(QString sFileName,
                             QString sSecName,
                             QString sKey,
                             QString sValue)
{
    QSettings settings(sFileName, QSettings::IniFormat);
    settings.beginGroup(sSecName);
    settings.setValue(sKey, sValue);
    settings.endGroup();
}

/**
 * @brief ConfigHelper::getCfgStr 读取配置字符串
 * @param sFileName ini文件名称
 * @param sSecName 组名称
 * @param sKey 键名称
 * @param sDefault 缺省值
 * @return 键值对应的字符串
 */
QString ConfigHelper::getCfgStr(QString sFileName,
                                QString sSecName,
                                QString sKey,
                                QString sDefault)
{
    QString retStr = QLatin1String("");
    QSettings settings(sFileName, QSettings::IniFormat);
    settings.beginGroup(sSecName);
    retStr = settings.value(sKey, sDefault).toString();
    settings.endGroup();
    return retStr.trimmed();
}

/**
 * @brief ConfigHelper::getCfgList 获取配置项目
 * @param sFileName ini文件名称
 * @param sSecName 组名称
 * @param sKeyName 键名称
 * @param strings 读取配置的数组值
 * @return 读取的个数
 */
int ConfigHelper::getCfgList(QString sFileName,
                             QString sSecName,
                             QString sKeyName,
                             QStringList &strings)
{
    QSettings settings(sFileName, QSettings::IniFormat);

    settings.beginGroup(sSecName);

    int iCount = settings.value(sKeyName + "-count").toInt();
    QString strValue;
    int iRet = 0;
    //qDebug()<< "iCount:" << iCount;
    for(int i = 0; i < iCount; i ++) {
        strValue = settings.value(sKeyName + "-" + QString::number(i + 1)).toString();
        strValue = strValue.trimmed();
        //qDebug()<< sKeyName + "-" + QString::number(i + 1) <<"=" << QString::fromLocal8Bit(strValue.toLatin1());
        if((strValue.length() != 0) && (strings.indexOf(strValue) < 0)) {
            strings << strValue;
            iRet ++;
        }
    }

    settings.endGroup();

    return iRet;
}

/**
 * @brief ConfigHelper::writeCfgList 设置配置项目
 * @param sFileName ini文件名称
 * @param sSecName 组名称
 * @param sKeyName 键名称
 * @param strings 写入配置的数组值
 */
void ConfigHelper::writeCfgList(QString sFileName,
                                QString sSecName,
                                QString sKeyName,
                                QStringList &strings)
{
    QSettings settings(sFileName, QSettings::IniFormat);

    settings.beginGroup(sSecName);

    settings.setValue(sKeyName + "-count", strings.count());
    for (int i = 0; i < strings.count(); i++) {
        settings.setValue(sKeyName + "-" + QString::number(i + 1), strings.at(i));
    }

    settings.endGroup();
}

/**
 * @brief ConfigHelper::setCfgInt 写整数配置值
 * @param sFileName ini文件名称
 * @param sSecName 组名称
 * @param sKey 键名称
 * @param iValue 设置值
 */
void ConfigHelper::setCfgInt(QString sFileName,
                             QString sSecName,
                             QString sKey,
                             int iValue)
{
    QSettings settings(sFileName, QSettings::IniFormat);
    settings.beginGroup(sSecName);
    settings.setValue(sKey, QString::number(iValue));
    settings.endGroup();
}

/**
 * @brief ConfigHelper::getCfgInt 读取整数配置值
 * @param sFileName ini文件名称
 * @param sSecName 组名称
 * @param sKey 键名称
 * @param iDefault 缺省值
 * @return 键值对应的字符串
 */
int ConfigHelper::getCfgInt(QString sFileName,
                            QString sSecName,
                            QString sKey,
                            int iDefault)
{
    int val = iDefault;
    QSettings settings(sFileName, QSettings::IniFormat);
    settings.beginGroup(sSecName);
    val = settings.value(sKey, iDefault).toInt();
    settings.endGroup();
    return val;
}

/**
 * @brief ConfigHelper::setCfgFloat 写字符配置单精度浮点值
 * @param sFileName ini文件名称
 * @param sSecName 组名称
 * @param sKey 键名称
 * @param fValue 设置值
 */
void ConfigHelper::setCfgFloat(QString sFileName,
                               QString sSecName,
                               QString sKey,
                               float fValue)
{
    QSettings settings(sFileName, QSettings::IniFormat);
    settings.beginGroup(sSecName);
    settings.setValue(sKey, QString::number(fValue));
    settings.endGroup();
}

/**
 * @brief ConfigHelper::getCfgFloat 读取配置单精度浮点值
 * @param sFileName ini文件名称
 * @param sSecName 组名称
 * @param sKey 键名称
 * @param fDefault 缺省值
 * @return 键值对应的字符串
 */
float ConfigHelper::getCfgFloat(QString sFileName,
                                QString sSecName,
                                QString sKey,
                                float fDefault)
{
    float val = fDefault;
    QSettings settings(sFileName, QSettings::IniFormat);
    settings.beginGroup(sSecName);
    val = settings.value(sKey, fDefault).toFloat();
    settings.endGroup();
    return val;
}

/**
 * @brief ConfigHelper::setCfgDouble 写字符配置单精度浮点值
 * @param sFileName ini文件名称
 * @param sSecName 组名称
 * @param sKey 键名称
 * @param dValue 设置值
 */
void ConfigHelper::setCfgDouble(QString sFileName,
                                QString sSecName,
                                QString sKey,
                                double dValue)
{
    char buffer[256];
    memset((void *)buffer, 0, 256);
    snprintf(buffer, 256, "%9.9lf", dValue);
    QSettings settings(sFileName, QSettings::IniFormat);
    settings.beginGroup(sSecName);
    settings.setValue(sKey, QString(buffer));
    settings.endGroup();
}

/**
 * @brief ConfigHelper::getCfgDouble 读取配置单精度浮点值
 * @param sFileName ini文件名称
 * @param sSecName 组名称
 * @param sKey 键名称
 * @param dDefault 缺省值
 * @return 键值对应的字符串
 */
double ConfigHelper::getCfgDouble(QString sFileName,
                                  QString sSecName,
                                  QString sKey,
                                  double dDefault)
{
    double val = dDefault;
    QSettings settings(sFileName, QSettings::IniFormat);
    settings.beginGroup(sSecName);
    val = settings.value(sKey, dDefault).toDouble();
    settings.endGroup();
    return val;
}
