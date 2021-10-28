#ifndef QFILEPROPERTY_H
#define QFILEPROPERTY_H

#include "qabstractproperty.h"
#include <QLabel>

class QFileProperty : public QAbstractProperty
{
    Q_OBJECT
public:
    Q_INVOKABLE QFileProperty(QAbstractProperty* parent = NULL);
    QIcon getValueIcon();
    QString getValueText();
};

#endif
