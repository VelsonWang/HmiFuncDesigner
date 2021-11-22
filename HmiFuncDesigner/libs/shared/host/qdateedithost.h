#ifndef QDATEEDITHOST_H
#define QDATEEDITHOST_H

#include "qabstractspinboxhost.h"

class QDateEditHost: public QAbstractSpinBoxHost
{
    Q_OBJECT
public:
    Q_INVOKABLE QDateEditHost(QAbstractHost *parent = 0);

    static QString getShowName();
    static QString getShowIcon();
    static QString getShowGroup();

public slots:
    void setDate(int year, int month, int day);
    int year();
    int month();
    int day();

    void setCurrentSection(int currentSection);
    int currentSection();

protected slots:
    void dateChanged(const QDate &date);

protected:
    void initProperty() override;

protected:
    void createObject() override;
    // 控件支持的功能事件
    QStringList supportFuncEvents() override;
};

#endif // QDATEEDITHOST_H
