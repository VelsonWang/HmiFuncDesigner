#include "GraphPageManager.h"


GraphPageManager::GraphPageManager()
{
}

void GraphPageManager::releaseAllGraphPage()
{
    qDeleteAll(graphPageList_);
    graphPageList_.clear();
}

GraphPageManager *GraphPageManager::getInstance()
{
    static GraphPageManager instance;
    return &instance;
}

void GraphPageManager::addGraphPage(GraphPage *graphPage)
{
    graphPageList_.append(graphPage);
}

void GraphPageManager::removeGraphPage(GraphPage *graphPage)
{
    graphPageList_.removeAt(graphPageList_.indexOf(graphPage));
}

GraphPage *GraphPageManager::getGraphPageById(const QString &id) const
{
    QListIterator <GraphPage*> iter(graphPageList_);
    GraphPage *graphPage;

    while (iter.hasNext()) {
        graphPage = iter.next();
        if (graphPage->getGraphPageId().contains(id, Qt::CaseInsensitive)) {
            return graphPage;
        }
    }

    return Q_NULLPTR;
}

int GraphPageManager::getIndexByGraphPage(GraphPage *graphPage)
{
    return graphPageList_.indexOf(graphPage);
}

QList<GraphPage*> *GraphPageManager::getGraphPageList()
{
    return &graphPageList_;
}

