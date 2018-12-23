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
    QString userFriendlyCurrentFile() const; // 提取文件名
    QString currentFile() const;


public:
    // 打开文件
    void open();
    // 保存文件
    void save();
    // 显示大图标
    void ShowLargeIcon();
    // 显示小图标
    void ShowSmallIcon();


private slots:
    void on_listViewProject_doubleClicked(const QModelIndex &index);

private:
    void setCurrentFile(const QString &fileName);
    QString curFile;
    bool isUntitled;    // saved flag

private:
    Ui::SystemParametersWin *ui;
    QStandardItemModel *pListViewProjectModel;

};

#endif // SYSTEMPARAMETERSWIN_H
