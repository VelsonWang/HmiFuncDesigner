#ifndef REALTIMEDATABASEWIN_H
#define REALTIMEDATABASEWIN_H

#include <QWidget>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QContextMenuEvent>
#include <QProcess>
#include <QScopedPointer>
#include "ChildBase.h"

namespace Ui {
class RealTimeDatabaseWin;
}

class RealTimeDatabaseWin : public ChildBase
{
    Q_OBJECT

public:
    explicit RealTimeDatabaseWin(QWidget *parent = 0,
                                 const QString &itemName = "",
                                 const QString &projName = "");
    ~RealTimeDatabaseWin();
    void init();

public:
    // 打开文件
    void open();
    // 保存文件
    void save();
    // 显示大图标
    void showLargeIcon();
    // 显示小图标
    void showSmallIcon();

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
