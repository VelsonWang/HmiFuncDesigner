#ifndef PROPERTYDELEGATE_H
#define PROPERTYDELEGATE_H

#include <QStyledItemDelegate>

class PropertyDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit PropertyDelegate(QObject *parent = 0);

    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

protected:
    int rowHeight;
    bool addRowHeight;
};

#endif // PROPERTYDELEGATE_H
