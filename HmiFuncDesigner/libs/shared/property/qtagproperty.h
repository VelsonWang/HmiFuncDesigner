#ifndef QTAGPROPERTY_H
#define QTAGPROPERTY_H

#include "qabstractproperty.h"

class QTagProperty : public QAbstractProperty
{
    Q_OBJECT
public:
    Q_INVOKABLE QTagProperty(QAbstractProperty* parent = NULL);
    QIcon getValueIcon();
    QString getValueText();
};

#endif // QTAGPROPERTY_H

