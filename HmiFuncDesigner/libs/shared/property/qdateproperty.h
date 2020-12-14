#ifndef QDATEPROPERTY_H
#define QDATEPROPERTY_H

#include "qabstractproperty.h"

#include <QDate>

class QDateProperty: public QAbstractProperty
{
    Q_OBJECT
public:
    Q_INVOKABLE QDateProperty(QAbstractProperty* parent=0);

    QString         get_value_text();
    QIcon           get_value_icon();
protected:
    void    make_value();
    void    write_value();
};
#endif // QDATEPROPERTY_H
