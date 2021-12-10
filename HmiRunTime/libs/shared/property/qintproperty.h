#ifndef QINTPROPERTY_H
#define QINTPROPERTY_H

#include "qabstractproperty.h"


class QIntProperty: public QAbstractProperty
{
    Q_OBJECT
public:
    Q_INVOKABLE QIntProperty(QAbstractProperty* parent = 0);

    QString get_value_text();
    QIcon get_value_icon();
};

#endif // QINTPROPERTY_H
