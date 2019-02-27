#ifndef EMPTYPROPERTY_H
#define EMPTYPROPERTY_H

#include "property.h"

class EmptyProperty : public Property
{
public:
    explicit EmptyProperty(const QString& name);

    virtual QVariant data (int column = ColumnProperty, int role = Qt::DisplayRole);
    virtual QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& options, const QAbstractItemDelegate* delegate);
    virtual QVariant getEditorData(QWidget* editor) const;
    virtual Qt::ItemFlags flags(int column = ColumnProperty);
};

#endif // EMPTYPROPERTY_H
