#ifndef QDATETIMEEDITHOST_H
#define QDATETIMEEDITHOST_H

#include "qabstractspinboxhost.h"

class QDateTimeEditHost: public QAbstractSpinBoxHost
{
    Q_OBJECT
public:
    Q_INVOKABLE QDateTimeEditHost(QAbstractHost *parent = 0);

    static QString get_show_name();
    static QString get_show_icon();
    static QString get_show_group();
public slots:
    void    setDateTime(int year,int month,int day,int hour,int minute,int second);
    int     year();
    int     month();
    int     day();
    int     hour();
    int     minute();
    int     second();

    void    setCurrentSection(int currentSection);
    int     currentSection();
protected slots:
    void	dateTimeChanged(const QDateTime &datetime);
protected:
    void init_property();
protected:
    void    create_object();
};


#endif // QDATETIMEEDITHOST_H
