#ifndef FUNCTIONPROPERTY_H
#define FUNCTIONPROPERTY_H

#include "property.h"
#include "functionpropertyeditor.h"

class FunctionProperty : public Property
{
public:
    FunctionProperty(const QString &pname);
    QVariant data(int column, int role);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &options, const QAbstractItemDelegate *delegate);
    bool setEditorData(QWidget *editor);
    QVariant getEditorData(QWidget *editor) const;

    void setSupportEvents(QStringList events);

private:
    QStringList supportEvents_;
};

#endif // FUNCTIONPROPERTY_H
