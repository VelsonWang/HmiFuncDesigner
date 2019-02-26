#ifndef RULETABLEDELEGATE_H
#define RULETABLEDELEGATE_H

#include <QItemDelegate>

class RuleTableDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    RuleTableDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // RULETABLEDELEGATE_H
