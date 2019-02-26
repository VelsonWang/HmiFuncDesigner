#include "propertytableview.h"

PropertyTableView::PropertyTableView(PropertyModel *model, QWidget *parent)
    : QTableView(parent)
{
    if (model) {
        setModel(model);
    }

    init();
}

void PropertyTableView::init() {

    resizeColumnsToContents();
    setColumnWidth(0,150);
    setAlternatingRowColors(true);
    setSelectionMode(QTableView::SingleSelection);
    setSelectionBehavior(QTableView::SelectRows);
    setEditTriggers(QAbstractItemView::CurrentChanged | QAbstractItemView::SelectedClicked);
    verticalHeader()->hide();
    verticalHeader()->setDefaultSectionSize(25);
    horizontalHeader()->setStretchLastSection(true);

    delegate = new PropertyDelegate(this);
    setItemDelegate(delegate);
}
