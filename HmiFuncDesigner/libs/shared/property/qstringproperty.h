#ifndef QSTRINGPROPERTY_H
#define QSTRINGPROPERTY_H

#include "qabstractproperty.h"

class QStringProperty : public QAbstractProperty
{
    Q_OBJECT
public:
    Q_INVOKABLE QStringProperty(QAbstractProperty* parent = 0);

    QString getValueText();
    QIcon getValueIcon();
};

#endif // QSTRINGPROPERTY_H

