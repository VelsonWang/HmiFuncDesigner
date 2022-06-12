#ifndef SERIALPORTREMAPPING_H
#define SERIALPORTREMAPPING_H

#include <QMap>
#include "Singleton.h"

class SerialPortReMapping
{
public:
    SerialPortReMapping();
    ~SerialPortReMapping();

    // 获取重映射后的串口名称
    QString getSerialPortName(const QString &szOldName) const;

private:
    QMap<QString, QString> mapSerialPortName_;
};

#endif // SERIALPORTREMAPPING_H
