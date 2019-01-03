#ifndef CONFIGUTILS_H
#define CONFIGUTILS_H

#include <QString>

class ConfigUtils
{
private:
    ConfigUtils(){}
    ~ConfigUtils(){}

public:
    // 写字符配置值
    static void setCfgStr(QString sFileName,
                          QString sSecName,
                          QString sKey,
                          QString sValue);
    // 读取配置字符串
    static QString getCfgStr(QString sFileName,
                             QString sSecName,
                             QString sKey,
                             QString sDefault = "");
    // 获取配置项目
    static int getCfgList(QString sFileName,
                          QString sSecName,
                          QString sKeyName,
                          QStringList &strings);
    // 获取配置项目
    static void writeCfgList(QString sFileName,
                             QString sSecName,
                             QString sKeyName,
                             QStringList &strings);

    // 写字符配置整数值
    static void setCfgInt(QString sFileName,
                          QString sSecName,
                          QString sKey,
                          int iValue);
    // 读取配置整数值
    static int getCfgInt(QString sFileName,
                         QString sSecName,
                         QString sKey,
                         int iDefault = 0);

    // 写字符配置单精度浮点值
    static void setCfgFloat(QString sFileName,
                            QString sSecName,
                            QString sKey,
                            float fValue);
    // 读取配置单精度浮点值
    static float getCfgFloat(QString sFileName,
                             QString sSecName,
                             QString sKey,
                             float fDefault = 0.0);

    // 写字符配置双精度浮点值
    static void setCfgDouble(QString sFileName,
                             QString sSecName,
                             QString sKey,
                             double dValue);
    // 读取配置双精度浮点值
    static double getCfgDouble(QString sFileName,
                               QString sSecName,
                               QString sKey,
                               double dDefault = 0.0);
};


#endif // CONFIGUTILS_H
