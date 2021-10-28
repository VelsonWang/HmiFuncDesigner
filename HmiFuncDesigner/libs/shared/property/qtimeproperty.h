#ifndef QTIMEPROPERTY_H
#define QTIMEPROPERTY_H

#include "qabstractproperty.h"

#include <QTime>

class QTimeProperty: public QAbstractProperty
{
    Q_OBJECT
public:
    Q_INVOKABLE QTimeProperty(QAbstractProperty* parent = 0);

    QString getValueText();
    QIcon getValueIcon();

protected:
    void makeValue();
    void writeValue();

};

#endif // QTIMEPROPERTY_H

