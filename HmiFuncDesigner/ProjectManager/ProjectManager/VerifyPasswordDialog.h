#ifndef VERIFYPASSWORDDIALOG_H
#define VERIFYPASSWORDDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>

class VerifyPasswordDialog : public QDialog
{
    Q_OBJECT

public:
    VerifyPasswordDialog(bool bOpen = true, QWidget *parent = NULL);
    ~VerifyPasswordDialog();
    // 设置匹配的工程密码
    void setTargetPassword(const QString &szPwd);
    // 获取设置的工程密码
    QString getSetPassword();

private slots:
    void onOkClicked();

private:
    QLineEdit *m_pPwdEditObj = NULL;
    QLineEdit *m_pVerifyPwdEditObj = NULL;
    bool m_bOpenProj = true;
    QString m_szPwd = "";
    QPushButton *m_pBtnOkObj = NULL;
    QPushButton *m_pBtnCancelObj = NULL;
};

#endif // VERIFYPASSWORDDIALOG_H
