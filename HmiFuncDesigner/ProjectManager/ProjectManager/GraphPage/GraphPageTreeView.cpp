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
    QList<GraphPage*>* pGraphPageListObj = GraphPageManager::getInstance()->getGraphPageList();
    for(int i=0; i<pGraphPageListObj->count(); i++) {
        GraphPage* pObj = pGraphPageListObj->at(i);
        addNewElement(pObj->getGraphPageId());
    }
}
