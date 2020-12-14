#ifndef QSLIDERHOST_H
#define QSLIDERHOST_H

#include "qabstractsliderhost.h"

class QSliderHost: public QAbstractSliderHost
{
    Q_OBJECT
public:
    Q_INVOKABLE QSliderHost(QAbstractHost *parent = 0);

    static QString get_show_name();
    static QString get_show_icon();
    static QString get_show_group();
protected:
    void init_property();
protected:
    void    create_object();
};

#endif // QSLIDERHOST_H
