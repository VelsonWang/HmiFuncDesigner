#ifndef QDIALHOST_H
#define QDIALHOST_H

#include "qabstractsliderhost.h"

class QDialHost: public QAbstractSliderHost
{
    Q_OBJECT
public:
    Q_INVOKABLE QDialHost(QAbstractHost *parent = 0);

    static QString get_show_name();
    static QString get_show_icon();
    static QString get_show_group();
public slots:

    void    setNotchTarget(float notchTarget);
    float   notchTarget();

    void    setWrapping(bool wrapping);
    bool    wrapping();

    void    setNotchesVisible(bool notchesVisible);
    bool    notchesVisible();
protected:
    void    init_property();
protected:
    void    create_object();
};

#endif // QDIALHOST_H
