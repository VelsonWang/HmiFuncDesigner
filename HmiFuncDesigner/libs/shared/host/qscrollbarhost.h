#ifndef QSCROLLBARHOST_H
#define QSCROLLBARHOST_H

#include "qabstractsliderhost.h"

class QScrollBarHost: public QAbstractSliderHost
{
    Q_OBJECT
public:
    Q_INVOKABLE QScrollBarHost(QAbstractHost *parent = 0);

    static QString getShowName();
    static QString getShowIcon();
    static QString getShowGroup();
protected:
    void    initProperty();
protected:
    void    createObject();
};
#endif // QSCROLLBARHOST_H
