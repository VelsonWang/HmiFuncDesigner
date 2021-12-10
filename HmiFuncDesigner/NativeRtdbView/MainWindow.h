#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMdiSubWindow>
#include <QCloseEvent>
#include <QContextMenuEvent>
#include <QString>
#include <QList>
#include <QStringList>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QModelIndex>
#include <QTimer>
#include <QMutex>
#include <QTableView>
#include "DBTag.h"
#include "RtdbTag.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QString projPath, QWidget *parent = Q_NULLPTR);
    ~MainWindow();
    void ShowFirstPage();
    void SetTagLogicValueAndStatus(QString id,
                                   QString logicValue,
                                   QString status);
private:
    void CreateItemWindows();
    void ReadSettings();  // 读取窗口设置
    void WriteSettings(); // 写入窗口设置
    void InitWindow(); // 初始化窗口
    void InitTreeViewUi();
    QStandardItem *CreateTreeItem(const QString name);
    bool Load();
    QString getProjectName(const QString &szProjectPath);
    void setTableView(QList<TagItem *> &tagList, const QString &szTittle);

private slots:
    void on_TagTreeView_clicked(const QModelIndex &index);
    void timeout();
    void onWriteRtdbTag();

protected:
    void closeEvent(QCloseEvent *event);
    void contextMenuEvent(QContextMenuEvent * event);

private:
    QTimer *timer_;

private:
    Ui::MainWindow *ui;
    QString mProjectPath;
    QStandardItemModel *pTagTreeViewModel;
    QStandardItem *pTreeItemRoot;
    QStandardItem *pTreeIoItem;
    QStandardItem *pTreeTmpItem;
    QStandardItem *pTreeSysItem;

    QMap<QString, QList<TagItem *>> mMapIoTagList;
    QStringList mIoTagTableNameList;
    QList<TagItem *> mTmpTagList;
    QList<TagItem *> mSysTagList;
    QMap<QString, qint32> mMapIoTagPageGroupId;
    QMap<QString, qint32> mMapIoTagPageId;
    QList<TagItem *> mCurTagList;

private:
    TagTableModel *pTagTableModel;
    QTableView *pTableView;
};

#endif // MAINWINDOW_H
