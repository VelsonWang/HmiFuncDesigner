#ifndef INTEGERPROPERTY_H
#define INTEGERPROPERTY_H

#include "property.h"

class IntegerProperty : public Property
{
public:
    IntegerProperty(const QString& name);

    virtual QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& options, const QAbstractItemDelegate* delegate);
    virtual QVariant getEditorData(QWidget* editor) const;
    virtual void setSettings(int minimum, int maximum);

protected:
    int min, max;
    static const int standardMin;
    static const int standardMax;
};

#endif // INTEGERPROPERTY_H
