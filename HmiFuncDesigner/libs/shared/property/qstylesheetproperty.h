#ifndef QSTYLESHEETPROPERTY_H
#define QSTYLESHEETPROPERTY_H

#include "qabstractproperty.h"

class QStylesheetProperty : public QAbstractProperty
{
    Q_OBJECT
public:
    Q_INVOKABLE QStylesheetProperty(QAbstractProperty* parent = 0);

    QString getValueText();
    QIcon getValueIcon();

    void fromObject(XMLObject *xml);
    void toObject(XMLObject *xml);
};

#endif // QSTYLESHEETPROPERTY_H

