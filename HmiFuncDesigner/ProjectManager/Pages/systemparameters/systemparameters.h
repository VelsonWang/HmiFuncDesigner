#ifndef SYSTEMPARAMETERS_H
#define SYSTEMPARAMETERS_H

#include "listviewex.h"
#include <QWidget>
#include <QStandardItemModel>
#include <QVBoxLayout>
#include <QListView>

class SystemParameters : public QWidget
{
    Q_OBJECT
public:
    explicit SystemParameters(QWidget *parent = NULL);
    ~SystemParameters();

private slots:
    void onSlotListViewProjectDoubleClicked(const QModelIndex &index);

private:
    QStandardItemModel *m_pListViewProjectModelObj = NULL;
    ListViewEx *m_pListViewProjectObj = NULL;
};

#endif
