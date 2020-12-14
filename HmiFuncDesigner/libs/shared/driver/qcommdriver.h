#ifndef QCOMMDRIVER_H
#define QCOMMDRIVER_H

#include "qabstractdriver.h"

class QCommDriverHost : public QAbstractDriver
{
    Q_OBJECT
public:
    Q_INVOKABLE QCommDriverHost(QAbstractHost* parent=0);

    static QString get_show_name();
protected:
    void    create_object();
    void    init_property();
};

class QCommDriverObject: public QAbstractDriverObject
{
    Q_OBJECT
public:
    void    start();
    void    close();
};

#endif // QCOMMDRIVER_H
