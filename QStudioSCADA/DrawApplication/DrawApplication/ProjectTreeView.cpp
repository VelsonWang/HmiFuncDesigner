#include "ProjectTreeView.h"
#include "GraphPage.h"
#include "GraphPageManager.h"

ProjectTreeView::ProjectTreeView(QWidget *parent)
    : QListWidget(parent)
{
    setIconSize(QSize(32,32));
    itemIcon = QIcon(":/images/screen.png");
}

void ProjectTreeView::addNewElement(const QString &id) {

    QListWidgetItem *item = new QListWidgetItem(itemIcon,id);
    addItem(item);
}

void ProjectTreeView::graphPageChangeName(int index, const QString &name) {

    if (item(index)) {
        item(index)->setText(name);
    }
}

void ProjectTreeView::updateView() {

    clear();

    QListIterator <GraphPage*> it(GraphPageManager::getInstance()->getGraphPageList());

    while (it.hasNext()) {
        addNewElement(it.next()->getGraphPageId());
    }
}
