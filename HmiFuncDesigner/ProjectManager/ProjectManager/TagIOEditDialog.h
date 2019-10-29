#ifndef TAGIOEDITDIALOG_H
#define TAGIOEDITDIALOG_H

#include <QDialog>
#include <QStringList>

class IDevicePlugin;

namespace Ui {
class TagIOEditDialog;
}

class TagIOEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TagIOEditDialog(QString projName, QWidget *parent = Q_NULLPTR);
    ~TagIOEditDialog();

public:
    int createTagNum();
    void setCreateTagNum(int iNum);

    int addrOffset();
    void setAddrOffset(int iOffset);

    QString tagName();
    void setTagName(const QString &szName);

    QString tagDesc();
    void setTagDesc(const QString &szDesc);

    QString deviceName();
    void setDeviceName(const QString &szName);

    QString deviceAddr();
    void setDeviceAddr(const QString &szAddr);

    QString regSection();
    void setRegSection(const QString &szSect);

    QString regAddr();
    void setRegAddr(const QString &szRegAddr);

    QString regAddrOffset();
    void setRegAddrOffset(const QString &szOffset);

    QString typeReadWrite();
    void setTypeReadWrite(const QString &szType);

    QString dataType();
    void setDataType(const QString &szType);

    QString tagInitValue();
    void setTagInitValue(const QString &szInit);

    QString tagMinValue();
    void setTagMinValue(const QString &szMin);

    QString tagMaxValue();
    void setTagMaxValue(const QString &szMax);

    QString scale() const;
    void setScale(const QString &szScale);

private slots:
    void on_btnOk_clicked();
    void on_btnCancel_clicked();
    void on_cboDeviceName_currentTextChanged(const QString &arg1);

private:
    Ui::TagIOEditDialog *ui;
    QString m_szProjectName;

    IDevicePlugin *devPlugin_;

    int m_iCreateTagNum = 1; // 创建个数
    int m_iAddrOffset; // 寄存器地址间隔

    QString m_szTagName = ""; // 名称
    QString m_szTagDesc = ""; // 变量描述
    QString m_szDeviceName = ""; // 设备名
    QString m_szDeviceAddr = ""; // 设备地址
    QString m_szRegSection = ""; // 寄存器区
    QString m_szRegAddr = ""; // 寄存器地址
    QString m_szRegAddrOffset = ""; // 偏移地址
    QString m_szTypeReadWrite = ""; // 读写类型
    QString m_szDataType = ""; // 数据类型
    QString m_szTagInitValue = ""; // 初始值
    QString m_szTagMinValue = ""; // 最小值
    QString m_szTagMaxValue = ""; // 最大值
    QString m_szScale = "";
};

#endif // TAGIOEDITDIALOG_H



