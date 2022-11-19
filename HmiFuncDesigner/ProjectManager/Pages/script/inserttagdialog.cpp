#include "inserttagdialog.h"
#include "ui_inserttagdialog.h"
#include "qsoftcore.h"
#include "qprojectcore.h"

InsertTagDialog::InsertTagDialog(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::InsertTagDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() & (~Qt::WindowContextHelpButtonHint));
    m_listTags.clear();
    QSoftCore::getCore()->getProjectCore()->getAllTagName(m_listTags, "IO");
    for(QString varName : m_listTags) {
        QListWidgetItem *pItem = new QListWidgetItem(QIcon(":/images/pj_var.png"), varName);
        ui->listWidgetTagIO->addItem(pItem);
    }

    m_listTags.clear();
    QSoftCore::getCore()->getProjectCore()->getAllTagName(m_listTags, "TMP");
    for(QString varName : m_listTags) {
        QListWidgetItem *pItem = new QListWidgetItem(QIcon(":/images/pj_var.png"), varName);
        ui->listWidgetTagTmp->addItem(pItem);
    }

    m_listTags.clear();
    QSoftCore::getCore()->getProjectCore()->getAllTagName(m_listTags, "SYS");
    for(QString szName : m_listTags) {
        QListWidgetItem *pItem = new QListWidgetItem(QIcon(":/images/pj_var.png"), szName);
        ui->listWidgetTagSys->addItem(pItem);
    }

}

InsertTagDialog::~InsertTagDialog()
{
    delete ui;
}

/**
 * @brief InsertTagDialog::on_btnOk_clicked
 * @details 插槽： 确定
 *
 */
void InsertTagDialog::on_btnOk_clicked()
{
    QDialog::accept();
}

/**
 * @brief InsertTagDialog::on_btnCancel_clicked
 * @details 插槽： 取消
 */
void InsertTagDialog::on_btnCancel_clicked()
{
    m_szSelectedTagName = "";
    QDialog::reject();
}

/**
 * @brief InsertTagDialog::on_listWidgetTagIO_itemClicked
 * @param item
 */
void InsertTagDialog::on_listWidgetTagIO_itemClicked(QListWidgetItem *)
{
    ui->listWidgetTagSys->clearSelection();
    ui->listWidgetTagTmp->clearSelection();
    m_szSelectedTagName = ui->listWidgetTagIO->currentItem()->text();
}

/**
 * @brief InsertTagDialog::on_listWidgetTagTmp_itemClicked
 * @param item
 */
void InsertTagDialog::on_listWidgetTagTmp_itemClicked(QListWidgetItem *)
{
    ui->listWidgetTagSys->clearSelection();
    ui->listWidgetTagIO->clearSelection();
    m_szSelectedTagName = ui->listWidgetTagTmp->currentItem()->text();
}

/**
 * @brief InsertTagDialog::on_listWidgetTagSys_itemClicked
 * @param item
 */
void InsertTagDialog::on_listWidgetTagSys_itemClicked(QListWidgetItem *)
{
    ui->listWidgetTagIO->clearSelection();
    ui->listWidgetTagTmp->clearSelection();
    m_szSelectedTagName = ui->listWidgetTagSys->currentItem()->text();
}

QString InsertTagDialog::getSelectedTagName(void) const
{
    return m_szSelectedTagName;
}
