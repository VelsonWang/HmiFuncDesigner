#ifndef LISTPROPERTY_H
#define LISTPROPERTY_H

#include "property.h"

class ListProperty : public Property
{
public:
    ListProperty(const QString &name);

    virtual QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& options, const QAbstractItemDelegate* delegate);
    virtual QVariant getEditorData(QWidget* editor) const;
    virtual void setList(const QStringList &list);

protected:
    QStringList szList_;
};

#endif // LISTPROPERTY_H
