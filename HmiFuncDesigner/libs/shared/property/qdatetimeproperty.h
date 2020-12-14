#ifndef QDATETIMEPROPERTY_H
#define QDATETIMEPROPERTY_H

#include "qabstractproperty.h"

#include <QDateTime>

class QDateTimeProperty: public QAbstractProperty
{
    Q_OBJECT
public:
    Q_INVOKABLE QDateTimeProperty(QAbstractProperty* parent=0);

    QString         get_value_text();
    QIcon           get_value_icon();
protected:
    void    make_value();
    void    write_value();
};

#endif // QDATETIMEPROPERTY_H
