#include "VerifyPasswordDialog.h"
#include <QFormLayout>
#include <QRegExpValidator>
#include <QRegExp>
#include <QValidator>
#include <QMessageBox>
#include <QCryptographicHash>
#include <QDebug>

VerifyPasswordDialog::VerifyPasswordDialog(bool bOpen, QWidget *parent)
    : QDialog(parent),
      m_bOpenProj(bOpen)
{
    this->setWindowFlags(this->windowFlags() & (~Qt::WindowContextHelpButtonHint));
    this->setWindowTitle(tr("请输入工程密码"));
    QFormLayout *pFormLayoutObj = new QFormLayout;
    pFormLayoutObj->setRowWrapPolicy(QFormLayout::DontWrapRows);
    pFormLayoutObj->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);
    pFormLayoutObj->setFormAlignment(Qt::AlignHCenter | Qt::AlignTop);
    pFormLayoutObj->setLabelAlignment(Qt::AlignLeft);

    m_pPwdEditObj = new QLineEdit(this);
    m_pPwdEditObj->setEchoMode(QLineEdit::Password);
    m_pPwdEditObj->setMaxLength(24);
    QRegExp regx("[a-zA-Z0-9-]{0,24}");
    QValidator *pValidatorObj = new QRegExpValidator(regx, m_pPwdEditObj);
    m_pPwdEditObj->setValidator(pValidatorObj);
    pFormLayoutObj->addRow(tr("输入密码:"), m_pPwdEditObj);
    if(!bOpen) {
        m_pVerifyPwdEditObj = new QLineEdit(this);
        m_pVerifyPwdEditObj->setEchoMode(QLineEdit::Password);
        m_pVerifyPwdEditObj->setMaxLength(24);
        QValidator *pValidatorObj2 = new QRegExpValidator(regx, m_pVerifyPwdEditObj);
        m_pVerifyPwdEditObj->setValidator(pValidatorObj2);
        pFormLayoutObj->addRow(tr("再次输入密码:"), m_pVerifyPwdEditObj);
    }

    m_pBtnOkObj = new QPushButton(tr("确定"), this);
    connect(m_pBtnOkObj, SIGNAL(clicked(bool)), this, SLOT(onOkClicked()));
    m_pBtnCancelObj = new QPushButton(tr("取消"), this);
    connect(m_pBtnCancelObj, SIGNAL(clicked(bool)), this, SLOT(reject()));

    QHBoxLayout *pHLayoutObj = new QHBoxLayout;
    QSpacerItem *pSpacerItemObj = new QSpacerItem(40, 26, QSizePolicy::Expanding, QSizePolicy::Preferred);
    pHLayoutObj->addSpacerItem(pSpacerItemObj);
    pHLayoutObj->addWidget(m_pBtnOkObj);
    pHLayoutObj->addWidget(m_pBtnCancelObj);
    pFormLayoutObj->addRow(pHLayoutObj);

    setLayout(pFormLayoutObj);
}

VerifyPasswordDialog::~VerifyPasswordDialog()
{

}


///
/// \brief VerifyPasswordDialog::setTargetPassword
/// \details 设置匹配的工程密码
/// \param szPwd 密码
///
void VerifyPasswordDialog::setTargetPassword(const QString &szPwd)
{
    m_szPwd = szPwd;
}

///
/// \brief VerifyPasswordDialog::getSetPassword
/// \details 获取设置的工程密码
/// \return 密码
///
QString VerifyPasswordDialog::getSetPassword()
{
    return m_szPwd;
}


void VerifyPasswordDialog::onOkClicked()
{
    QString szPasswd = m_pPwdEditObj->text().trimmed();
    QString szVerifyPasswd = "";
    //qDebug() << "VerifyPasswordDialog::onOkClicked() m_szPwd: " << m_szPwd;
    if(m_bOpenProj) {
        QCryptographicHash crypt(QCryptographicHash::Md5);
        crypt.reset();
        crypt.addData(szPasswd.toStdString().c_str(), szPasswd.toStdString().length());
        QByteArray baPwd = crypt.result();
        if(baPwd.toHex() != m_szPwd) {
            m_pPwdEditObj->clear();
            QMessageBox::information(this, tr("提示"), tr("密码错误!"));
            return;
        }
    } else {
        szVerifyPasswd = m_pVerifyPwdEditObj->text().trimmed();
        if(szPasswd != szVerifyPasswd) {
            m_pPwdEditObj->clear();
            m_pVerifyPwdEditObj->clear();
            QMessageBox::information(this, tr("提示"), tr("两次输入密码不一致。"));
            return;
        }
        m_szPwd = m_pPwdEditObj->text().trimmed();
    }

    this->accept();
}
