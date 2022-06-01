#ifndef QCHECKBOXHOST_H
#define QCHECKBOXHOST_H

#include "qabstractbuttonhost.h"

class QCheckBoxHost: public QAbstractButtonHost
{
    Q_OBJECT
public:
    Q_INVOKABLE QCheckBoxHost(QAbstractHost *parent = 0);

    static QString getShowName();
    static QString getShowIcon();
    static QString getShowGroup();
public slots:
    void setTristate(bool tristate);
    bool tristate();

protected slots:
    void stateChanged(int state);

protected:
    void initProperty() override;

protected:
    void createObject() override;

};

#endif // QCHECKBOXHOST_H
