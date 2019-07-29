#ifndef FILEPROPERTY_H
#define FILEPROPERTY_H

#include "property.h"
#include "filepropertyeditor.h"

class FileProperty : public Property
{
public:
    FileProperty(const QString &pname);
    QVariant data(int column, int role);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &options, const QAbstractItemDelegate *delegate);
    bool setEditorData(QWidget *editor);
    QVariant getEditorData(QWidget *editor) const;
};

#endif // FILEPROPERTY_H
