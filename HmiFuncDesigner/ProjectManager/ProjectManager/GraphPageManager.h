#ifndef GRAPHPAGEMANAGER_H
#define GRAPHPAGEMANAGER_H

#include <QObject>
#include "GraphPage.h"
#include <QGraphicsView>

class GraphPageManager : public QObject
{
    Q_OBJECT
public:
    static GraphPageManager *getInstance();
    void addGraphPage(GraphPage*);
    GraphPage *getGraphPageById(const QString &) const;
    int getIndexByGraphPage(GraphPage *);
    QList<GraphPage*> getGraphPageList() const;
    void removeGraphPage(GraphPage *graphPage);

private:
    GraphPageManager();
    static GraphPageManager *instance_;
    QList<GraphPage *> graphPageList_;
};

#endif // GRAPHPAGEMANAGER_H
