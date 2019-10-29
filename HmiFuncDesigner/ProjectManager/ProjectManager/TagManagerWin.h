#ifndef TAGMANAGERWIN_H
#define TAGMANAGERWIN_H

#include "ChildBase.h"
#include "TagFuncEditDialog.h"
#include "../../Public/Public.h"
#include "TagManager.h"
#include <QList>
#include <QString>
#include <QWidget>
#include <QStandardItemModel>
#include <QTableWidgetItem>
#include <QStringList>
#include <QVBoxLayout>

//////////////////////////////////////////////////////////


namespace Ui {
class TagManagerWin;
}


struct TagTmpDBItem;
struct TagIODBItem;

class TagManagerWin : public ChildBase
{
    Q_OBJECT

public:
    explicit TagManagerWin(QWidget *parent = Q_NULLPTR,
                                const QString &itemName = "",
                                const QString &projName = "");
    ~TagManagerWin();

    void init(const QString &itemName = "");

public:
    // 打开文件
    void open();
    // 保存文件
    void save();
    // 显示大图标
    void showLargeIcon();
    // 显示小图标
    void showSmallIcon();

public:
    void exportToCsv(const QString &path);
    void importFromCsv(const QString &path);
    void SetTitle(const QString &t);

private:
    void load(const QString &it);
    void save(const QString &it);

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
    void updateTableTagIO(const QString &szGroupName);
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

protected:
    void contextMenuEvent(QContextMenuEvent * event);
    void closeEvent(QCloseEvent *event);  // 关闭事件

public slots:
    void onTagAdd();
    void onTagAppend();
    void onTagRowCopy();
    void onTagColCopy();
    void onTagModify();
    void onTagDelete();

public:
    QString m_IOVariableListWhat;

private slots:
    void on_tableTagIO_itemPressed(QTableWidgetItem *item);
    void on_tableTagTmp_itemPressed(QTableWidgetItem *item);
    void on_tableTagTmp_itemDoubleClicked(QTableWidgetItem *item);
    void on_tableTagIO_itemDoubleClicked(QTableWidgetItem *item);

private:
    Ui::TagManagerWin *ui; 
    QString m_strCurTagType;
    int m_iTableTagTmpSelectedRow = -1;
    int m_iTableTagIOSelectedRow = -1;
    QStringList m_listTagTmpDeleteRows;
};

#endif // TAGMANAGERWIN_H
