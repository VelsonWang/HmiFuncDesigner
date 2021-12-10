#ifndef QCOMBOBOXHOST_H
#define QCOMBOBOXHOST_H

#include "qwidgethost.h"

class QComboBoxHost : public QWidgetHost
{
    Q_OBJECT
public:
    Q_INVOKABLE QComboBoxHost(QAbstractHost *parent = 0);

    static QString getShowName();
    static QString getShowIcon();
    static QString getShowGroup();
protected:
    void    createObject();
};

#endif // QCOMBOBOXHOST_H
