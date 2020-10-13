#ifndef TAGMANAGERWIN_H
#define TAGMANAGERWIN_H

#include "ChildInterface.h"
#include "ListViewEx.h"
#include "TagFuncEditDialog.h"
#include "../../Public/Public.h"
#include "ProjectData.h"
#include <QList>
#include <QString>
#include <QWidget>
#include <QStandardItemModel>
#include <QTableWidgetItem>
#include <QStringList>
#include <QVBoxLayout>
#include <QComboBox>
#include <QMap>
#include <QStackedWidget>
#include <QTableWidget>
#include <QDir>
#include <QFile>
#include <QPluginLoader>
#include <QMessageBox>
#include <QToolBar>

//////////////////////////////////////////////////////////


struct TagTmpDBItem;
struct TagIODBItem;

class TagManagerChild : public QWidget, public ChildInterface
{
    Q_OBJECT
    Q_INTERFACES(ChildInterface)
public:
    explicit TagManagerChild(QWidget *parent = Q_NULLPTR);
    ~TagManagerChild();

public:
    void buildUserInterface(QMainWindow* pMainWin);
    void removeUserInterface(QMainWindow* pMainWin);
    QString wndTitle() const;

public:
    void exportToCsv(const QString &path);
    void importFromCsv(const QString &path);

private:
    bool save();

private:
    void initUi();
    // 初始化系统变量表
    void initialTableTagSys();
    // 刷新系统变量表
    void updateTableTagSys();


    // 初始化中间变量表
    void initialTableTagTmp();
    // 中间变量表新增一行
    int tableTagTmpAddRow();
    // 获取指定行中间变量id数值部分
    int getTagTmpIdNumValue(int iRow);
    // 刷新中间变量表
    void updateTableTagTmp();
    // 保存中间变量表
    void saveTableTagTmp();
    // 中间变量表变量导出至CSV文件
    void tagTmpExportToCsv(const QString &path, const QString & /*group*/);
    // 从CSV文件导入中间变量至中间变量表
    void tagTmpImportFromCsv(const QString &path);
    // 创建中间变量
    void createTagTmp();
    // 追加中间变量
    void appendTagTmp();
    // 获取指定行中间变量对象
    TagTmpDBItem *getTagTmpObjByRow(int iRow);
    // 设置指定行中间变量对象
    void setTagTmpObjByRow(int iRow, TagTmpDBItem *pObj);
    // 拷贝选中中间变量
    void copyCurTagTmp();
    // 修改选中中间变量
    void modifyCurTagTmp();
    // 删除选中中间变量
    void deleteCurTagTmp();


    // 初始化设备变量表
    void initialTableTagIO();
    // 设备变量表新增一行
    int tableTagIOAddRow();
    // 获取指定行设备变量id数值部分
    int getTagIOIdNumValue(int iRow);
    // 刷新设备变量表
    void updateTableTagIO();
    // 保存设备变量表
    void saveTableTagIO();
    // 设备变量表变量导出至CSV文件
    void tagIOExportToCsv(const QString &path, const QString & /*group*/);
    // 从CSV文件导入设备变量至设备变量表
    void tagIOImportFromCsv(const QString &path);
    // 获取指定行设备变量对象
    TagIODBItem *getTagIOObjByRow(int iRow);
    // 设置指定行设备变量对象
    void setTagIOObjByRow(int iRow, TagIODBItem *pObj);
    // 创建设备变量
    void createTagIO();
    // 追加设备变量
    void appendTagIO();
    // 拷贝选中设备变量
    void copyCurTagIO();
    // 修改选中设备变量
    void modifyCurTagIO();
    // 删除选中设备变量
    void deleteCurTagIO();

protected:
    void contextMenuEvent(QContextMenuEvent * event);
    void closeEvent(QCloseEvent *event);  // 关闭事件

public slots:
    // 添加变量
    void onSlotAddTag();
    // 追加变量
    void onSlotAppendTag();
    // 拷贝变量
    void onSlotRowCopyTag();
    // 修改变量
    void onSlotModifyTag();
    // 删除变量
    void onSlotDeleteTag();
    // 导出变量
    void onSlotExportTag();
    // 导入变量
    void onSlotImportTag();

public:
    QString m_szCurIOGroupName;

private slots:
    void on_tableTagIO_itemPressed(QTableWidgetItem *item);
    void on_tableTagTmp_itemPressed(QTableWidgetItem *item);
    void on_tableTagTmp_itemDoubleClicked(QTableWidgetItem *item);
    void on_tableTagIO_itemDoubleClicked(QTableWidgetItem *item);
    void on_tableTagIO_cellClicked(int row, int column);
    void on_tableTagIO_cellDoubleClicked(int row, int column);
    void on_tableTagTmp_cellClicked(int row, int column);
    void on_tableTagTmp_cellDoubleClicked(int row, int column);

private:
    int m_iTableTagTmpSelectedRow = -1;
    int m_iTableTagIOSelectedRow = -1;
    QStringList m_listTagTmpDeleteRows;
    QStringList m_listTagIODeleteRows;
    QMainWindow* m_pMainWinObj = Q_NULLPTR;

    QMenu *m_pMenuTagEditObj = Q_NULLPTR; // 变量编辑菜单
    QToolBar *m_pToolBarTagEditObj = Q_NULLPTR; // 变量编辑工具条
    QAction *m_pActAddTagObj = Q_NULLPTR; // 添加变量
    QAction *m_pActAppendTagObj = Q_NULLPTR; // 追加变量
    QAction *m_pActRowCopyTagObj = Q_NULLPTR; // 拷贝变量
    QAction *m_pActModifyTagObj = Q_NULLPTR; // 修改变量
    QAction *m_pActDeleteTagObj = Q_NULLPTR; // 删除变量
    QAction *m_pActExportTagObj = Q_NULLPTR; // 导出变量
    QAction *m_pActImportTagObj = Q_NULLPTR; // 导入变量

public:
    QVBoxLayout *m_pTopVLayoutObj;
    QStackedWidget *m_pStackedWidgetObj;
    QTableWidget *m_pTableTagIOObj;
    QTableWidget *m_pTableTagTmpObj;
    QTableWidget *m_pTableTagSysObj;
};

#endif // TAGMANAGERWIN_H
