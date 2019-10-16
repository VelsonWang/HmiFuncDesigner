#include "editbasicpropertydialog.h"
#include "ui_editbasicpropertydialog.h"
#include "inserttagdlg.h"
#include <QFileDialog>
#include <QStandardPaths>

EditBasicPropertyDialog::EditBasicPropertyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditBasicPropertyDialog)
{
    ui->setupUi(this);
    szSelectedTag_ = "";
    stateOnInitial_ = false;
    resetFileIndicationLamp_ = "";
    setFileIndicationLamp_ = "";
    ui->editTagSelected->setReadOnly(true);
    ui->editPicReset->setReadOnly(true);
    ui->editPicSet->setReadOnly(true);
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
    stateOnInitial_ = ui->rBtnOpen->isChecked();
    resetFileIndicationLamp_ = ui->editPicReset->text();
    setFileIndicationLamp_ = ui->editPicSet->text();
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


QString EditBasicPropertyDialog::setFileIndicationLamp() const
{
    return setFileIndicationLamp_;
}

void EditBasicPropertyDialog::setSetFileIndicationLamp(const QString &setFileIndicationLamp)
{
    setFileIndicationLamp_ = setFileIndicationLamp;
    ui->editPicSet->setText(setFileIndicationLamp);
}

QString EditBasicPropertyDialog::resetFileIndicationLamp() const
{
    return resetFileIndicationLamp_;
}

void EditBasicPropertyDialog::setResetFileIndicationLamp(const QString &resetFileIndicationLamp)
{
    resetFileIndicationLamp_ = resetFileIndicationLamp;
    ui->editPicReset->setText(resetFileIndicationLamp);
}

bool EditBasicPropertyDialog::stateOnInitial() const
{
    return stateOnInitial_;
}

void EditBasicPropertyDialog::setStateOnInitial(bool stateOnInitial)
{
    stateOnInitial_ = stateOnInitial;
    ui->rBtnOpen->setChecked(stateOnInitial);
    ui->rBtnClose->setChecked(!stateOnInitial);
}

/**
 * @brief EditBasicPropertyDialog::on_btnSelectPicReset_clicked
 * @details 选择复位图片
 */
void EditBasicPropertyDialog::on_btnSelectPicReset_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(
                this,
                tr("选择复位图片"),
                QStandardPaths::displayName(QStandardPaths::DesktopLocation),
                "image files (*.png *.jpg *.jpeg *.bmp)");
    if (filePath.isNull())
        return;
    ui->editPicReset->setText(filePath);
}

/**
 * @brief EditBasicPropertyDialog::on_btnSelectPicSet_clicked
 * @details 选择置位图片
 */
void EditBasicPropertyDialog::on_btnSelectPicSet_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(
                this,
                tr("选择置位图片"),
                QStandardPaths::displayName(QStandardPaths::DesktopLocation),
                "image files (*.png *.jpg *.jpeg *.bmp)");
    if (filePath.isNull())
        return;
    ui->editPicSet->setText(filePath);
}
