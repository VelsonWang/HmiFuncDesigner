#include "selectprotocoldialog.h"
#include "ui_selectprotocoldialog.h"

SelectProtocolDialog::SelectProtocolDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectProtocolDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() & (~Qt::WindowContextHelpButtonHint));
    m_protocolName = "";
}

SelectProtocolDialog::~SelectProtocolDialog()
{
    delete ui;
}

/*
* 设置支持的协议列表
*/
void SelectProtocolDialog::SetProtocolList(QStringList list)
{
    foreach (QString s, list) {
        ui->ProtocolListWidget->addItem(s);
    }
}

/*
* 获取选中的协议名
*/
QString SelectProtocolDialog::GetProtocolName()
{
    return m_protocolName;
}

void SelectProtocolDialog::on_ProtocolListWidget_currentTextChanged(const QString &currentText)
{
    m_protocolName = currentText;
}

void SelectProtocolDialog::on_btnOk_clicked()
{
    accept();
}

void SelectProtocolDialog::on_btnExit_clicked()
{
    reject();
}


