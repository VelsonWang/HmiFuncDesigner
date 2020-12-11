#ifndef VERIFYPASSWORDDIALOG_H
#define VERIFYPASSWORDDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>

class VerifyPasswordDialog : public QDialog
{
    Q_OBJECT

public:
    VerifyPasswordDialog(bool bOpen = true, QWidget *parent = Q_NULLPTR);
    ~VerifyPasswordDialog();
    // 设置匹配的工程密码
    void setTargetPassword(const QString &szPwd);
    // 获取设置的工程密码
    QString getSetPassword();

private slots:
    void onOkClicked();

private:
    QLineEdit *m_pPwdEditObj = Q_NULLPTR;
    QLineEdit *m_pVerifyPwdEditObj = Q_NULLPTR;
    bool m_bOpenProj = true;
    QString m_szPwd = "";
    QPushButton *m_pBtnOkObj = Q_NULLPTR;
    QPushButton *m_pBtnCancelObj = Q_NULLPTR;
};

#endif // VERIFYPASSWORDDIALOG_H
