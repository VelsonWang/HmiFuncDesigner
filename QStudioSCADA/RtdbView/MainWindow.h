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
#include <QTcpSocket>
#include <QHostAddress>
#include <QDataStream>
#include <QTimer>
#include "MdiChildWindow.h"
#include "Tag.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QString projPath, QWidget *parent = 0);
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
    QStandardItem *CreateTreeItem(const QString name, bool bconnect);

    QJsonObject LoadJsonObjectFromFile(SaveFormat saveFormat, QString f);
    bool Load(SaveFormat saveFormat);


protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void SetActiveSubWindow(MdiChildWindow *window);
    MdiChildWindow* GetActiveSubWindow();
    void on_TagTreeView_clicked(const QModelIndex &index);
    void on_actionConnect_triggered();
    void slotConnected();
    void slotDisconnected();
    void dataReceived();
    void on_actionClose_triggered();
    void timeout();
    void writeRtdbTag(QString cmdline);

private:
    QString onDataTransfer(QDataStream& in);
    void ReadParser(QString cmdline);
    void WriteParser(QString cmdline);

private:
    int port;
    QHostAddress *serverIP;
    QTcpSocket *tcpSocket;
    bool bConnectStatus;
    QTimer *pTimer;

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
};

#endif // MAINWINDOW_H
