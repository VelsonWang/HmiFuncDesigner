#ifndef CHILDFORM_H
#define CHILDFORM_H

#include <QWidget>

#include "SystemParametersWin.h"


namespace Ui {
class ChildForm;
}

enum PAGE_FLOWTYPE{
    PAGE_SYSTEM_PARAMETER,
};
Q_DECLARE_METATYPE(PAGE_FLOWTYPE)

class ChildForm : public QWidget
{
    Q_OBJECT

public:
    explicit ChildForm(QWidget *parent = 0, const QString &projName = "");
    ~ChildForm();

    // 页面切换
    void switchPage(PAGE_FLOWTYPE page);

    QString getProjectName() const;
    void setProjectName(const QString & s);
    // 设置文件修改标志
    void setModifiedFlag(bool b);
    // 获取文件修改标志
    bool getModifiedFlag();

/*
    // 打开文件
    virtual void open() = 0;
    // 保存文件
    virtual void save() = 0;
    // 显示大图标
    virtual void ShowLargeIcon() = 0;
    // 显示小图标
    virtual void ShowSmallIcon() = 0;
*/
private:


private:
    PAGE_FLOWTYPE m_currPageFlow; // 当前页面
    bool m_bModifiedFlag;
    QString m_strProjectName;
    SystemParametersWin *m_sysParamWinPtr; //系统参数设置

private:
    Ui::ChildForm *ui;
};

#endif // CHILDFORM_H
