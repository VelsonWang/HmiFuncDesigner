#ifndef QTIMEEDITHOST_H
#define QTIMEEDITHOST_H


#include "qabstractspinboxhost.h"

class QTimeEditHost: public QAbstractSpinBoxHost
{
    Q_OBJECT
public:
    Q_INVOKABLE QTimeEditHost(QAbstractHost *parent = 0);

    static QString get_show_name();
    static QString get_show_icon();
    static QString get_show_group();
public slots:
    void    setTime(int hour,int minute,int second);
    int     hour();
    int     minute();
    int     second();

    void    setCurrentSection(int currentSection);
    int     currentSection();
protected slots:
    void    timeChanged(const QTime &time);
protected:
    void init_property();
protected:
    void    create_object();
};


#endif // QTIMEEDITHOST_H
