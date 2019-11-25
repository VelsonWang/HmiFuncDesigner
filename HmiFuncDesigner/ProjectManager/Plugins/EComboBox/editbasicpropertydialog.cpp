#include "editbasicpropertydialog.h"
#include "ui_editbasicpropertydialog.h"
#include "inserttagdlg.h"
#include "tagtextlisteditordialog.h"
#include <QColorDialog>
#include <cfloat>

EditBasicPropertyDialog::EditBasicPropertyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditBasicPropertyDialog)
{
    ui->setupUi(this);
    szTagSelected_ = "";
    tagTextList_.clear();
    szTagSelected_ = "";
    elementText_ = "";
    transparentBackground_ = true;
    szHAlign_ = "";
    szVAlign_ = "";
    ui->editTagSelected->setReadOnly(true);
    ui->editTagTextColorList->setReadOnly(true);
    ui->cboHAlign->clear();
    ui->cboHAlign->addItem(tr("左对齐"));
    ui->cboHAlign->addItem(tr("居中对齐"));
    ui->cboHAlign->addItem(tr("右对齐"));
    ui->cboVAlign->clear();
    ui->cboVAlign->addItem(tr("上对齐"));
    ui->cboVAlign->addItem(tr("居中对齐"));
    ui->cboVAlign->addItem(tr("下对齐"));
    borderWidth_ = 1;
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
    tagTextList_ = ui->editTagTextColorList->text().split('|');;
    transparentBackground_ = ui->chkTransparentBackground->isChecked();
    elementText_ = ui->editText->toPlainText();
    szHAlign_ = ui->cboHAlign->currentText();
    szVAlign_ = ui->cboVAlign->currentText();
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


/**
 * @brief EditBasicPropertyDialog::on_btnTagTextColorList_clicked
 * @details 单击变量文本颜色列表按钮
 */
void EditBasicPropertyDialog::on_btnTagTextColorList_clicked()
{
    TagTextListEditorDialog dlg;
    dlg.setValueTextList(tagTextList_);


    if(dlg.exec() == QDialog::Accepted) {
        QStringList list = dlg.getValueTextList();
        if (list != tagTextList_) {
            this->setTagTextList(list);
        }
    }
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
 * @brief EditBasicPropertyDialog::on_btnTextColor_clicked
 * @details 单击文本颜色按钮
 */
void EditBasicPropertyDialog::on_btnTextColor_clicked()
{
    QColor color = QColorDialog::getColor(textColor_, this);
    this->setTextColor(color);
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

bool EditBasicPropertyDialog::transparentBackground() const
{
    return transparentBackground_;
}

void EditBasicPropertyDialog::setTransparentBackground(bool transparentBackground)
{
    transparentBackground_ = transparentBackground;
    ui->chkTransparentBackground->setChecked(transparentBackground);
}

QColor EditBasicPropertyDialog::backgroundColor() const
{
    return backgroundColor_;
}

void EditBasicPropertyDialog::setBackgroundColor(const QColor &backgroundColor)
{
    backgroundColor_ = backgroundColor;
    ui->labelBackgroundColor->setStyleSheet(QString("background:%1").arg(backgroundColor.name()));
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

QString EditBasicPropertyDialog::elementText() const
{
    return elementText_;
}

void EditBasicPropertyDialog::setElementText(const QString &elementText)
{
    elementText_ = elementText;
    ui->editText->setText(elementText);
}

QStringList EditBasicPropertyDialog::tagTextList() const
{
    return tagTextList_;
}

void EditBasicPropertyDialog::setTagTextList(const QStringList &tagTextList)
{
    tagTextList_ = tagTextList;
    ui->editTagTextColorList->setText(tagTextList.join("|"));
}
