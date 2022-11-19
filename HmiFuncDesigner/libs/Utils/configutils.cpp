#include "configutils.h"
#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <QSettings>
#include <QString>

/**
 * @brief ConfigUtils::setCfgStr 写字符配置值
 * @param sFileName ini文件名称
 * @param sSecName 组名称
 * @param sKey 键名称
 * @param sValue 设置值
 */
void ConfigUtils::setCfgStr(QString sFileName,
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
 * @brief ConfigUtils::getCfgStr 读取配置字符串
 * @param sFileName ini文件名称
 * @param sSecName 组名称
 * @param sKey 键名称
 * @param sDefault 缺省值
 * @return 键值对应的字符串
 */
QString ConfigUtils::getCfgStr(QString sFileName,
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
 * @brief ConfigUtils::getCfgList 获取配置项目
 * @param sFileName ini文件名称
 * @param sSecName 组名称
 * @param sKeyName 键名称
 * @param strings 读取配置的数组值
 * @return 读取的个数
 */
int ConfigUtils::getCfgList(QString sFileName,
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
    for(int i = 0; i < iCount; i ++)
    {
        strValue = settings.value(sKeyName + "-" + QString::number(i + 1)).toString();
        strValue = strValue.trimmed();
        //qDebug()<< sKeyName + "-" + QString::number(i + 1) <<"=" << QString::fromLocal8Bit(strValue.toLatin1());
        if((strValue.length() != 0) && (strings.indexOf(strValue) < 0))
        {
            strings << strValue;
            iRet ++;
        }
    }

  settings.endGroup();

  return iRet;
}

/**
 * @brief ConfigUtils::writeCfgList 设置配置项目
 * @param sFileName ini文件名称
 * @param sSecName 组名称
 * @param sKeyName 键名称
 * @param strings 写入配置的数组值
 */
void ConfigUtils::writeCfgList(QString sFileName,
                               QString sSecName,
                               QString sKeyName,
                               QStringList &strings)
{
    QSettings settings(sFileName, QSettings::IniFormat);

  settings.beginGroup(sSecName);

  settings.setValue(sKeyName + "-count", strings.count());
  for (int i = 0; i < strings.count(); i++)
    settings.setValue(sKeyName + "-" + QString::number(i + 1), strings.at(i));

  settings.endGroup();
}

/**
 * @brief ConfigUtils::setCfgInt 写整数配置值
 * @param sFileName ini文件名称
 * @param sSecName 组名称
 * @param sKey 键名称
 * @param iValue 设置值
 */
void ConfigUtils::setCfgInt(QString sFileName,
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
 * @brief ConfigUtils::getCfgInt 读取整数配置值
 * @param sFileName ini文件名称
 * @param sSecName 组名称
 * @param sKey 键名称
 * @param iDefault 缺省值
 * @return 键值对应的字符串
 */
int ConfigUtils::getCfgInt(QString sFileName,
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
 * @brief ConfigUtils::setCfgFloat 写字符配置单精度浮点值
 * @param sFileName ini文件名称
 * @param sSecName 组名称
 * @param sKey 键名称
 * @param fValue 设置值
 */
void ConfigUtils::setCfgFloat(QString sFileName,
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
 * @brief ConfigUtils::getCfgFloat 读取配置单精度浮点值
 * @param sFileName ini文件名称
 * @param sSecName 组名称
 * @param sKey 键名称
 * @param fDefault 缺省值
 * @return 键值对应的字符串
 */
float ConfigUtils::getCfgFloat(QString sFileName,
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
 * @brief ConfigUtils::setCfgDouble 写字符配置单精度浮点值
 * @param sFileName ini文件名称
 * @param sSecName 组名称
 * @param sKey 键名称
 * @param dValue 设置值
 */
void ConfigUtils::setCfgDouble(QString sFileName,
                              QString sSecName,
                              QString sKey,
                              double dValue)
{
    QSettings settings(sFileName, QSettings::IniFormat);
    settings.beginGroup(sSecName);
    settings.setValue(sKey, QString::asprintf("%9.9lf", dValue));
    settings.endGroup();
}

/**
 * @brief ConfigUtils::getCfgDouble 读取配置单精度浮点值
 * @param sFileName ini文件名称
 * @param sSecName 组名称
 * @param sKey 键名称
 * @param dDefault 缺省值
 * @return 键值对应的字符串
 */
double ConfigUtils::getCfgDouble(QString sFileName,
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
