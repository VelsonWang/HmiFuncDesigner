#include "editbasicpropertydialog.h"
#include "ui_editbasicpropertydialog.h"
#include "inserttagdlg.h"
#include "tagtextlisteditordialog.h"
#include <QFileDialog>
#include <QStandardPaths>
#include <cfloat>
#include "functioneditordialog.h"
#include "scriptpropertyeditordlg.h"

EditBasicPropertyDialog::EditBasicPropertyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditBasicPropertyDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() & (~Qt::WindowContextHelpButtonHint));
    funcs_.clear();
    supportEvents_.clear();
    szTagSelected_ = "";
    stateOnInitial_ = false;
    bShowContentText_ = true;
    resetPictureFile_ = "";
    setPictureFile_ = "";
    resetText_ = "";
    setText_ = "";
    szHAlign_ = "";
    szVAlign_ = "";
    ui->editFuncOpt->setReadOnly(true);
    ui->editTagSelected->setReadOnly(true);
    ui->editPicReset->setReadOnly(true);
    ui->editPicSet->setReadOnly(true);
    ui->cboHAlign->clear();
    ui->cboHAlign->addItem(tr("左对齐"));
    ui->cboHAlign->addItem(tr("居中对齐"));
    ui->cboHAlign->addItem(tr("右对齐"));
    ui->cboVAlign->clear();
    ui->cboVAlign->addItem(tr("上对齐"));
    ui->cboVAlign->addItem(tr("居中对齐"));
    ui->cboVAlign->addItem(tr("下对齐"));
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
    funcs_ = ui->editFuncOpt->text().split('|');;
    stateOnInitial_ = ui->rBtnOpen->isChecked();
    bShowContentText_ = ui->rBtnText->isChecked();
    resetText_ = ui->editTextReset->toPlainText();
    setText_ = ui->editTextSet->toPlainText();
    szHAlign_ = ui->cboHAlign->currentText();
    szVAlign_ = ui->cboVAlign->currentText();
    resetPictureFile_ = ui->editPicReset->text();
    setPictureFile_ = ui->editPicSet->text();
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
 * @brief EditBasicPropertyDialog::on_btnFuncOpt_clicked
 * @details 单击执行功能按钮
 */
void EditBasicPropertyDialog::on_btnFuncOpt_clicked()
{
    FunctionEditorDialog dlg(Q_NULLPTR, supportEvents_);
    dlg.setFunctions(funcs_);
    if(dlg.exec() == QDialog::Accepted) {
        QStringList funcs = dlg.getFunctions();
        setFuncs(funcs);
    }
}

/**
 * @brief EditBasicPropertyDialog::on_btnSelectTag_clicked
 * @details: 单击选择变量按钮
 */
void EditBasicPropertyDialog::on_btnSelectTag_clicked()
{
    InsertTagDlg *pDlg = new InsertTagDlg(this);
    if (pDlg->exec() == QDialog::Accepted) {
        setTagSelected(pDlg->getSelectedTagName());
    }
    delete pDlg;
}

/**
 * @brief EditBasicPropertyDialog::on_btnSelectedPic_clicked
 * @details 单击选择复位图片按钮
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
 * @brief EditBasicPropertyDialog::on_btnSelectedPic_clicked
 * @details 单击选择置位图片按钮
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


QStringList EditBasicPropertyDialog::supportEvents() const
{
    return supportEvents_;
}

void EditBasicPropertyDialog::setSupportEvents(const QStringList &supportEvents)
{
    supportEvents_ = supportEvents;
}

QString EditBasicPropertyDialog::vAlign() const
{
    return szVAlign_;
}

void EditBasicPropertyDialog::setVAlign(const QString &szVAlign)
{
    szVAlign_ = szVAlign;
    ui->cboVAlign->setCurrentText(szVAlign);
}

QString EditBasicPropertyDialog::hAlign() const
{
    return szHAlign_;
}

void EditBasicPropertyDialog::setHAlign(const QString &szHAlign)
{
    szHAlign_ = szHAlign;
    ui->cboHAlign->setCurrentText(szHAlign);
}


bool EditBasicPropertyDialog::showContentText() const
{
    return bShowContentText_;
}

void EditBasicPropertyDialog::setShowContentText(bool bShowContentText)
{
    bShowContentText_ = bShowContentText;
    ui->rBtnText->setChecked(bShowContentText);
    ui->groupBoxText->setEnabled(bShowContentText);
    ui->rBtnPic->setChecked(!bShowContentText);
    ui->groupBoxPic->setEnabled(!bShowContentText);
}


QStringList EditBasicPropertyDialog::funcs() const
{
    return funcs_;
}

void EditBasicPropertyDialog::setFuncs(const QStringList &funcs)
{
    funcs_ = funcs;
    ui->editFuncOpt->setText(funcs.join('|'));
}


void EditBasicPropertyDialog::on_rBtnText_clicked(bool checked)
{
    ui->groupBoxText->setEnabled(checked);
    ui->groupBoxPic->setEnabled(!checked);
}

void EditBasicPropertyDialog::on_rBtnPic_clicked(bool checked)
{
    ui->groupBoxText->setEnabled(!checked);
    ui->groupBoxPic->setEnabled(checked);
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

QString EditBasicPropertyDialog::setText() const
{
    return setText_;
}

void EditBasicPropertyDialog::setSetText(const QString &setText)
{
    setText_ = setText;
    ui->editTextSet->setText(setText);
}

QString EditBasicPropertyDialog::resetText() const
{
    return resetText_;
}

void EditBasicPropertyDialog::setResetText(const QString &resetText)
{
    resetText_ = resetText;
    ui->editTextReset->setText(resetText);
}

QString EditBasicPropertyDialog::setPictureFile() const
{
    return setPictureFile_;
}

void EditBasicPropertyDialog::setSetPictureFile(const QString &setPictureFile)
{
    setPictureFile_ = setPictureFile;
    ui->editPicSet->setText(setPictureFile);
}

QString EditBasicPropertyDialog::resetPictureFile() const
{
    return resetPictureFile_;
}

void EditBasicPropertyDialog::setResetPictureFile(const QString &resetPictureFile)
{
    resetPictureFile_ = resetPictureFile;
    ui->editPicReset->setText(resetPictureFile);
}

QString EditBasicPropertyDialog::tagSelected() const
{
    return szTagSelected_;
}

void EditBasicPropertyDialog::setTagSelected(const QString &szTagSelected)
{
    szTagSelected_ = szTagSelected;
    ui->editTagSelected->setText(szTagSelected_);
}


