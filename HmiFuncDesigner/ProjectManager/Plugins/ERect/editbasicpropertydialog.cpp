#include "editbasicpropertydialog.h"
#include "ui_editbasicpropertydialog.h"
#include "inserttagdlg.h"
#include "tagcolorlisteditordialog.h"
#include <QColorDialog>
#include <cfloat>

EditBasicPropertyDialog::EditBasicPropertyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditBasicPropertyDialog)
{
    ui->setupUi(this);
    szTagSelected_ = "";
    tagColorList_.clear();
    isFill_ = false;
    borderWidth_ = 1;
    ui->editTagSelected->setReadOnly(true);
    ui->editTagColorList->setReadOnly(true);
    ui->spinBoxBorderWidth->setRange(0, 5000);
    ui->spinBoxBorderWidth->setSingleStep(1);
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
    isFill_ = ui->chkFillColor->isChecked();
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
 * @brief EditBasicPropertyDialog::on_btnTagColorList_clicked
 * @details 单击填充颜色列表按钮
 */
void EditBasicPropertyDialog::on_btnTagColorList_clicked()
{
    TagColorListEditorDialog dlg;
    dlg.setValueColorList(tagColorList_);
    if(dlg.exec() == QDialog::Accepted) {
        QStringList list = dlg.getValueColorList();
        setTagColorList(list);
    }
}

/**
 * @brief EditBasicPropertyDialog::on_btnFillColor_clicked
 * @details 单击填充颜色按钮
 */
void EditBasicPropertyDialog::on_btnFillColor_clicked()
{
    QColor color = QColorDialog::getColor(fillColor_, this);
    setFillColor(color);
}


/**
 * @brief EditBasicPropertyDialog::on_btnBorderColor_clicked
 * @details 单击边框颜色按钮
 */
void EditBasicPropertyDialog::on_btnBorderColor_clicked()
{
    QColor color = QColorDialog::getColor(borderColor_, this);
    setBorderColor(color);
}

QColor EditBasicPropertyDialog::borderColor() const
{
    return borderColor_;
}

void EditBasicPropertyDialog::setBorderColor(const QColor &borderColor)
{
    borderColor_ = borderColor;
    ui->labelBorderColor->setStyleSheet(QString("background:%1").arg(borderColor_.name()));
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

bool EditBasicPropertyDialog::isFill() const
{
    return isFill_;
}

void EditBasicPropertyDialog::setIsFill(bool isFill)
{
    isFill_ = isFill;
    ui->chkFillColor->setChecked(isFill);
}

QColor EditBasicPropertyDialog::fillColor() const
{
    return fillColor_;
}

void EditBasicPropertyDialog::setFillColor(const QColor &fillColor)
{
    fillColor_ = fillColor;
    ui->labelFillColor->setStyleSheet(QString("background:%1").arg(fillColor_.name()));
}

QStringList EditBasicPropertyDialog::tagColorList() const
{
    return tagColorList_;
}

void EditBasicPropertyDialog::setTagColorList(const QStringList &tagColorList)
{
    tagColorList_ = tagColorList;
    ui->editTagColorList->setText(tagColorList.join('|'));
}


