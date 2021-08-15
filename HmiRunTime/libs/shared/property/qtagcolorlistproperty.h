#ifndef QTAGCOLORLISTPROPERTY_H
#define QTAGCOLORLISTPROPERTY_H

#include "qabstractproperty.h"

class QTagColorListProperty : public QAbstractProperty
{
    Q_OBJECT
public:
    Q_INVOKABLE QTagColorListProperty(QAbstractProperty* parent = Q_NULLPTR);
    QIcon get_value_icon();
    QString get_value_text();
};

#endif
