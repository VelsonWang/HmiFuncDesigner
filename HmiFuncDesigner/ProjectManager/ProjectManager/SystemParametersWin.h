#ifndef SYSTEMPARAMETERSWIN_H
#define SYSTEMPARAMETERSWIN_H

#include "ChildBase.h"
#include <QWidget>
#include <QStandardItemModel>

namespace Ui {
class SystemParametersWin;
}

class SystemParametersWin : public ChildBase
{
    Q_OBJECT
public:
    explicit SystemParametersWin(QWidget *parent = 0,
                                 const QString &itemName = "",
                                 const QString &projName = "");
    ~SystemParametersWin();

public:
    // 打开文件
    void open();
    // 保存文件
    void save();

private slots:
    void on_listViewProject_doubleClicked(const QModelIndex &index);

private:
    Ui::SystemParametersWin *ui;
    QStandardItemModel *pListViewProjectModel;

};

#endif // SYSTEMPARAMETERSWIN_H
