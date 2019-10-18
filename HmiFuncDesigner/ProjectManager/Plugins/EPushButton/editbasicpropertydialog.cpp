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
    funcs_.clear();
    script_ = "";
    bShowContentText_ = true;
    filePicture_ = "";
    elementText_ = "";
    szHAlign_ = "";
    szVAlign_ = "";
    ui->editFuncOpt->setReadOnly(true);
    ui->editExecScript->setReadOnly(true);
    ui->editSelectedPic->setReadOnly(true);
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
    script_ = ui->editExecScript->text();
    bShowContentText_ = ui->rBtnText->isChecked();
    filePicture_ = ui->editSelectedPic->text();
    elementText_ = ui->editText->toPlainText();
    szHAlign_ = ui->cboHAlign->currentText();
    szVAlign_ = ui->cboVAlign->currentText();
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
 * @brief EditBasicPropertyDialog::on_btnExecScript_clicked
 * @details 单击执行脚本按钮
 */
void EditBasicPropertyDialog::on_btnExecScript_clicked()
{
    ScriptPropertyEditorDlg dlg(Q_NULLPTR, supportEvents_);
    dlg.setScript(script_);
    if(dlg.exec() == QDialog::Accepted) {
        QString script = dlg.getScript();
        setScript(script);
    }
}

/**
 * @brief EditBasicPropertyDialog::on_btnSelectedPic_clicked
 * @details 单击选择图片按钮
 */
void EditBasicPropertyDialog::on_btnSelectedPic_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(
                this,
                tr("选择图片"),
                QStandardPaths::displayName(QStandardPaths::DesktopLocation),
                "image files (*.png *.jpg *.jpeg *.bmp)");
    if (filePath.isNull())
        return;
    ui->editSelectedPic->setText(filePath);
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

QString EditBasicPropertyDialog::elementText() const
{
    return elementText_;
}

void EditBasicPropertyDialog::setElementText(const QString &elementText)
{
    elementText_ = elementText;
}

QString EditBasicPropertyDialog::filePicture() const
{
    return filePicture_;
}

void EditBasicPropertyDialog::setFilePicture(const QString &filePicture)
{
    filePicture_ = filePicture;
    ui->editSelectedPic->setText(filePicture);
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

QString EditBasicPropertyDialog::script() const
{
    return script_;
}

void EditBasicPropertyDialog::setScript(const QString &script)
{
    script_ = script;
    ui->editExecScript->setText(script);
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
