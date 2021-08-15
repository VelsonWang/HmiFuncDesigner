#ifndef QFLOATPROPERTY_H
#define QFLOATPROPERTY_H

#include "qabstractproperty.h"


class QFloatProperty : public QAbstractProperty
{
    Q_OBJECT
public:
    Q_INVOKABLE QFloatProperty(QAbstractProperty* parent = 0);

    QString get_value_text();
    QIcon get_value_icon();
};


#endif // QFLOATPROPERTY_H

