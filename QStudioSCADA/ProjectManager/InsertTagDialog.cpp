#include "InsertTagDialog.h"
#include "ui_InsertTagDialog.h"
#include "TagManager.h"

InsertTagDialog::InsertTagDialog(QString projectPath, QWidget *parent) :
    QDialog(parent),
    m_strProjectPath(projectPath),
    ui(new Ui::InsertTagDialog)
{
    ui->setupUi(this);

    m_varsList.clear();
    TagManager::getAllTagName(m_strProjectPath, m_varsList, "IO");
    for(QString varName : m_varsList)
    {
        QListWidgetItem *pItem = new QListWidgetItem(QIcon(":/images/pj_var.png"), varName);
        ui->listWidgetTagIO->addItem(pItem);
    }

    m_varsList.clear();
    TagManager::getAllTagName(m_strProjectPath, m_varsList, "TMP");
    for(QString varName : m_varsList)
    {
        QListWidgetItem *pItem = new QListWidgetItem(QIcon(":/images/pj_var.png"), varName);
        ui->listWidgetTagTmp->addItem(pItem);
    }

    m_varsList.clear();
    TagManager::getAllTagName(m_strProjectPath, m_varsList, "SYS");
    for(QString varName : m_varsList)
    {
        QListWidgetItem *pItem = new QListWidgetItem(QIcon(":/images/pj_var.png"), varName);
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
    m_strSelectedTagName = "";
    QDialog::reject();
}

/**
 * @brief InsertTagDialog::on_listWidgetTagIO_itemClicked
 * @param item
 */
void InsertTagDialog::on_listWidgetTagIO_itemClicked(QListWidgetItem *item)
{
    ui->listWidgetTagSys->clearSelection();
    ui->listWidgetTagTmp->clearSelection();
    m_strSelectedTagName = ui->listWidgetTagIO->currentItem()->text();
}

/**
 * @brief InsertTagDialog::on_listWidgetTagTmp_itemClicked
 * @param item
 */
void InsertTagDialog::on_listWidgetTagTmp_itemClicked(QListWidgetItem *item)
{
    ui->listWidgetTagSys->clearSelection();
    ui->listWidgetTagIO->clearSelection();
    m_strSelectedTagName = ui->listWidgetTagTmp->currentItem()->text();
}

/**
 * @brief InsertTagDialog::on_listWidgetTagSys_itemClicked
 * @param item
 */
void InsertTagDialog::on_listWidgetTagSys_itemClicked(QListWidgetItem *item)
{
    ui->listWidgetTagIO->clearSelection();
    ui->listWidgetTagTmp->clearSelection();
    m_strSelectedTagName = ui->listWidgetTagSys->currentItem()->text();
}

QString InsertTagDialog::getSelectedTagName(void) const
{
    return m_strSelectedTagName;
}
