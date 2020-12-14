#ifndef QOBJECTLISTWIDGET_H
#define QOBJECTLISTWIDGET_H

#include <QWidget>
#include <QUndoStack>

class StyledBar;
class QObjectListView;
class QAbstractHost;

class QObjectListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QObjectListWidget(QWidget *parent = 0);
    void    set_undo_stack(QUndoStack *stack);
public slots:
    void    set_select(QAbstractHost* host);
signals:
    void    select(QAbstractHost* host);
public slots:
protected:
    StyledBar *m_styledBar;
    QObjectListView *m_object_list;
};

#endif // QOBJECTLISTWIDGET_H
