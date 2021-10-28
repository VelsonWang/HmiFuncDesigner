#ifndef QBYTEARRAYPROPERTY_H
#define QBYTEARRAYPROPERTY_H

#include "qabstractproperty.h"

class QByteArrayProperty : public QAbstractProperty
{
    Q_OBJECT
public:
    Q_INVOKABLE QByteArrayProperty(QAbstractProperty* parent = 0);

    QString getValueText();
    QIcon getValueIcon();
};

#endif // QBYTEARRAYPROPERTY_H

