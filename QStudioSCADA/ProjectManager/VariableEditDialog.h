#ifndef VARIABLEEDITDIALOG_H
#define VARIABLEEDITDIALOG_H

#include <QDialog>
#include <QStringList>

class IDevicePlugin;

namespace Ui {
class VariableEditDialog;
}

class VariableEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit VariableEditDialog(QString projName, QWidget *parent = 0);
    ~VariableEditDialog();

public:
    enum VAR_TYPE{
        NONE,
        DI,
        AI
    };

public:
    QString GetDataType();
    QString GetName();
    QString GetDescription();
    QString GetUnit();
    QString GetBatchNum();
    void SetBatchOffset(int offset);
    QString GetDataRegisterSpace();
    void SetCurrentTabIndex(int index);
    void RemoveTab(int index);
    // 取得基本设置字符
    QStringList GetBasicSetting();
    // 设置基本设置字符
    void SetBasicSetting(QStringList sl);
    // 取得数据属性字符
    QString GetDataAttribuyeString();
    // 设置数据属性字符
    void SetDataAttribuyeString(QString s);
    // 取得报警字符
    QString GetAlarmString();
    // 设置报警字符
    void SetAlarmString(QString s);
    // 取得存盘字符
    QString GetSaveDiskString();
    // 设置存盘字符
    void SetSaveDiskString(QString s);
    // 获取变量类型
    int GetVariableType();
    // 设置变量类型
    void SetVariableType(VAR_TYPE t);
    // 取得IO连接字符
    QString GetIOConnectString();
    // 设置IO连接字符
    void SetIOConnectString(QString s);

private:
    void DialogFixedSize();


private slots:
    void on_btnOk_clicked();
    void on_btnCancel_clicked();
    void on_checkIsAlarm_clicked(bool checked);
    void on_checkIsSaveDisk_clicked(bool checked);
    void on_cboDataType_currentIndexChanged(const QString &arg1);
    void on_chkIsIOConnect_clicked(bool checked);
    void on_chkIsSelfVariable_clicked(bool checked);
    void on_chkGlobal_clicked(bool checked);
    void on_cboDeviceName_currentTextChanged(const QString &arg1);

private:
    Ui::VariableEditDialog *ui;
    QString m_strProjectName;
    VAR_TYPE m_iVarType;  // 变量类型
    int m_iRegAddrOffset;
    IDevicePlugin *devPlugin_;
};

#endif // VARIABLEEDITDIALOG_H



