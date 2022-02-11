#ifndef QSPINBOXHOST_H
#define QSPINBOXHOST_H

#include "qabstractspinboxhost.h"

class QSpinBoxHost: public QAbstractSpinBoxHost
{
    Q_OBJECT
public:
    Q_INVOKABLE QSpinBoxHost(QAbstractHost *parent = 0);

    static QString getShowName();
    static QString getShowIcon();
    static QString getShowGroup();
public slots:
    void    setValue(int value);
    int     value();

    void    setSingleStep(int singleStep);
    int     singleStep();
protected slots:
    void    valueChanged(const QString &value);
protected:
    void initProperty();
protected:
    void    createObject();
};

#endif // QSPINBOXHOST_H
