#ifndef REALTIMEDATABASEWIN_H
#define REALTIMEDATABASEWIN_H

#include <QWidget>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QContextMenuEvent>
#include "ChildBase.h"

namespace Ui {
class RealTimeDatabaseWin;
}

class RealTimeDatabaseWin : public ChildBase
{
    Q_OBJECT

public:
    explicit RealTimeDatabaseWin(QWidget *parent = 0, QString itemName = "", QString projName = "");
    ~RealTimeDatabaseWin();

public:
    // 打开文件
    void open();
    // 保存文件
    void save();
    // 显示大图标
    void ShowLargeIcon();
    // 显示小图标
    void ShowSmallIcon();

private:
    void ListViewInitUi();

private slots:
    void on_listView_clicked(const QModelIndex &index);

    void on_listView_doubleClicked(const QModelIndex &index);

private:
    Ui::RealTimeDatabaseWin *ui;
    QStandardItemModel *pListViewModel;
    QString m_ProjPath;
};

#endif // REALTIMEDATABASEWIN_H
