#include "TagTmpEditDialog.h"
#include "ui_TagTmpEditDialog.h"


TagTmpEditDialog::TagTmpEditDialog(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::TagTmpEditDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() & (~Qt::WindowContextHelpButtonHint));
    ui->tabWidget->setCurrentIndex(0);

    QStringList listDataType;
    listDataType << tr("Bit1开关量")
                 << tr("Char8位有符号数")
                 << tr("Byte8位无符号数")
                 << tr("Short16位有符号数")
                 << tr("Word16位无符号数")
                 << tr("ASCII2个字符")
                 << tr("Long32位有符号数")
                 << tr("Dword32位无符号数")
                 << tr("Float单精度浮点数")
                 << tr("String字符串")
                 << tr("Double双精度浮点数")
                 << tr("BCD")
                 << tr("LongLong64位有符号数")
                 << tr("DwordDword64位无符号数");

    ui->cboDataType->addItems(listDataType);
    ui->cboDataType->setCurrentIndex(0);

    ui->spinBoxCreateTagNum->setRange(1, 32767);
    ui->spinBoxCreateTagNum->setValue(1);
}

TagTmpEditDialog::~TagTmpEditDialog()
{
    delete ui;
}


void TagTmpEditDialog::on_btnOk_clicked()
{
    m_iCreateTagNum = ui->spinBoxCreateTagNum->value();
    m_szTagName = ui->editTagName->text();
    m_szTagDescription = ui->editTagDescription->text();
    m_szTagDataType = ui->cboDataType->currentText();
    m_szTagInitValue = ui->editInitialValue->text();
    m_szTagMinValue = ui->editMinValue->text();
    m_szTagMaxValue = ui->editMaxValue->text();
    accept();
}

void TagTmpEditDialog::on_btnCancel_clicked()
{
    reject();
}

QString TagTmpEditDialog::tagMaxValue()
{
    return m_szTagMaxValue;
}

void TagTmpEditDialog::setTagMaxValue(const QString &maxValue)
{
    m_szTagMaxValue = maxValue;
    ui->editMaxValue->setText(maxValue);
}

QString TagTmpEditDialog::tagMinValue()
{
    return m_szTagMinValue;
}

void TagTmpEditDialog::setTagMinValue(const QString &minValue)
{
    m_szTagMinValue = minValue;
    ui->editMinValue->setText(minValue);
}

QString TagTmpEditDialog::tagInitValue()
{
    return m_szTagInitValue;
}

void TagTmpEditDialog::setTagInitValue(const QString &initValue)
{
    m_szTagInitValue = initValue;
    ui->editInitialValue->setText(initValue);
}

QString TagTmpEditDialog::tagDataType()
{
    return m_szTagDataType;
}

void TagTmpEditDialog::setTagDataType(const QString &dataType)
{
    m_szTagDataType = dataType;
    ui->cboDataType->setCurrentText(dataType);
}

QString TagTmpEditDialog::tagDescription()
{
    return m_szTagDescription;
}

void TagTmpEditDialog::setTagDescription(const QString &desc)
{
    m_szTagDescription = desc;
    ui->editTagDescription->setText(desc);
}

QString TagTmpEditDialog::tagName()
{
    return m_szTagName;
}

void TagTmpEditDialog::setTagName(const QString &name)
{
    m_szTagName = name;
    ui->editTagName->setText(name);
}

int TagTmpEditDialog::createTagNum()
{
    return m_iCreateTagNum;
}

void TagTmpEditDialog::setCreateTagNum(int num)
{
    m_iCreateTagNum = num;
    ui->spinBoxCreateTagNum->setValue(num);
}


void TagTmpEditDialog::hideCreateNumUI()
{
    ui->groupBoxCreateNum->hide();
}
