#ifndef TAGTEXTLISTPROPERTY_H
#define TAGTEXTLISTPROPERTY_H

#include "property.h"
#include "tagtextlistpropertyeditor.h"

class TagTextListProperty : public Property
{
public:
    TagTextListProperty(const QString &pname);
    QVariant data(int column, int role);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &options, const QAbstractItemDelegate *delegate);
    bool setEditorData(QWidget *editor);
    QVariant getEditorData(QWidget *editor) const;
};

#endif // TAGTEXTLISTPROPERTY_H
