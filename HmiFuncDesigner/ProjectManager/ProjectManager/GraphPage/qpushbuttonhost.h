#ifndef QPUSHBUTTONHOST_H
#define QPUSHBUTTONHOST_H

#include "qabstractbuttonhost.h"

class QPushButtonHost: public QAbstractButtonHost
{
    Q_OBJECT
public:
    Q_INVOKABLE QPushButtonHost(QAbstractHost *parent = Q_NULLPTR);

    static QString getShowName();
    static QString getShowIcon();
    static QString getShowGroup();

protected:
    void createObject();
};

#endif // QPUSHBUTTONHOST_H
