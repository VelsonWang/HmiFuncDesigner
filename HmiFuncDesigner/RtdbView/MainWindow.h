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
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QMutex>
#include "MdiChildWindow.h"
#include "Tag.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QString projPath, QWidget *parent = nullptr);
    ~MainWindow();
    void ShowFirstPage();

private:
    MdiChildWindow* ActiveMdiChild();
    void CreateItemWindows();
    MdiChildWindow* FindMdiChild(const QString &windowTitle);
    QMdiSubWindow* FindMdiSubWindow(const QString &windowTitle);
    void ReadSettings();  // 读取窗口设置
    void WriteSettings(); // 写入窗口设置
    void InitWindow(); // 初始化窗口
    void InitTreeViewUi();
    QStandardItem *CreateTreeItem(const QString name);
    bool Load();
    QString getProjectName(const QString &szProjectPath);

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void finished(QNetworkReply *reply);
    void SetActiveSubWindow(MdiChildWindow *window);
    MdiChildWindow* GetActiveSubWindow();
    void on_TagTreeView_clicked(const QModelIndex &index);
    void on_actionConnect_triggered();
    void on_actionClose_triggered();
    void timeout();
    void writeRtdbTag(QString cmdline);

private:
    int port_;
    QString ip_;
    QString url_;
    bool bConnectStatus_;
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

    QNetworkAccessManager *m_networkAccessManager;
    QNetworkReply *m_reply;
    QMutex mutex_;
};

#endif // MAINWINDOW_H
