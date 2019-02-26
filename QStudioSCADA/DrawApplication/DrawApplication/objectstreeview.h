#ifndef OBJECTSTREEVIEW_H
#define OBJECTSTREEVIEW_H

#include <QTreeWidget>
#include <QGraphicsItem>
#include "graphicselements/figure.h"

class ObjectsTreeView : public QTreeWidget
{
    Q_OBJECT

public:
    ObjectsTreeView(QWidget *parent = 0);

    void setContentList(const QList<QGraphicsItem*> &items);
    void updateContent();
    void clearModel();
    void fillTreeModel();

protected:
    void contextMenuEvent(QContextMenuEvent *);

private:
    QList <QGraphicsItem*> contentList;

private slots:
    void slotItemChanged(QTreeWidgetItem*,int);
};

#endif // OBJECTSTREEVIEW_H
