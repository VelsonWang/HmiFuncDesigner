#include "editbasicpropertydialog.h"
#include "ui_editbasicpropertydialog.h"
#include "inserttagdlg.h"
#include <QFileDialog>
#include <QStandardPaths>
#include <cfloat>

EditBasicPropertyDialog::EditBasicPropertyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditBasicPropertyDialog)
{
    ui->setupUi(this);
    szSelectedTag_ = "";
    jarShape_ = "";
    upperLimitValue_ = 0.0;
    lowerLimitValue_ = 0.0;
    maxValue_ = 0.0;
    scaleNum_ = 0;
    ui->editTagSelected->setReadOnly(true);

    ui->spinBoxUpper->setRange(DBL_MIN, DBL_MAX);
    ui->spinBoxUpper->setSingleStep(0.1);
    ui->spinBoxUpper->setDecimals(1);

    ui->spinBoxLower->setRange(DBL_MIN, DBL_MAX);
    ui->spinBoxLower->setSingleStep(0.1);
    ui->spinBoxLower->setDecimals(1);

    ui->spinBoxMax->setRange(DBL_MIN, DBL_MAX);
    ui->spinBoxMax->setSingleStep(0.1);
    ui->spinBoxMax->setDecimals(1);

    ui->spinBoxNum->setRange(0, 5000);
    ui->spinBoxNum->setSingleStep(1);

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
    jarShape_ = ui->editTitle->text();
    upperLimitValue_ = ui->spinBoxUpper->value();
    lowerLimitValue_ = ui->spinBoxLower->value();
    maxValue_ = ui->spinBoxMax->value();
    scaleNum_ = ui->spinBoxNum->value();
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


int EditBasicPropertyDialog::scaleNum() const
{
    return scaleNum_;
}

void EditBasicPropertyDialog::setScaleNum(int scaleNum)
{
    scaleNum_ = scaleNum;
    ui->spinBoxNum->setValue(scaleNum);
}

double EditBasicPropertyDialog::maxValue() const
{
    return maxValue_;
}

void EditBasicPropertyDialog::setMaxValue(double maxValue)
{
    maxValue_ = maxValue;
    ui->spinBoxMax->setValue(maxValue);
}

double EditBasicPropertyDialog::lowerLimitValue() const
{
    return lowerLimitValue_;
}

void EditBasicPropertyDialog::setLowerLimitValue(double lowerLimitValue)
{
    lowerLimitValue_ = lowerLimitValue;
    ui->spinBoxLower->setValue(lowerLimitValue);
}

double EditBasicPropertyDialog::upperLimitValue() const
{
    return upperLimitValue_;
}

void EditBasicPropertyDialog::setUpperLimitValue(double upperLimitValue)
{
    upperLimitValue_ = upperLimitValue;
    ui->spinBoxUpper->setValue(upperLimitValue);
}

QString EditBasicPropertyDialog::jarShape() const
{
    return jarShape_;
}

void EditBasicPropertyDialog::setJarShape(const QString &jarShape)
{
    jarShape_ = jarShape;
    ui->editTitle->setText(jarShape);
}

