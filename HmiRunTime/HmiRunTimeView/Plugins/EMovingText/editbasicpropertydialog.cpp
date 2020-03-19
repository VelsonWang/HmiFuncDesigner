#include "editbasicpropertydialog.h"
#include "ui_editbasicpropertydialog.h"
#include "inserttagdlg.h"
#include "tagtextlisteditordialog.h"
#include <QFileDialog>
#include <QStandardPaths>
#include <cfloat>

EditBasicPropertyDialog::EditBasicPropertyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditBasicPropertyDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() & (~Qt::WindowContextHelpButtonHint));
    szSelectedTag_ = "";
    tagTextList_.clear();
    szMoveDir_ = "";
    iMoveCharNum_ = 1;
    period_ = 1.0;

    ui->editTagSelected->setReadOnly(true);
    ui->editTagTextList->setReadOnly(true);
    ui->cboMoveDir->clear();
    ui->cboMoveDir->addItem(tr("从左到右"));
    ui->cboMoveDir->addItem(tr("从右到左"));

    ui->spinBoxNum->setRange(0, 5000);
    ui->spinBoxNum->setSingleStep(1);

    ui->SpinBoxSecond->setRange(0, 6000);
    ui->SpinBoxSecond->setSingleStep(0.1);
    ui->SpinBoxSecond->setDecimals(1);

    this->setWindowTitle(tr("编辑基本属性"));
}

EditBasicPropertyDialog::~EditBasicPropertyDialog()
{
    delete ui;
}

/**
 * @brief EditBasicPropertyDialog::on_btnOk_clicked
 * @details 单击确定按钮
 */
void EditBasicPropertyDialog::on_btnOk_clicked()
{
    szSelectedTag_ = ui->editTagSelected->text();
    tagTextList_ = ui->editTagTextList->text().split('|');
    szElementText_ = ui->editMoveText->toPlainText();
    szMoveDir_ = ui->cboMoveDir->currentText();
    iMoveCharNum_ = ui->spinBoxNum->value();
    period_ = ui->SpinBoxSecond->value();
    this->accept();
}

/**
 * @brief EditBasicPropertyDialog::on_btnCancel_clicked
 * @details 单击取消按钮
 */
void EditBasicPropertyDialog::on_btnCancel_clicked()
{
    this->reject();
}

/**
 * @brief EditBasicPropertyDialog::on_btnSelectTag_clicked
 * @details: 单击选择变量按钮
 */
void EditBasicPropertyDialog::on_btnSelectTag_clicked()
{
    InsertTagDlg *pDlg = new InsertTagDlg(this);
    if (pDlg->exec() == QDialog::Accepted) {
        setSelectedTag(pDlg->getSelectedTagName());
    }
    delete pDlg;
}

QString EditBasicPropertyDialog::selectedTag() const
{
    return szSelectedTag_;
}

void EditBasicPropertyDialog::setSelectedTag(const QString &szSelectedTag)
{
    szSelectedTag_ = szSelectedTag;
    ui->editTagSelected->setText(szSelectedTag);
}


/**
 * @brief EditBasicPropertyDialog::on_btnTagTextList_clicked
 * @details 单击变量文本列表按钮
 */
void EditBasicPropertyDialog::on_btnTagTextList_clicked()
{
    TagTextListEditorDialog dlg;
    dlg.setValueTextList(tagTextList_);

    if(dlg.exec() == QDialog::Accepted) {
        setTagTextList(dlg.getValueTextList());
    }
}

QString EditBasicPropertyDialog::elementText() const
{
    return szElementText_;
}

void EditBasicPropertyDialog::setElementText(const QString &szElementText)
{
    szElementText_ = szElementText;
    ui->editMoveText->setText(szElementText);
}

double EditBasicPropertyDialog::period() const
{
    return period_;
}

void EditBasicPropertyDialog::setPeriod(double period)
{
    period_ = period;
    ui->SpinBoxSecond->setValue(period);
}

int EditBasicPropertyDialog::moveCharNum() const
{
    return iMoveCharNum_;
}

void EditBasicPropertyDialog::setMoveCharNum(int iMoveCharNum)
{
    iMoveCharNum_ = iMoveCharNum;
    ui->spinBoxNum->setValue(iMoveCharNum);
}

QString EditBasicPropertyDialog::moveDir() const
{
    return szMoveDir_;
}

void EditBasicPropertyDialog::setMoveDir(const QString &szMoveDir)
{
    szMoveDir_ = szMoveDir;
    ui->cboMoveDir->setCurrentText(szMoveDir);
}

QStringList EditBasicPropertyDialog::tagTextList() const
{
    return tagTextList_;
}

void EditBasicPropertyDialog::setTagTextList(const QStringList &tagTextList)
{
    tagTextList_ = tagTextList;
    ui->editTagTextList->setText(tagTextList.join('|'));
}
