#ifndef DRAWPAGEWIN_H
#define DRAWPAGEWIN_H

#include "ChildBase.h"

#include <QStringList>
#include <QWidget>
#include <QStandardItemModel>
#include <QFileSystemWatcher>

namespace Ui {
class DrawPageWin;
}

class DrawPageWin : public ChildBase
{
    Q_OBJECT

public:
    explicit DrawPageWin(QWidget *parent = nullptr,
                         const QString &itemName = "",
                         const QString &projName = "");
    ~DrawPageWin();
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
    void ListViewUISetting();
    void ListViewUpdate();
    // 创建空的画面页
    void createEmptyGraphpage(const QString &projPath,
                              const QString &graphPageName,
                              int width,
                              int height);


protected:
    void contextMenuEvent(QContextMenuEvent * event);
    void closeEvent(QCloseEvent *event);


private slots:
    void NewDrawPage();
    void ModifyDrawPage();
    void DeleteDrawPage();
    void CopyDrawPage();
    void PasteDrawPage();
    void on_listViewDrawPage_doubleClicked(const QModelIndex &index);
    void onFileChanged(const QString &path);

private:
    Ui::DrawPageWin *ui;
    QStandardItemModel *pListDrawPageModel;
    QString szProjPath_;
    QString szProjName_;
    QString szCopyDrawPageFileName_;
    QFileSystemWatcher fileSystemWatcher_;
};

#endif // DRAWPAGEWIN_H
