#include "objectstreeview.h"
#include <QtDebug>

ObjectsTreeView::ObjectsTreeView(QWidget *parent)
    : QTreeWidget(parent)
{
    setColumnCount(2);
    QStringList headers;

    headers << trUtf8("对象") << trUtf8("类型");
    setHeaderLabels(headers);

    setWindowFlags(Qt::Tool);
    setWindowTitle(trUtf8("对象树"));

    connect(this,SIGNAL(itemClicked(QTreeWidgetItem*,int)),SLOT(slotItemChanged(QTreeWidgetItem*,int)));
}

void ObjectsTreeView::setContentList(const QList<QGraphicsItem *> &items) {

    contentList = items;
}

void ObjectsTreeView::updateContent() {

    clearModel();
    fillTreeModel();
}

void ObjectsTreeView::clearModel() {

    model()->removeRows(0,model()->rowCount(),QModelIndex());
}

void ObjectsTreeView::fillTreeModel() {

    foreach (QGraphicsItem *item, contentList) {

        Figure *figure = dynamic_cast<Figure*>(item);

        if (figure) {
            QTreeWidgetItem *treeItem = new QTreeWidgetItem(this);
            treeItem->setText(0,figure->getElementId());
            treeItem->setText(1,figure->getInternalElementType());
            treeItem->setIcon(0,figure->getElementIcon());
        }
    }
}

void ObjectsTreeView::contextMenuEvent(QContextMenuEvent *event) {

    Q_UNUSED(event)
}

void ObjectsTreeView::slotItemChanged(QTreeWidgetItem *item, int column) {

    if (column == 1) {
        return;
    }

    foreach (QGraphicsItem *item,contentList) {
        item->setSelected(false);
    }

    contentList.at(indexFromItem(item).row())->setSelected(true);
}
