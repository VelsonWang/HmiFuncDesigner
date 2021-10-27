#ifndef RTDBWin_H
#define RTDBWin_H

#include "listviewex.h"
#include <QWidget>
#include <QStandardItemModel>
#include <QVBoxLayout>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QContextMenuEvent>
#include <QProcess>
#include <QScopedPointer>


class RTDBWin : public QWidget
{
    Q_OBJECT
public:
    explicit RTDBWin(QWidget *parent = NULL);
    ~RTDBWin();

public:
    bool event(QEvent *ev);

private:
    void ListViewInitUi();

private slots:
    void onSlotListViewProjectDoubleClicked(const QModelIndex &index);

private:
    ListViewEx *m_pListViewObj = NULL;
    QStandardItemModel *m_pListViewModelObj = NULL;
};

#endif // RTDBWin_H
