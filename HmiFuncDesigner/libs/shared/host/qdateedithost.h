#ifndef QDATEEDITHOST_H
#define QDATEEDITHOST_H

#include "qabstractspinboxhost.h"

class QDateEditHost: public QAbstractSpinBoxHost
{
    Q_OBJECT
public:
    Q_INVOKABLE QDateEditHost(QAbstractHost *parent = 0);

    static QString get_show_name();
    static QString get_show_icon();
    static QString get_show_group();
public slots:
    void    setDate(int year,int month,int day);
    int     year();
    int     month();
    int     day();

    void    setCurrentSection(int currentSection);
    int     currentSection();
protected slots:
    void    dateChanged(const QDate &date);
protected:
    void init_property();
protected:
    void    create_object();
};

#endif // QDATEEDITHOST_H
