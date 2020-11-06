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


/**
 * @brief GraphPageManager::getAllElementIDName
 * @details 获取工程所有控件的ID名称
 * @param szIDList
 */
void GraphPageManager::getAllElementIDName(QStringList &szIDList) {
    QListIterator <GraphPage*> iter(graphPageList_);
    GraphPage *graphPage = Q_NULLPTR;
    QString szPageId = "";
    while (iter.hasNext()) {
        graphPage = iter.next();
        szPageId = graphPage->getGraphPageId();
        QString szID = "";
        QListIterator <QGraphicsItem*> it(graphPage->items());
        while (it.hasNext()) {
            Element *pEleObj = static_cast<Element *>(it.next());
            szID = pEleObj->getElementId();
            szIDList.append(szPageId + "." + szID);
        }
    }
}


/**
 * @brief GraphPageManager::getAllGraphPageName
 * @details 获取工程所有画面名称
 * @param szList 所有画面名称
 */
void GraphPageManager::getAllGraphPageName(QStringList &szList) {
    QListIterator <GraphPage*> iter(graphPageList_);
    GraphPage *graphPage = Q_NULLPTR;
    while (iter.hasNext()) {
        graphPage = iter.next();
        szList.append(graphPage->getGraphPageId());
    }
}


