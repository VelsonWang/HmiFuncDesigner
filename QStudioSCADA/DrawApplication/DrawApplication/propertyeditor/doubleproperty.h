#ifndef DOUBLEPROPERTY_H
#define DOUBLEPROPERTY_H

#include "property.h"

class DoubleProperty : public Property
{
public:
    DoubleProperty(const QString& name);

    virtual QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& options, const QAbstractItemDelegate* delegate);

    virtual QVariant getEditorData(QWidget* editor) const;

    virtual void setSettings(double minimum, double maxiumum, int precision);

protected:
    int precision;
    double min, max;
    const static double standardPrecision;
};

#endif // DOUBLEPROPERTY_H
