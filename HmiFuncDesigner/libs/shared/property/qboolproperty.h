#ifndef QBOOLPROPERTY_H
#define QBOOLPROPERTY_H

#include "qabstractproperty.h"

class QBoolProperty : public QAbstractProperty
{
    Q_OBJECT
public:
    Q_INVOKABLE QBoolProperty(QAbstractProperty* parent = 0);

    QString getValueText();
    QIcon getValueIcon();
};

#endif // QBOOLPROPERTY_H

