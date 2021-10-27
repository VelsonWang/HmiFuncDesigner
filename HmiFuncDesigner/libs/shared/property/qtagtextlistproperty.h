#ifndef QTAGTEXTLISTPROPERTY_H
#define QTAGTEXTLISTPROPERTY_H

#include "qabstractproperty.h"

class QTagTextListProperty : public QAbstractProperty
{
    Q_OBJECT
public:
    Q_INVOKABLE QTagTextListProperty(QAbstractProperty* parent = NULL);
    QIcon get_value_icon();
    QString get_value_text();
};

#endif
