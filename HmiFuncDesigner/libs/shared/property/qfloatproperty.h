#ifndef QFLOATPROPERTY_H
#define QFLOATPROPERTY_H

#include "qabstractproperty.h"


class QFloatProperty : public QAbstractProperty
{
    Q_OBJECT
public:
    Q_INVOKABLE QFloatProperty(QAbstractProperty* parent = 0);

    QString getValueText();
    QIcon getValueIcon();
};


#endif // QFLOATPROPERTY_H

