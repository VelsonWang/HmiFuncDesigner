#ifndef ENUMPROPERTY_H
#define ENUMPROPERTY_H

#include "property.h"
#include <QComboBox>

class EnumProperty : public Property
{
public:
    EnumProperty(const QString& name);

    virtual QVariant data (int column = ColumnProperty, int role = Qt::DisplayRole);

    virtual QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& options, const QAbstractItemDelegate* delegate);

    virtual QVariant getEditorData(QWidget* editor) const;

    virtual void setLiterals(const QStringList &literals);

    virtual QStringList getLiterals();

    virtual void setValue(const QVariant& value);

    virtual QVariant getValue() const;

protected:
    QStringList enumerationLiterals;
};

#endif // ENUMPROPERTY_H
