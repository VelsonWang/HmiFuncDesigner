#ifndef QCOMBOBOXHOST_H
#define QCOMBOBOXHOST_H

#include "qwidgethost.h"

class QComboBoxHost : public QWidgetHost
{
    Q_OBJECT
public:
    Q_INVOKABLE QComboBoxHost(QAbstractHost *parent = 0);

    static QString get_show_name();
    static QString get_show_icon();
    static QString get_show_group();
protected:
    void    create_object();
};

#endif // QCOMBOBOXHOST_H
