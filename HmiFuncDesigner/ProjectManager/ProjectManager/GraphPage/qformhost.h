#ifndef QFORMHOST_H
#define QFORMHOST_H


#include "qabstractwidgethost.h"

#define FORM_TITLE "form"

class QFormHost : public QAbstractWidgetHost
{
    Q_OBJECT
public:
    QFormHost(QAbstractHost *parent = Q_NULLPTR);

protected:
    void createObject();
};

#endif // QFORMHOST_H
