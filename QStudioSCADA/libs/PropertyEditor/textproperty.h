#ifndef TEXTPROPERTY_H
#define TEXTPROPERTY_H

#include "property.h"

class TextProperty : public Property
{
public:
    TextProperty(const QString &name);

    virtual QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& options, const QAbstractItemDelegate* delegate);
    virtual QVariant getEditorData(QWidget* editor) const;
    virtual QVariant data (int column = ColumnProperty, int role = Qt::DisplayRole);
    virtual Qt::ItemFlags flags(int column = ColumnProperty);
    void setReadOnly(bool readOnly);

protected:
    QString text;
    bool readOnly_;
};

#endif // TEXTPROPERTY_H
