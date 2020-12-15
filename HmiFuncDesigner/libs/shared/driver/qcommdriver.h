#ifndef QCOMMDRIVER_H
#define QCOMMDRIVER_H

#include "qabstractdriver.h"

class QCommDriverHost : public QAbstractDriver
{
    Q_OBJECT
public:
    Q_INVOKABLE QCommDriverHost(QAbstractHost* parent=0);

    static QString getShowName();
protected:
    void    createObject();
    void    initProperty();
};

class QCommDriverObject: public QAbstractDriverObject
{
    Q_OBJECT
public:
    void    start();
    void    close();
};

#endif // QCOMMDRIVER_H
