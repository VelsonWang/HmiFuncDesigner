#ifndef QTIMEEDITHOST_H
#define QTIMEEDITHOST_H


#include "qabstractspinboxhost.h"

class QTimeEditHost: public QAbstractSpinBoxHost
{
    Q_OBJECT
public:
    Q_INVOKABLE QTimeEditHost(QAbstractHost *parent = 0);

    static QString getShowName();
    static QString getShowIcon();
    static QString getShowGroup();

public slots:
    void setTime(int hour, int minute, int second);
    int hour();
    int minute();
    int second();

    void setCurrentSection(int currentSection);
    int currentSection();

protected slots:
    void timeChanged(const QTime &time);

protected:
    void initProperty() override;

protected:
    void createObject() override;

};


#endif // QTIMEEDITHOST_H
