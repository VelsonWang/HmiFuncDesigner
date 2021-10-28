#ifndef QCOLORPROPERTY_H
#define QCOLORPROPERTY_H

#include "qabstractproperty.h"
#include <QLabel>

class QColorProperty : public QAbstractProperty
{
    Q_OBJECT
public:
    Q_INVOKABLE QColorProperty(QAbstractProperty* parent = NULL);
    QIcon getValueIcon();
    QString getValueText();
};

#endif
