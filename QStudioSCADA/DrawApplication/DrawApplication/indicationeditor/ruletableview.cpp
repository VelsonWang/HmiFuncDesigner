#include "ruletableview.h"
#include <QHeaderView>

RuleTableView::RuleTableView(RuleTableModel *model, QWidget *parent)
    : QTableView(parent)
{
    if (model) {
        setModel(model);
    }

    init();
}

void RuleTableView::init() {

    setColumnWidth(0,150);
    setSelectionMode(QTableView::SingleSelection);
    setEditTriggers(QAbstractItemView::CurrentChanged | QAbstractItemView::SelectedClicked);
    setAlternatingRowColors(true);
    verticalHeader()->hide();
    verticalHeader()->setDefaultSectionSize(30);
    horizontalHeader()->setStretchLastSection(true);

    ruleDelegate = new RuleTableDelegate(this);
    setItemDelegateForColumn(1,ruleDelegate);
    setItemDelegateForColumn(3,ruleDelegate);
    setItemDelegateForColumn(4,ruleDelegate);
}
