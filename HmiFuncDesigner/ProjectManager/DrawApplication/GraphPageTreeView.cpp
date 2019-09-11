#include "GraphPageTreeView.h"
#include "GraphPage.h"
#include "GraphPageManager.h"

GraphPageTreeView::GraphPageTreeView(QWidget *parent)
    : QListWidget(parent)
{
    setIconSize(QSize(32, 32));
    itemIcon = QIcon(":/images/GraphPage.png");
}

void GraphPageTreeView::addNewElement(const QString &id) {

    QListWidgetItem *item = new QListWidgetItem(itemIcon, id);
    addItem(item);
}

void GraphPageTreeView::graphPageChangeName(int index, const QString &name) {

    if (item(index)) {
        item(index)->setText(name);
    }
}

void GraphPageTreeView::updateView() {

    clear();

    QListIterator <GraphPage*> it(GraphPageManager::getInstance()->getGraphPageList());

    while (it.hasNext()) {
        addNewElement(it.next()->getGraphPageId());
    }
}
