#include "TagEditDialog.h"
#include "ui_TagEditDialog.h"
#include <QMessageBox>


TagEditDialog::TagEditDialog(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::TagEditDialog) {
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() & (~Qt::WindowContextHelpButtonHint));
    m_mapAddrType.clear();
    m_mapAddrTypeToDataType.clear();
    ui->tabWidget->setCurrentIndex(0);
    ui->cboAddrType2->setEnabled(false);
    ui->editAddrOffset->setText("0");
    ui->editAddrOffset2->setEnabled(false);
    ui->editAddrOffset2->setText("0");
    ui->editTagName->setText("");
    ui->editTagUnit->setText("");
    ui->cboDataType->setCurrentIndex(-1);
    ui->cboAddrType->setCurrentIndex(-1);
    ui->cboReadWriteType->setCurrentIndex(-1);
    ui->editTagDesc->setPlainText("");

    QStringList szListDataType;
    szListDataType << tr("bool")
                   << tr("int8")
                   << tr("uint8")
                   << tr("int16")
                   << tr("uint16")
                   << tr("int32")
                   << tr("uint32")
                   << tr("float32")
                   << tr("double")
                   << tr("bcd16")
                   << tr("bcd32");
    ui->cboDataType->addItems(szListDataType);
    ui->cboDataType->setCurrentText(tr("uint8"));
}

TagEditDialog::~TagEditDialog()
{
    delete ui;
}

///
/// \brief TagEditDialog::setInfosAndTagObj
/// @details 设置变量信息
/// \param jsonTag 变量
///
void TagEditDialog::setTagObj(QJsonObject &jsonTag) {
    m_jsonTagObj = jsonTag;
}


///
/// \brief TagEditDialog::getTagObj
/// @details 返回变量信息
/// \return 变量信息
///
QJsonObject TagEditDialog::getTagObj() {
    return m_jsonTagObj;
}

///
/// \brief TagEditDialog::setAddrTypeAndDataType
/// @details 设置地址类型和数据类型
/// \param mapAddrType 地址类型1=地址类型2x,地址类型2y,地址类型2z
/// \param mapDataType 地址类型=数据类型,数据类型,数据类型
///
void TagEditDialog::setAddrTypeAndDataType(QMap<QString, QStringList> mapAddrType, QMap<QString, QStringList> mapDataType) {
    m_mapAddrType = mapAddrType;
    m_mapAddrTypeToDataType = mapDataType;
}


///
/// \brief TagEditDialog::setAddrTypeLimit
/// \details 设置地址类型的限制范围
/// \param mapLimit
///
void TagEditDialog::setAddrTypeLimit(QMap<QString, QMap<QString, quint32>> mapLimit) {
    m_mapAddrTypeToLimit = mapLimit;
}


///
/// \brief TagEditDialog::on_btnOk_clicked
/// \details 单击确定按钮
///
void TagEditDialog::on_btnOk_clicked() {
    bool bOk = false;
    float fAddrOffset = ui->editAddrOffset->text().toFloat(&bOk);
    if(!bOk) {
        QMessageBox::critical(this, tr("错误"), tr("地址偏移设置错误!"));
        return;
    }

    bOk = false;
    fAddrOffset = ui->editAddrOffset2->text().toFloat(&bOk);
    if(!bOk) {
        QMessageBox::critical(this, tr("错误"), tr("地址偏移2设置错误!"));
        return;
    }

    QString szTagName = ui->editTagName->text();
    if(szTagName == "") {
        QMessageBox::critical(this, tr("错误"), tr("变量名称不能为空!"));
        return;
    }

    quint32 dwLowerLimit = 0;
    quint32 dwUpperLimit = 0;
    QString szAreaName = ui->cboAddrType->currentText();
    QMap<QString, quint32> mapLimit = m_mapAddrTypeToLimit[szAreaName];
    if(!mapLimit.isEmpty()) {
        dwLowerLimit = mapLimit["MIN"];
        dwUpperLimit = mapLimit["MAX"];
        quint32 iAddrOffset = ui->editAddrOffset->text().toUInt();
        if(iAddrOffset < dwLowerLimit || iAddrOffset > dwUpperLimit) {
            QMessageBox::warning(this,
                                 tr("提示"),
                                 QString(tr("地址偏移不在范围[%1,%2]"))
                                 .arg(QString::number(dwLowerLimit))
                                 .arg(QString::number(dwUpperLimit)));
            return;
        }
    }

    if(ui->editTagName->text() == "") {
        QString szErrorInfo = QString(QObject::tr("变量名称不能为空！"));
        QMessageBox::critical(this, QObject::tr("错误"), szErrorInfo);
        return;
    }

    if(ui->cboAddrType->count() > 0 && ui->cboAddrType->currentText() == "") {
        QString szErrorInfo = QString(QObject::tr("地址类型错误"));
        QMessageBox::critical(this, QObject::tr("错误"), szErrorInfo);
        return;
    }

    if(ui->cboDataType->currentText() == "") {
        QString szErrorInfo = QString(QObject::tr("数据类型错误"));
        QMessageBox::critical(this, QObject::tr("错误"), szErrorInfo);
        return;
    }

    if(ui->cboReadWriteType->currentIndex() == -1) {
        QString szErrorInfo = QString(QObject::tr("读写类型错误"));
        QMessageBox::critical(this, QObject::tr("错误"), szErrorInfo);
        return;
    }

    m_jsonTagObj["name"] = ui->editTagName->text();
    m_jsonTagObj["addr_type"] = ui->cboAddrType->currentText();
    m_jsonTagObj["addr_offset"] = ui->editAddrOffset->text();
    m_jsonTagObj["addr_type2"] = ui->cboAddrType2->currentText();
    m_jsonTagObj["addr_offset2"] = ui->editAddrOffset2->text();
    m_jsonTagObj["data_type"] = ui->cboDataType->currentText();
    m_jsonTagObj["unit"] = ui->editTagUnit->text();
    m_jsonTagObj["writeable"] = ui->cboReadWriteType->currentText();
    m_jsonTagObj["remark"] = ui->editTagDesc->toPlainText();

    this->accept();
}

///
/// \brief TagEditDialog::on_btnCancel_clicked
/// \details 单击取消按钮
///
void TagEditDialog::on_btnCancel_clicked()
{
    this->reject();
}

///
/// \brief TagEditDialog::on_cboAddrType_currentTextChanged
/// \details 地址类型改变
/// \param szAddrType
///
void TagEditDialog::on_cboAddrType_currentTextChanged(const QString &szAddrType)
{  
    QStringList listSubAddrType;
    listSubAddrType.clear();

    listSubAddrType = m_mapAddrType[szAddrType];
    ui->cboAddrType2->clear();
    ui->cboAddrType2->addItems(listSubAddrType);
    //ui->cboAddrType2->setCurrentIndex(-1);

    ui->cboAddrType2->setEnabled((listSubAddrType.size() > 0));
    ui->editAddrOffset2->setEnabled((listSubAddrType.size() > 0));

    if(listSubAddrType.size() < 1) {
        ui->cboDataType->clear();
        ui->cboDataType->addItems(m_mapAddrTypeToDataType[szAddrType]);
    }
}

void TagEditDialog::on_cboAddrType2_currentTextChanged(const QString &szAddrType)
{
    ui->cboDataType->clear();
    ui->cboDataType->addItems(m_mapAddrTypeToDataType[szAddrType]);
}

///
/// \brief TagEditDialog::updateUI
/// \details 更新UI
///
void TagEditDialog::updateUI()
{
    ui->tabWidget->setCurrentIndex(0);
    ui->cboAddrType2->setEnabled(false);
    ui->editAddrOffset2->setEnabled(false);

    QStringList listAddrType = m_mapAddrType.keys();
    ui->cboAddrType->clear();
    ui->cboAddrType->addItems(listAddrType);
    ui->cboAddrType->setCurrentIndex(-1);
    ui->cboAddrType2->clear();
    ui->cboAddrType2->addItems(listAddrType);
    ui->cboAddrType2->setCurrentIndex(-1);
    ui->cboReadWriteType->setCurrentIndex(1);

    if(!m_jsonTagObj.isEmpty()) {
        ui->editTagName->setText(m_jsonTagObj["name"].toString());
        QString szAddrType = m_jsonTagObj["addr_type"].toString();
        if(szAddrType != "") {
            ui->cboAddrType->setCurrentText(szAddrType);
            on_cboAddrType_currentTextChanged(szAddrType);
        }

        ui->editAddrOffset->setText(m_jsonTagObj["addr_offset"].toString());

        QString szAddrType2 = m_jsonTagObj["addr_type2"].toString();
        ui->cboAddrType2->setCurrentText(szAddrType2);

        ui->editAddrOffset2->setText(m_jsonTagObj["addr_offset2"].toString());

        QString szDataType = m_jsonTagObj["data_type"].toString();
        if(szDataType != "") {
            ui->cboDataType->setCurrentText(szDataType);
        }
        ui->cboReadWriteType->setCurrentIndex(m_jsonTagObj["writeable"].toInt());
        ui->editTagDesc->setPlainText(m_jsonTagObj["remark"].toString());

        ui->editTagUnit->setText(m_jsonTagObj["unit"].toString());
    }
}







