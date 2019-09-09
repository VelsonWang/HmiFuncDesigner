#include "ObjectsTreeView.h"
#include "Element.h"
#include <QtDebug>

ObjectsTreeView::ObjectsTreeView(QWidget *parent)
    : QTreeWidget(parent)
{
    setColumnCount(2);
    QStringList headers;

    headers << tr("对象") << tr("类型");
    setHeaderLabels(headers);

    setWindowFlags(Qt::Tool);
    setWindowTitle(tr("对象树"));

    connect(this, SIGNAL(itemClicked(QTreeWidgetItem*,int)), SLOT(slotItemChanged(QTreeWidgetItem*,int)));
}

void ObjectsTreeView::setContentList(const QList<QGraphicsItem *> &items) {

    contentList = items;
}

void ObjectsTreeView::updateContent() {

    clearModel();
    fillTreeModel();
}

void ObjectsTreeView::clearModel() {

    model()->removeRows(0, model()->rowCount(), QModelIndex());
}

void ObjectsTreeView::fillTreeModel() {

    foreach (QGraphicsItem *item, contentList) {

        Element *ele = dynamic_cast<Element *>(item);

        if (ele) {
            QTreeWidgetItem *treeItem = new QTreeWidgetItem(this);
            treeItem->setText(0, ele->getElementId());
            treeItem->setText(1, ele->getInternalElementType());
            treeItem->setIcon(0, ele->getElementIcon());
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

    foreach (QGraphicsItem *item, contentList) {
        item->setSelected(false);
    }

    contentList.at(indexFromItem(item).row())->setSelected(true);
}
