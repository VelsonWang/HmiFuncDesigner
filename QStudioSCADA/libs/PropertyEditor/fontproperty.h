#ifndef FONTPROPERTY_H
#define FONTPROPERTY_H

#include "property.h"
#include "fontpropertyeditor.h"

class FontProperty : public Property
{
public:
    FontProperty(const QString &pname);
    QVariant data(int column, int role);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &options, const QAbstractItemDelegate *delegate);
    bool setEditorData(QWidget *editor);
    QVariant getEditorData(QWidget *editor) const;
};

#endif // FONTPROPERTY_H
