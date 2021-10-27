#ifndef QTAGPROPERTY_H
#define QTAGPROPERTY_H

#include "qabstractproperty.h"

class QTagProperty : public QAbstractProperty
{
    Q_OBJECT
public:
    Q_INVOKABLE QTagProperty(QAbstractProperty* parent = NULL);
    QIcon get_value_icon();
    QString get_value_text();
};

#endif // QTAGPROPERTY_H

