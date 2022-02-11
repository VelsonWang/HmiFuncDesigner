#include "inserttagdlg.h"
#include "ui_inserttagdlg.h"
#include "qsoftcore.h"
#include "../../shared/qprojectcore.h"

InsertTagDlg::InsertTagDlg(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::InsertTagDlg)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() & (~Qt::WindowContextHelpButtonHint));
    m_varsList.clear();
    QSoftCore::getCore()->getProjectCore()->getAllTagName(m_varsList, "IO");
    for(QString varName : m_varsList) {
        QListWidgetItem *pItem = new QListWidgetItem(QIcon(":/images/tag.png"), varName);
        ui->listWidgetTagIO->addItem(pItem);
    }

    m_varsList.clear();
    QSoftCore::getCore()->getProjectCore()->getAllTagName(m_varsList, "TMP");
    for(QString varName : m_varsList) {
        QListWidgetItem *pItem = new QListWidgetItem(QIcon(":/images/tag.png"), varName);
        ui->listWidgetTagTmp->addItem(pItem);
    }

    m_varsList.clear();
    QSoftCore::getCore()->getProjectCore()->getAllTagName(m_varsList, "SYS");
    for(QString varName : m_varsList) {
        QListWidgetItem *pItem = new QListWidgetItem(QIcon(":/images/tag.png"), varName);
        ui->listWidgetTagSys->addItem(pItem);
    }

}

InsertTagDlg::~InsertTagDlg()
{
    delete ui;
}

/**
 * @brief InsertTagDlg::on_btnOk_clicked
 * @details 插槽： 确定
 *
 */
void InsertTagDlg::on_btnOk_clicked()
{
    QDialog::accept();
}

/**
 * @brief InsertTagDlg::on_btnCancel_clicked
 * @details 插槽： 取消
 */
void InsertTagDlg::on_btnCancel_clicked()
{
    m_selectedTagName = "";
    QDialog::reject();
}

/**
 * @brief InsertTagDlg::on_listWidgetTagIO_itemClicked
 * @param item
 */
void InsertTagDlg::on_listWidgetTagIO_itemClicked(QListWidgetItem *item)
{
    Q_UNUSED(item)
    ui->listWidgetTagSys->clearSelection();
    ui->listWidgetTagTmp->clearSelection();
    m_selectedTagName = ui->listWidgetTagIO->currentItem()->text();
}

/**
 * @brief InsertTagDlg::on_listWidgetTagTmp_itemClicked
 * @param item
 */
void InsertTagDlg::on_listWidgetTagTmp_itemClicked(QListWidgetItem *item)
{
    Q_UNUSED(item)
    ui->listWidgetTagSys->clearSelection();
    ui->listWidgetTagIO->clearSelection();
    m_selectedTagName = ui->listWidgetTagTmp->currentItem()->text();
}

/**
 * @brief InsertTagDlg::on_listWidgetTagSys_itemClicked
 * @param item
 */
void InsertTagDlg::on_listWidgetTagSys_itemClicked(QListWidgetItem *item)
{
    Q_UNUSED(item)
    ui->listWidgetTagIO->clearSelection();
    ui->listWidgetTagTmp->clearSelection();
    m_selectedTagName = ui->listWidgetTagSys->currentItem()->text();
}

QString InsertTagDlg::getSelectedTagName(void) const
{
    return m_selectedTagName;
}
