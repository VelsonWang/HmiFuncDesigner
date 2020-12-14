#ifndef QLCDNUMBERHOST_H
#define QLCDNUMBERHOST_H

#include "qframehost.h"

class QLCDNumberHost : public QFrameHost
{
    Q_OBJECT
public:
    Q_INVOKABLE QLCDNumberHost(QAbstractHost *parent = 0);

    static QString get_show_name();
    static QString get_show_icon();
    static QString get_show_group();
public slots:
    void    setIntValue(int intValue);
    int     intValue();

    void    setValue(float value);
    float   value();

    void    setMode(int mode);
    int     mode();

    void    setSegmentStyle(int segmentStyle);
    int     segmentStyle();

    void    setDigitCount(int digitCount);
    int     digitCount();

    void    setNumDigits(int numDigits);
    int     numDigits();
protected:
    void    init_property();
protected:
    void    create_object();
};

#endif // QLCDNUMBERHOST_H
