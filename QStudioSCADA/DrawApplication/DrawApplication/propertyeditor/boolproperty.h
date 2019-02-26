#ifndef BOOLPROPERTY_H
#define BOOLPROPERTY_H

#include "property.h"

class BoolProperty : public Property
{
public:
    BoolProperty(const QString &name);
    virtual QVariant data (int column = ColumnProperty, int role = Qt::DisplayRole);
    virtual bool setData (const QVariant& data, int role = Qt::EditRole);
    virtual Qt::ItemFlags flags(int column = ColumnProperty);

protected:
    static QVariant trueText;
    static QVariant falseText;
};

#endif // BOOLPROPERTY_H
