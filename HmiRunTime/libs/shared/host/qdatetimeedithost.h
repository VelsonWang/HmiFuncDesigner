#ifndef QDATETIMEEDITHOST_H
#define QDATETIMEEDITHOST_H

#include "qabstractspinboxhost.h"

class QDateTimeEditHost: public QAbstractSpinBoxHost
{
    Q_OBJECT
public:
    Q_INVOKABLE QDateTimeEditHost(QAbstractHost *parent = 0);

    static QString getShowName();
    static QString getShowIcon();
    static QString getShowGroup();
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
    void initProperty();
protected:
    void    createObject();
};


#endif // QDATETIMEEDITHOST_H
