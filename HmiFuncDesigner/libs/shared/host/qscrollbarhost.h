#ifndef QSCROLLBARHOST_H
#define QSCROLLBARHOST_H

#include "qabstractsliderhost.h"

class QScrollBarHost: public QAbstractSliderHost
{
    Q_OBJECT
public:
    Q_INVOKABLE QScrollBarHost(QAbstractHost *parent = 0);

    static QString get_show_name();
    static QString get_show_icon();
    static QString get_show_group();
protected:
    void    init_property();
protected:
    void    create_object();
};
#endif // QSCROLLBARHOST_H
