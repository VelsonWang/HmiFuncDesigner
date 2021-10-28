#ifndef QINTPROPERTY_H
#define QINTPROPERTY_H

#include "qabstractproperty.h"


class QIntProperty: public QAbstractProperty
{
    Q_OBJECT
public:
    Q_INVOKABLE QIntProperty(QAbstractProperty* parent = 0);

    QString getValueText();
    QIcon getValueIcon();
};

#endif // QINTPROPERTY_H
