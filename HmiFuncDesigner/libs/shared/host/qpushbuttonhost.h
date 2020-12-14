#ifndef QPUSHBUTTONHOST_H
#define QPUSHBUTTONHOST_H

#include "qabstractbuttonhost.h"

class QPushButtonHost: public QAbstractButtonHost
{
    Q_OBJECT
public:
    Q_INVOKABLE QPushButtonHost(QAbstractHost *parent = 0);

    static QString get_show_name();
    static QString get_show_icon();
    static QString get_show_group();
protected:
    void    init_property();
protected:
    void    create_object();
};

#endif // QPUSHBUTTONHOST_H
