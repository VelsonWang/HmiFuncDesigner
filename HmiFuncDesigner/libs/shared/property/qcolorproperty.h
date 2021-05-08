#ifndef QCOLORPROPERTY_H
#define QCOLORPROPERTY_H

#include "qabstractproperty.h"
#include <QLabel>

class QColorProperty : public QAbstractProperty
{
    Q_OBJECT
public:
    Q_INVOKABLE QColorProperty(QAbstractProperty* parent = Q_NULLPTR);
    QIcon get_value_icon();
    QString get_value_text();
};

#endif
