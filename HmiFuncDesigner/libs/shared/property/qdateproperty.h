#ifndef QDATEPROPERTY_H
#define QDATEPROPERTY_H

#include "qabstractproperty.h"

#include <QDate>

class QDateProperty : public QAbstractProperty
{
    Q_OBJECT
public:
    Q_INVOKABLE QDateProperty(QAbstractProperty* parent = 0);

    QString getValueText();
    QIcon getValueIcon();
	
protected:
    void makeValue();
    void writeValue();
};

#endif // QDATEPROPERTY_H

