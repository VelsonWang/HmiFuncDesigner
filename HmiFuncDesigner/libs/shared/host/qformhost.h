#ifndef QFORMHOST_H
#define QFORMHOST_H

#include "../sharedlibglobal.h"

#include "qabstractwidgethost.h"

#define FORM_TITLE "form"

class SHAREDLIB_EXPORT QFormHost : public QAbstractWidgetHost
{
    Q_OBJECT
public:
    QFormHost(QAbstractHost *parent = 0);

protected:
    void    init_property();
protected:
    void    create_object();
};

#endif // QFORMHOST_H
