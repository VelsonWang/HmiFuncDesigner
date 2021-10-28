#ifndef QDATETIMEPROPERTY_H
#define QDATETIMEPROPERTY_H

#include "qabstractproperty.h"

#include <QDateTime>

class QDateTimeProperty : public QAbstractProperty
{
    Q_OBJECT
public:
    Q_INVOKABLE QDateTimeProperty(QAbstractProperty* parent = 0);

    QString getValueText();
    QIcon getValueIcon();
	
protected:
    void makeValue();
    void writeValue();
};

#endif // QDATETIMEPROPERTY_H

