#include "GraphPageManager.h"

GraphPageManager *GraphPageManager::instance_ = 0;

GraphPageManager::GraphPageManager()
{
}

GraphPageManager *GraphPageManager::getInstance() {

    if (!instance_) {
        instance_ = new GraphPageManager;
    }

    return instance_;
}

void GraphPageManager::addGraphPage(GraphPage *GraphPage) {

    graphPageList.insert(graphPageList.end(),GraphPage);
}

void GraphPageManager::removeGraphPage(GraphPage *GraphPage) {

    graphPageList.removeAt(graphPageList.indexOf(GraphPage));
}

GraphPage *GraphPageManager::getGraphPageById(const QString &id) const {

    QListIterator <GraphPage*> i(graphPageList);
    GraphPage *graphPage;

    while (i.hasNext()) {

        graphPage = i.next();

        if (graphPage->getGraphPageId().contains(id, Qt::CaseInsensitive)) {
            return graphPage;
        }
    }

    return 0;
}

int GraphPageManager::getIndexByGraphPage(GraphPage *graphPage) {

    return graphPageList.indexOf(graphPage);
}

QList <GraphPage*> GraphPageManager::getGraphPageList() const {
    return graphPageList;
}
