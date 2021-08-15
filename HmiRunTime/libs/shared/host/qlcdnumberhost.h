#ifndef QLCDNUMBERHOST_H
#define QLCDNUMBERHOST_H

#include "qframehost.h"

class QLCDNumberHost : public QFrameHost
{
    Q_OBJECT
public:
    Q_INVOKABLE QLCDNumberHost(QAbstractHost *parent = 0);

    static QString getShowName();
    static QString getShowIcon();
    static QString getShowGroup();
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
    void    initProperty();
protected:
    void    createObject();
};

#endif // QLCDNUMBERHOST_H
