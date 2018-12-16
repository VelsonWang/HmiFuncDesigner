#include "configutils.h"
#include <QString>
#include <QSettings>
#include <QApplication>
#include <QFile>
#include <QMessageBox>
#include <QDebug>

ConfigUtils::ConfigUtils()
{

}

// 取得程序所在目录
QString ConfigUtils::AppDir()
{
    return QCoreApplication::applicationDirPath();
}

//写字符配置值
void ConfigUtils::SetCfgStr(QString sFileName, QString sSecName, QString sIdent, QString sValue)
{
    QSettings settings(sFileName, QSettings::IniFormat);
    settings.beginGroup(sSecName);
    settings.setValue(sIdent, sValue);
    settings.endGroup();
}

//读取配置字符串
QString ConfigUtils::GetCfgStr(QString sFileName, QString sSecName, QString sIdent, QString sDefault)
{
    QString retStr = QLatin1String("");
    QSettings settings(sFileName, QSettings::IniFormat);
    settings.beginGroup(sSecName);
    retStr = settings.value(sIdent, sDefault).toString();
    settings.endGroup();
    return retStr.trimmed();
}

//获取配置项目
int ConfigUtils::GetCfgList(QString sFileName, QString sSecName, QString sKeyName, QStringList &strings)
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

//获取配置项目
void ConfigUtils::WriteCfgList(QString sFileName, QString sSecName, QString sKeyName, QStringList &strings)
{
    QSettings settings(sFileName, QSettings::IniFormat);

    settings.beginGroup(sSecName);

    settings.setValue(sKeyName + "-count", strings.count());
    for(int i = 0; i < strings.count(); i ++)
        settings.setValue(sKeyName + "-" + QString::number(i + 1), strings.at(i));

    settings.endGroup();
}


