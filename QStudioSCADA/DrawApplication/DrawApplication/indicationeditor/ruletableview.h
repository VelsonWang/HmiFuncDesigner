#ifndef RULETABLEVIEW_H
#define RULETABLEVIEW_H

#include <QTableView>
#include "ruletablemodel.h"
#include "ruletabledelegate.h"

class RuleTableView : public QTableView
{
public:
    RuleTableView(RuleTableModel *model,QWidget *parent = 0);

private:
    void init();
    RuleTableDelegate *ruleDelegate;
};

#endif // RULETABLEVIEW_H
