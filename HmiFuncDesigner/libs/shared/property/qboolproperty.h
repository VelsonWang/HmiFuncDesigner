#ifndef QBOOLPROPERTY_H
#define QBOOLPROPERTY_H

#include "qabstractproperty.h"

class QBoolProperty: public QAbstractProperty
{
    Q_OBJECT
public:
    Q_INVOKABLE QBoolProperty(QAbstractProperty* parent=0);

    QString         get_value_text();
    QIcon           get_value_icon();
};

#endif // QBOOLPROPERTY_H
