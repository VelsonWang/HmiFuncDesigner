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
    void setTrueText(const QString &text);
    void setFalseText(const QString &text);

protected:
    QVariant trueText;
    QVariant falseText;
};

#endif // BOOLPROPERTY_H
