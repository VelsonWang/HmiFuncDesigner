#ifndef QTHERMOMETERHOST_H
#define QTHERMOMETERHOST_H

#include "qframehost.h"

class QThermometerHost : public QFrameHost
{
    Q_OBJECT
public:
    Q_INVOKABLE QThermometerHost(QAbstractHost *parent = 0);

    static QString getShowName();
    static QString getShowIcon();
    static QString getShowGroup();

protected:
    void initProperty() override;

protected:
    void createObject() override;

};

#endif // QThermometerHost_H
