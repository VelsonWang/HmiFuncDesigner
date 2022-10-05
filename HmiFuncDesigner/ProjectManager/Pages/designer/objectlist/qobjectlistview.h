#ifndef QOBJECTLISTVIEW_H
#define QOBJECTLISTVIEW_H

#include <QTreeWidget>
#include <QUndoStack>

class QAbstractHost;

class QObjectListView : public QTreeWidget
{
    Q_OBJECT
public:
    explicit QObjectListView(QWidget *parent = NULL);
    void setUndoStack(QUndoStack* stack);

public slots:
    void set_select(QAbstractHost* host);
    void remove(QList<QAbstractHost*> hosts);

protected:
    void init();
    void clear();
    void drawRow(QPainter *painter, const QStyleOptionViewItem &options, const QModelIndex &index) const;

    QItemSelectionModel::SelectionFlags selectionCommand(const QModelIndex &index, const QEvent *event) const;

    QString get_host_icon(QAbstractHost* host);
    QModelIndex moveCursor(CursorAction cursorAction, Qt::KeyboardModifiers modifiers);

signals:
    void select(QAbstractHost* host);

protected slots:
    void onProjectOpened();
    void onProjectClosed();
    void item_clicked(QTreeWidgetItem *item);
    void select_changed(QTreeWidgetItem * current, QTreeWidgetItem * previous );

    void insert_host_slot(const QList<QAbstractHost*> &list, const QList<int> &indexs);
    void remove_host_slot(const QList<QAbstractHost*> &list);

    void insert_page_slot(QAbstractHost* page);
    void remove_page_slot(QAbstractHost *page);

    void host_parent_changed();

    void button_remove();

    void host_name_changed(QAbstractHost* host);

protected:
    friend class QObjectDelegate;

protected:
    QMap<QTreeWidgetItem*, QAbstractHost*> m_item_to_host;
    QMap<QAbstractHost*, QTreeWidgetItem*> m_host_to_item;
    QTreeWidgetItem *m_form_item;
    QTreeWidgetItem *m_keyboard_item;
    QTreeWidgetItem *m_dialog_item;
    QUndoStack *m_undo_stack;
};

#endif // QOBJECTLISTVIEW_H
