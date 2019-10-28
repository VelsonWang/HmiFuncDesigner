#ifndef VARIABLEMANAGERWIN_H
#define VARIABLEMANAGERWIN_H

#include "ChildBase.h"
#include "TagFuncEditDialog.h"
#include "../../Public/Public.h"
#include "TagManager.h"
#include <QList>
#include <QString>
#include <QWidget>
#include <QStandardItemModel>
#include <QStringList>
#include <QVBoxLayout>

//////////////////////////////////////////////////////////


namespace Ui {
class VariableManagerWin;
}

class VariableManagerWin : public ChildBase
{
    Q_OBJECT

public:
    explicit VariableManagerWin(QWidget *parent = Q_NULLPTR,
                                const QString &itemName = "",
                                const QString &projName = "");
    ~VariableManagerWin();

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
    // 拷贝选中中间变量
    void copyCurTagTmp();

    // 初始化设备变量表
    void initialTableTagIO();
    // 刷新设备变量表
    void updateTableTagIO(const QString &szGroupName);
    // 保存设备变量表
    void saveTableTagIO();
    // 设备变量表变量导出至CSV文件
    void tagIOExportToCsv(const QString &path, const QString & /*group*/);
    // 从CSV文件导入设备变量至设备变量表
    void tagIOImportFromCsv(const QString &path);

protected:
    void contextMenuEvent(QContextMenuEvent * event);
    void closeEvent(QCloseEvent *event);  // 关闭事件

public slots:
    void tableViewVariableDoubleClicked(const QModelIndex &index);
    void VariableAdd();
    void VariableAppend();
    void VariableRowCopy();
    void VariableColCopy();
    void VariableModify();
    void VariableDelete();

public:
    QString m_IOVariableListWhat;

private:
    Ui::VariableManagerWin *ui; 
    QString m_strCurTagType;
};

#endif // VARIABLEMANAGERWIN_H
