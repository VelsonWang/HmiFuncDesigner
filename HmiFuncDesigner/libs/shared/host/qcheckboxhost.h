#ifndef QCHECKBOXHOST_H
#define QCHECKBOXHOST_H

#include "qabstractbuttonhost.h"

class QCheckBoxHost: public QAbstractButtonHost
{
    Q_OBJECT
public:
    Q_INVOKABLE QCheckBoxHost(QAbstractHost *parent = 0);

    static QString get_show_name();
    static QString get_show_icon();
    static QString get_show_group();
public slots:
    void    setTristate(bool tristate);
    bool    tristate();
protected slots:
    void    stateChanged(int state);
protected:
    void    init_property();
protected:
    void    create_object();
};

#endif // QCHECKBOXHOST_H
