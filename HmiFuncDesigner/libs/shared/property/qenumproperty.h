#ifndef QENUMPROPERTY_H
#define QENUMPROPERTY_H

#include "qabstractproperty.h"

class QEnumProperty : public QAbstractProperty
{
    Q_OBJECT
public:
    Q_INVOKABLE QEnumProperty(QAbstractProperty* parent = 0);
    QIcon getValueIcon();
    QString getValueText();
};

#endif // QENUMPROPERTY_H

