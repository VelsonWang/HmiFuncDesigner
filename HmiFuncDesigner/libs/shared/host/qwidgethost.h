#ifndef QWIDGETHOST_H
#define QWIDGETHOST_H

#include "qabstractwidgethost.h"

class QWidgetHost : public QAbstractWidgetHost
{
    Q_OBJECT
public:
    Q_INVOKABLE QWidgetHost(QAbstractHost *parent = 0);

    static QString get_show_name();
    static QString get_show_icon();
    static QString get_show_group();
public slots:
    void   setVisible(bool visible);
    bool   visible();

    void    set_focus();
    void    kill_focus();
protected:
    void    create_object();
};

#endif // QWIDGETHOST_H
