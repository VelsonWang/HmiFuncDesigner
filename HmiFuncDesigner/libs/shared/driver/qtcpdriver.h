#ifndef QTCPDRIVER_H
#define QTCPDRIVER_H

#include "qabstractdriver.h"

class QTCPDriverHost : public QAbstractDriver
{
    Q_OBJECT
public:
    Q_INVOKABLE QTCPDriverHost(QAbstractHost* parent=0);

    static QString get_show_name();
protected:
    void    create_object();
    void    init_property();
};

class QTCPDriverObject: public QAbstractDriverObject
{
    Q_OBJECT
public:
    void    start();
    void    close();
};

#endif // QTCPDRIVER_H
