#ifndef PROPERTYTABLEVIEW_H
#define PROPERTYTABLEVIEW_H

#include <QTableView>
#include <QHeaderView>
#include "propertymodel.h"
#include "propertydelegate.h"

class PropertyTableView : public QTableView
{
public:
    PropertyTableView(PropertyModel *model,QWidget *parent = 0);

    void init();

private:
    PropertyDelegate *delegate;
};

#endif // PROPERTYTABLEVIEW_H
