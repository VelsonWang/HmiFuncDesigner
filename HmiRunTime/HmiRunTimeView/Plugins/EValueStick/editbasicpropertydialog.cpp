#include "editbasicpropertydialog.h"
#include "ui_editbasicpropertydialog.h"
#include "inserttagdlg.h"
#include <QColorDialog>
#include <cfloat>

EditBasicPropertyDialog::EditBasicPropertyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditBasicPropertyDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() & (~Qt::WindowContextHelpButtonHint));
    szTagSelected_ = "";
    maxValue_ = 0;
    minValue_ = 0;
    scaleNum_ = 10;
    scaleDir_ = "";
    scalePos_ = "";
    ui->editTagSelected->setReadOnly(true);
    ui->spinBoxMax->setRange(DBL_MIN, DBL_MAX);
    ui->spinBoxMax->setSingleStep(0.1);
    ui->spinBoxMax->setDecimals(1);
    ui->spinBoxMin->setRange(DBL_MIN, DBL_MAX);
    ui->spinBoxMin->setSingleStep(0.1);
    ui->spinBoxMin->setDecimals(1);
    ui->spinBoxNum->setRange(0, 5000);
    ui->spinBoxNum->setSingleStep(1);
    ui->cboScaleDir->clear();
    ui->cboScaleDir->addItem(tr("从左到右"));
    ui->cboScaleDir->addItem(tr("从右到左"));
    ui->cboScaleDir->addItem(tr("从上到下"));
    ui->cboScaleDir->addItem(tr("从下到上"));
    ui->cboScalePos->clear();
    ui->cboScalePos->addItem(tr("右下方"));
    ui->cboScalePos->addItem(tr("左上方"));
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
    szTagSelected_ = ui->editTagSelected->text();
    maxValue_ = ui->spinBoxMax->value();
    minValue_ = ui->spinBoxMin->value();
    scaleNum_ = ui->spinBoxNum->value();
    scaleDir_ = ui->cboScaleDir->currentText();
    scalePos_ = ui->cboScalePos->currentText();
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
    return szTagSelected_;
}

void EditBasicPropertyDialog::setSelectedTag(const QString &szSelectedTag)
{
    szTagSelected_ = szSelectedTag;
    ui->editTagSelected->setText(szSelectedTag);
}


/**
 * @brief EditBasicPropertyDialog::on_btnBackgroundColor_clicked
 * @details 单击背景颜色按钮
 */
void EditBasicPropertyDialog::on_btnBackgroundColor_clicked()
{
    QColor color = QColorDialog::getColor(backgroundColor_, this);
    this->setBackgroundColor(color);
}

/**
 * @brief EditBasicPropertyDialog::on_btnForeground_clicked
 * @details 单击前景颜色按钮
 */
void EditBasicPropertyDialog::on_btnForeground_clicked()
{
    QColor color = QColorDialog::getColor(foregroundColor_, this);
    this->setForegroundColor(color);
}

/**
 * @brief EditBasicPropertyDialog::on_btnScaleColor_clicked
 * @details 单击标尺颜色按钮
 */
void EditBasicPropertyDialog::on_btnScaleColor_clicked()
{
    QColor color = QColorDialog::getColor(scaleColor_, this);
    this->setScaleColor(color);
}

/**
 * @brief EditBasicPropertyDialog::on_btnTextColor_clicked
 * @details 单击文本颜色按钮
 */
void EditBasicPropertyDialog::on_btnTextColor_clicked()
{
    QColor color = QColorDialog::getColor(textColor_, this);
    this->setTextColor(color);
}

QColor EditBasicPropertyDialog::textColor() const
{
    return textColor_;
}

void EditBasicPropertyDialog::setTextColor(const QColor &textColor)
{
    textColor_ = textColor;
    ui->labelTextColor->setStyleSheet(QString("background:%1").arg(textColor.name()));
}

QString EditBasicPropertyDialog::scalePos() const
{
    return scalePos_;
}

void EditBasicPropertyDialog::setScalePos(const QString &scalePos)
{
    scalePos_ = scalePos;
    ui->cboScalePos->setCurrentText(scalePos);
}

QString EditBasicPropertyDialog::scaleDir() const
{
    return scaleDir_;
}

void EditBasicPropertyDialog::setScaleDir(const QString &scaleDir)
{
    scaleDir_ = scaleDir;
    ui->cboScaleDir->setCurrentText(scaleDir);
}

QColor EditBasicPropertyDialog::scaleColor() const
{
    return scaleColor_;
}

void EditBasicPropertyDialog::setScaleColor(const QColor &scaleColor)
{
    scaleColor_ = scaleColor;
    ui->labelScaleColor->setStyleSheet(QString("background:%1").arg(scaleColor.name()));
}

QColor EditBasicPropertyDialog::foregroundColor() const
{
    return foregroundColor_;
}

void EditBasicPropertyDialog::setForegroundColor(const QColor &foregroundColor)
{
    foregroundColor_ = foregroundColor;
    ui->labelForeground->setStyleSheet(QString("background:%1").arg(foregroundColor.name()));
}

QColor EditBasicPropertyDialog::backgroundColor() const
{
    return backgroundColor_;
}

void EditBasicPropertyDialog::setBackgroundColor(const QColor &backgroundColor)
{
    backgroundColor_ = backgroundColor;
    ui->labelForeground->setStyleSheet(QString("background:%1").arg(backgroundColor.name()));
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


double EditBasicPropertyDialog::minValue() const
{
    return minValue_;
}

void EditBasicPropertyDialog::setMinValue(double minValue)
{
    minValue_ = minValue;
    ui->spinBoxMin->setValue(minValue);
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


