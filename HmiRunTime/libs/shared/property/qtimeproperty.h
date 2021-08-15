#ifndef QTIMEPROPERTY_H
#define QTIMEPROPERTY_H

#include "qabstractproperty.h"

#include <QTime>

class QTimeProperty: public QAbstractProperty
{
    Q_OBJECT
public:
    Q_INVOKABLE QTimeProperty(QAbstractProperty* parent = 0);

    QString get_value_text();
    QIcon get_value_icon();
	
protected:
    void make_value();
    void write_value();
	
};

#endif // QTIMEPROPERTY_H

