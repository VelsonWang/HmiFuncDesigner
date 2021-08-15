#ifndef GRAPHPAGEMANAGER_H
#define GRAPHPAGEMANAGER_H

#include <QObject>
#include "GraphPage.h"

class GraphPageManager : public QObject
{
    Q_OBJECT
public:
    static GraphPageManager *getInstance();
    static void releaseInstance();
    void addGraphPage(GraphPage*);
    GraphPage *getGraphPageById(const QString &) const;
    int getIndexByGraphPage(GraphPage *);
    QList<GraphPage*> getGraphPageList() const;
    void removeGraphPage(GraphPage *graphPage);

private:
    GraphPageManager();
    static GraphPageManager *instance_;
    static QList<GraphPage *> graphPageList_;
};

#endif // GRAPHPAGEMANAGER_H
