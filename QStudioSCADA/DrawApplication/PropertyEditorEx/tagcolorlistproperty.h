#ifndef TAGCOLORLISTPROPERTY_H
#define TAGCOLORLISTPROPERTY_H

#include "property.h"
#include "tagcolorlistpropertyeditor.h"

class TagColorListProperty : public Property
{
public:
    TagColorListProperty(const QString &pname);
    QVariant data(int column, int role);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &options, const QAbstractItemDelegate *delegate);
    bool setEditorData(QWidget *editor);
    QVariant getEditorData(QWidget *editor) const;
};

#endif // TAGCOLORLISTPROPERTY_H
