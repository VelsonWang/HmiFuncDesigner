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
    explicit QObjectListWidget(QWidget *parent = NULL);
    void setUndoStack(QUndoStack *stack);

public slots:
    void set_select(QAbstractHost* host);
    void remove(QList<QAbstractHost*> hosts);

signals:
    void select(QAbstractHost* host);


protected:
    StyledBar *m_styledBar;
    QObjectListView *m_object_list;
};

#endif // QOBJECTLISTWIDGET_H
