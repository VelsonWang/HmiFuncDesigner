#ifndef CONFIGUTILS_H
#define CONFIGUTILS_H

#include <QString>

class ConfigUtils
{

public:
    ConfigUtils();

public:
    // 取得程序所在目录
    static QString AppDir();
    //写字符配置值
    static void SetCfgStr(QString sFileName, QString sSecName, QString sIdent, QString sValue);
    //读取配置字符串
    static QString GetCfgStr(QString sFileName, QString sSecName, QString sIdent, QString sDefault = "");
    //获取配置项目
    static int GetCfgList(QString sFileName, QString sSecName, QString sKeyName, QStringList &strings);
    //获取配置项目
    static void WriteCfgList(QString sFileName, QString sSecName, QString sKeyName, QStringList &strings);

};


#endif // CONFIGUTILS_H
