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
    explicit RTDBWin(QWidget *parent = Q_NULLPTR);
    ~RTDBWin();

public:
    bool eventFilter(QObject *obj, QEvent *ev);

private:
    void ListViewInitUi();

private slots:
    void onSlotListViewProjectDoubleClicked(const QModelIndex &index);

private:
    ListViewEx *m_pListViewObj = Q_NULLPTR;
    QStandardItemModel *m_pListViewModelObj = Q_NULLPTR;
};

#endif // RTDBWin_H
