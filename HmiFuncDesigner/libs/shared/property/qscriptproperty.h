#ifndef QSCRIPTPROPERTY_H
#define QSCRIPTPROPERTY_H

#include "qabstractproperty.h"

class QScriptProperty : public QAbstractProperty
{
    Q_OBJECT
public:
    Q_INVOKABLE QScriptProperty(QAbstractProperty* parent = 0);

    QString get_value_text();
    QIcon get_value_icon();
};

#endif // QSCRIPTPROPERTY_H

