#include "MdiArea.h"
#include <QList>
#include <QTabBar>


MdiArea::MdiArea(QWidget* parent)
    : QMdiArea(parent)
{
}

MdiArea::~MdiArea()
{
}

void MdiArea::setupMdiArea()
{
    QList<QTabBar*> tabBars = findChildren<QTabBar*>();
    if (tabBars.count() > 0) {
        tabBars[0]->setTabsClosable(true);
        QObject::connect(tabBars[0], SIGNAL(tabCloseRequested(int)), this, SIGNAL(tabCloseRequested(int)));
    }
}


