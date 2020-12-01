#ifndef TAGMANAGERWIN_H
#define TAGMANAGERWIN_H

#include "ChildInterface.h"
#include "ListViewEx.h"
#include "TagFuncEditDialog.h"
#include "../../Public/Public.h"
#include <QString>
#include <QWidget>
#include <QTableWidgetItem>
#include <QStringList>
#include <QVBoxLayout>
#include <QMap>
#include <QTableWidget>
#include <QToolBar>
#include <QStringList>
#include <QAbstractTableModel>
#include <QVector>
#include "Tag.h"

class TagTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum Column {
        TagID = 0,
        Name = 1,
        Addr = 2,
        DataType = 3,
        ReadWrite = 4,
        Unit = 5,
        Remark = 6,
        // ...

        MaxColumns = 7
    };

public:
    explicit TagTableModel(QObject *parent=nullptr) : QAbstractTableModel(parent) {}

    Qt::ItemFlags flags(const QModelIndex &index) const;

    QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role=Qt::DisplayRole) const;

    int rowCount(const QModelIndex &parent=QModelIndex()) const;
    int columnCount(const QModelIndex &parent=QModelIndex()) const;

    bool setData(const QModelIndex &index, const QVariant &value, int role=Qt::EditRole);
    bool setHeaderData(int, Qt::Orientation, const QVariant&, int=Qt::EditRole) { return false; }

    bool insertRows(int row, int count, const QModelIndex &parent=QModelIndex());
    bool removeRows(int row, int count, const QModelIndex &parent=QModelIndex());

public:
    void AppendRow(QStringList rowDat);
    void AppendRows(QVector<QStringList> rowsDat);
    void InsertRow(int i, QStringList rowDat);
    QStringList GetRow(int i);
    void UpdateRow(int i, QStringList rowDat);
    void UpdateRows(QVector<QStringList> rowsDat);

public:
    QVector<QStringList> m_tagRows;
};

//-----------------------------------------------------------------------------

class Tag;

typedef enum {
    TagAct_Add = 0,
    TagAct_Copy,
    TagAct_Cut,
    TagAct_Paste,
    TagAct_Delete,
    TagAct_Edit,

    TagAct_None = 255
} TagTableActonType;

class QTableWidgetEx : public QTableView
{
    Q_OBJECT
public:
    QTableWidgetEx(QWidget *parent = Q_NULLPTR);
    ~QTableWidgetEx();

    // 清空变量表
    void clearTable();
    // 刷新变量表
    void updateTable();
    // 启用或禁用功能菜单
    void setActionEnable(TagTableActonType id, bool enable);

public slots:
    // 单元格被双击
    void onDoubleClicked(const QModelIndex &index);

    // 添加变量
    void onAddTag();
    // 复制
    void onCopyTag();
    // 粘贴
    void onPasteTag();
    // 删除
    void onDeleteTag();
    // 属性
    void onEditTag();

signals:
    // 单击空白区域
    void clickedBlankArea();
    // 已经拷贝或剪切变量值剪切板
    void copyOrCutTagToClipboard();
    // 剪切的变量值已经被粘贴
    void cutTagIsPasted();
    // 更新保存按钮状态
    void notifyUpdateSaveButtonStatus(bool bEnable);

private:
    // 初始化变量表控件
    void initTagsTable();
    // 设置行数据
    void setRowData(QStringList &rowDat, Tag *pObj);

protected:
    void contextMenuEvent(QContextMenuEvent * event);
    void mousePressEvent(QMouseEvent *event);

public:
    TagTableModel *m_pTagTableModel = Q_NULLPTR;

private:
    bool m_bCopyOrCutDone = false; // true-执行过复制或剪切, false-未执行过复制或剪切
    QMap<TagTableActonType, QAction*> m_mapIdToAction;

};



//------------------------------------------------------------------------------


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

    // 中间变量表新增一行
    int tableTagTmpAddRow();
    // 获取指定行中间变量id数值部分
    int getTagTmpIdNumValue(int iRow);
    // 刷新中间变量表
    void updateTableTagTmp();
    // 中间变量表变量导出至CSV文件
    void tagTmpExportToCsv(const QString &path, const QString & /*group*/);
    // 从CSV文件导入中间变量至中间变量表
    void tagTmpImportFromCsv(const QString &path);
    // 创建中间变量
    void createTagTmp();
    // 追加中间变量
    void appendTagTmp();
    // 拷贝选中中间变量
    void copyCurTagTmp();
    // 修改选中中间变量
    void modifyCurTagTmp();
    // 删除选中中间变量
    void deleteCurTagTmp();

    // 设备变量表新增一行
    int tableTagIOAddRow();
    // 获取指定行设备变量id数值部分
    int getTagIOIdNumValue(int iRow);
    // 刷新设备变量表
    void updateTableTagIO();
    // 设备变量表变量导出至CSV文件
    void tagIOExportToCsv(const QString &path, const QString & /*group*/);
    // 从CSV文件导入设备变量至设备变量表
    void tagIOImportFromCsv(const QString &path);
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
    QTableWidgetEx *m_pTagMgrTableViewObj;
};

#endif // TAGMANAGERWIN_H
