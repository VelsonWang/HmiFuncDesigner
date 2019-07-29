#ifndef COLORPROPERTY_H
#define COLORPROPERTY_H

#include "property.h"
#include "colorpropertyeditor.h"

class ColorProperty : public Property
{
public:
    ColorProperty(const QString &pname);
    QVariant data(int column, int role);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &options, const QAbstractItemDelegate *delegate);
    bool setEditorData(QWidget *editor);
    QVariant getEditorData(QWidget *editor) const;
};

#endif // COLORPROPERTY_H
