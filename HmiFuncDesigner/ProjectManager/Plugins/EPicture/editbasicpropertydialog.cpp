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
    filePicture_ = "";
    showNoScale_ = false;
    borderWidth_ = 0;
    ui->editSelectedPic->setReadOnly(true);
    ui->chkShowNoScale->setChecked(showNoScale_);
    ui->spinBoxBorderWidth->setRange(0, 5000);
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
    filePicture_ = ui->editSelectedPic->text();
    showNoScale_ = ui->chkShowNoScale->isChecked();
    borderWidth_ = ui->spinBoxBorderWidth->value();
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
 * @brief EditBasicPropertyDialog::on_btnSelectPic_clicked
 * @details 单击选择图片按钮
 */
void EditBasicPropertyDialog::on_btnSelectPic_clicked()
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

int EditBasicPropertyDialog::borderWidth() const
{
    return borderWidth_;
}

void EditBasicPropertyDialog::setBorderWidth(int borderWidth)
{
    borderWidth_ = borderWidth;
    ui->spinBoxBorderWidth->setValue(borderWidth);
}

bool EditBasicPropertyDialog::showNoScale() const
{
    return showNoScale_;
}

void EditBasicPropertyDialog::setShowNoScale(bool showNoScale)
{
    showNoScale_ = showNoScale;
    ui->chkShowNoScale->setChecked(showNoScale);
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
