#ifndef QTAGCOLORLISTPROPERTY_H
#define QTAGCOLORLISTPROPERTY_H

#include "qabstractproperty.h"

class QTagColorListProperty : public QAbstractProperty
{
    Q_OBJECT
public:
    Q_INVOKABLE QTagColorListProperty(QAbstractProperty* parent = NULL);
    QIcon getValueIcon();
    QString getValueText();
};

#endif
