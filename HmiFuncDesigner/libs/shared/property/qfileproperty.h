#ifndef QFILEPROPERTY_H
#define QFILEPROPERTY_H

#include "qabstractproperty.h"
#include <QLabel>

class QFileProperty : public QAbstractProperty
{
    Q_OBJECT
public:
    Q_INVOKABLE QFileProperty(QAbstractProperty* parent = Q_NULLPTR);
    QIcon get_value_icon();
    QString get_value_text();
};

#endif
