#ifndef QSPINBOXHOST_H
#define QSPINBOXHOST_H

#include "qabstractspinboxhost.h"

class QSpinBoxHost: public QAbstractSpinBoxHost
{
    Q_OBJECT
public:
    Q_INVOKABLE QSpinBoxHost(QAbstractHost *parent = 0);

    static QString get_show_name();
    static QString get_show_icon();
    static QString get_show_group();
public slots:
    void    setValue(int value);
    int     value();

    void    setSingleStep(int singleStep);
    int     singleStep();
protected slots:
    void    valueChanged(const QString &value);
protected:
    void init_property();
protected:
    void    create_object();
};

#endif // QSPINBOXHOST_H
