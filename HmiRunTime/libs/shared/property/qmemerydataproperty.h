#ifndef QMEMERYDATAPROPERTY_H
#define QMEMERYDATAPROPERTY_H

#include "qabstractproperty.h"

class tagMemeryValueInfo;

class QMemeryDataProperty : public QAbstractProperty
{
    Q_OBJECT
public:
    Q_INVOKABLE QMemeryDataProperty(QAbstractProperty *parent = 0);

    QIcon get_value_icon();
    QString get_value_text();
    void toObject(XMLObject *xml);
    void fromObject(XMLObject *xml);
};

#endif // QMEMERYDATAPROPERTY_H

