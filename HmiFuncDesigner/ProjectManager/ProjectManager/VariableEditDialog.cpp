#include "VariableEditDialog.h"
#include "ui_VariableEditDialog.h"
#include "ConfigUtils.h"
#include "Helper.h"
#include "IDevicePlugin.h"
#include "ProjectMgrUtils.h"
#include "ProjectData.h"
#include <QStringList>
#include <QMessageBox>
#include <QDir>
#include <QFile>
#include <QPluginLoader>
#include <QDebug>

VariableEditDialog::VariableEditDialog(QString projName, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VariableEditDialog),
    m_strProjectName(projName),
    devPlugin_(nullptr)
{
    ui->setupUi(this);
    ui->tabWidget->setCurrentIndex(0);
    on_checkIsAlarm_clicked(false);
    on_checkIsSaveDisk_clicked(false);
    on_cboDataType_currentIndexChanged(ui->cboDataType->currentText());

    // 已经建立设备列表
    DeviceInfo &deviceInfo = ProjectData::getInstance()->deviceInfo_;
    deviceInfo.load(ProjectData::getInstance()->dbData_);
    for(int i=0; i<deviceInfo.listDeviceInfoObject_.count(); i++) {
        DeviceInfoObject *pObj = deviceInfo.listDeviceInfoObject_.at(i);
        ui->cboDeviceName->addItem(pObj->szDeviceName_);
    }
    if(ui->cboDeviceName->count()>0)
        ui->cboDeviceName->setCurrentIndex(0);

    ui->tabBaseLayout->addStretch();
    ui->tabDataAttributeLayout->addStretch();
    ui->tabIOConnectLayout->addStretch();

    m_iRegAddrOffset = 0;
}

VariableEditDialog::~VariableEditDialog()
{
    delete ui;
}

void VariableEditDialog::DialogFixedSize()
{
    ui->tabBaseLayout->setSizeConstraint(QLayout::SetFixedSize);
    ui->tabDataAttributeLayout->setSizeConstraint(QLayout::SetFixedSize);
    ui->tabIOConnectLayout->setSizeConstraint(QLayout::SetFixedSize);
    ui->VariableEditDialogLayout->setSizeConstraint(QLayout::SetFixedSize);
}


QString VariableEditDialog::GetDataType()
{
    return ui->cboDataType->currentText();
}


QString VariableEditDialog::GetName()
{
    return ui->editVariableName->text();
}


QString VariableEditDialog::GetDescription()
{
    return ui->editDescription->text();
}


QString VariableEditDialog::GetUnit()
{
    return ui->editUnit->text();
}


QString VariableEditDialog::GetBatchNum()
{
    return ui->editBatchNum->text();
}


QString VariableEditDialog::GetDataRegisterSpace()
{
    return ui->editRegisterSpace->text();
}


void VariableEditDialog::on_btnOk_clicked()
{
    bool ok = false;
    int iRegAddr = ui->editRegisterAddress->text().toInt(&ok);
    if(!ok)
        iRegAddr = -1;

    ok = false;
    int iOffset = ui->editAddressOffset->text().toInt(&ok);
    if(!ok)
        iOffset = -1;

    quint32 lowerLimit;
    quint32 upperLimit;
    QString area = ui->cboRegisterSection->currentText();
    if(devPlugin_ != nullptr) {
        devPlugin_->GetRegisterAreaLimit(area, lowerLimit, upperLimit);
        if((iRegAddr + iOffset) < lowerLimit || (iRegAddr + iOffset) > upperLimit) {
            QMessageBox::warning(this,
                                 tr("提示"),
                                 QString(tr("地址设置不在范围[%1,%2]"))
                                 .arg(QString::number(lowerLimit))
                                 .arg(QString::number(upperLimit)));
            return;
        }

    }
    accept();
}

void VariableEditDialog::on_btnCancel_clicked()
{
    reject();
}

void VariableEditDialog::SetCurrentTabIndex(int index)
{
    ui->tabWidget->setCurrentIndex(index);
    DialogFixedSize();
}

void VariableEditDialog::RemoveTab(int index)
{
    ui->tabWidget->removeTab(index);
}

/*
 * QStringList.at(0)--DataType
 * QStringList.at(1)--Name
 * QStringList.at(2)--Description
 * QStringList.at(3)--Unit
 * 取得基本设置
 */
QStringList VariableEditDialog::GetBasicSetting()
{
    QStringList sl;
    sl << ui->cboDataType->currentText();
    sl << ui->editVariableName->text();
    sl << ui->editDescription->text();
    sl << ui->editUnit->text();
    return sl;
}

/*
 * QStringList.at(0)--DataType
 * QStringList.at(1)--Name
 * QStringList.at(2)--Description
 * QStringList.at(3)--Unit
 * 设置基本设置
 */
void VariableEditDialog::SetBasicSetting(QStringList sl)
{
    if(sl.count()<4)
        return;
    ui->cboDataType->setCurrentText(sl.at(0));
    ui->editVariableName->setText(sl.at(1));
    ui->editDescription->setText(sl.at(2));
    ui->editUnit->setText(sl.at(3));
    DialogFixedSize();
}

/*
 *  取得数据属性字符
 */
QString VariableEditDialog::GetDataAttribuyeString()
{
    QString s = "";
    s += ui->cboAttribute->currentText() + ",";
    s += ui->editMaxValue->text() + ",";
    s += ui->editMinValue->text() + ",";
    s += ui->editInitialValue->text() + ",";
    s += ui->editScaleValue->text();
    return s;
}

/*
 *  设置数据属性字符
 */
void VariableEditDialog::SetDataAttribuyeString(QString s)
{
    QStringList list;
    list = s.split(QRegExp(","));
    if(list.size() == 5)
    {
        ui->cboAttribute->setCurrentText(list.at(0));
        ui->editMaxValue->setText(list.at(1));
        ui->editMinValue->setText(list.at(2));
        ui->editInitialValue->setText(list.at(3));
        ui->editScaleValue->setText(list.at(4));
    }
    DialogFixedSize();
}

/*
 *  取得报警字符
 */
QString VariableEditDialog::GetAlarmString()
{
    QString s = "";
    return s;
}

/*
 * 设置报警字符
 */
void VariableEditDialog::SetAlarmString(QString s)
{
}

/*
 *取得存盘字符
 */
QString VariableEditDialog::GetSaveDiskString()
{
    QString s = "";
    return s;
}

/*
 *设置存盘字符
 */
void VariableEditDialog::SetSaveDiskString(QString s)
{
}


/*
 * 是否告警
 */
void VariableEditDialog::on_checkIsAlarm_clicked(bool checked)
{

}

/*
 * 是否存盘
 */
void VariableEditDialog::on_checkIsSaveDisk_clicked(bool checked)
{

}

/*
 * 获取变量类型
 */
int VariableEditDialog::GetVariableType()
{
    return static_cast<int>(m_iVarType);
}

/*
 * 设置变量类型
 */
void VariableEditDialog::SetVariableType(VAR_TYPE t)
{
    m_iVarType = t;

    DialogFixedSize();
}


void VariableEditDialog::on_cboDataType_currentIndexChanged(const QString &s)
{
    if(s == "数字量")
    {
        SetVariableType(DI);
    }
    if(s == "模拟量")
    {
        SetVariableType(AI);
    }
}

/*
* 取得IO连接字符
*/
QString VariableEditDialog::GetIOConnectString()
{
    QString s = "";
    if(ui->chkIsIOConnect->isChecked())
    {
        if(ui->chkIsIOConnect->isChecked())
            s += "1,";
        else
            s += "0,";
        if(ui->chkIsSelfVariable->isChecked())
            s += "1,";
        else
            s += "0,";
        if(ui->chkGlobal->isChecked())
            s += "1,";
        else
            s += "0,";

        s += ui->cboDeviceName->currentText() + ",";
        s += ui->cboTypeReadWrite->currentText() + ",";

        s += ui->editDeviceAddress->text() + ",";
        s += ui->editIOMaxValue->text() + ",";

        s += ui->cboRegisterSection->currentText() + ",";
        s += ui->editIOMinValue->text() + ",";

        int iRegAddr = ui->editRegisterAddress->text().toInt();

        s += QString("%1").arg(m_iRegAddrOffset + iRegAddr) + ",";
        s += ui->editIOInitValue->text() + ",";

        s += ui->cboIODataType->currentText() + ",";
        s += ui->editIOScale->text() + ",";

        s += ui->editAddressOffset->text() + ",";
        s += ui->editAddressInFrame->text();
    }
    return s;

}


/*
* 设置IO连接字符
*/
void VariableEditDialog::SetIOConnectString(QString s)
{
    QStringList list;
    list = s.split(QRegExp(","));
//    qDebug()<< s;
//    qDebug()<< list.size();
    if(list.size() == 15)
    {
        if(list.at(0) == "1")
            ui->chkIsIOConnect->setChecked(true);
        else
            ui->chkIsIOConnect->setChecked(false);
        on_chkIsIOConnect_clicked(ui->chkIsIOConnect->isChecked());

        if(list.at(1) == "1")
            ui->chkIsSelfVariable->setChecked(true);
        else
            ui->chkIsSelfVariable->setChecked(false);

        if(list.at(2) == "1")
            ui->chkGlobal->setChecked(true);
        else
            ui->chkGlobal->setChecked(false);

        ui->cboDeviceName->setCurrentText(list.at(3));
        ui->cboTypeReadWrite->setCurrentText(list.at(4));

        ui->editDeviceAddress->setText(list.at(5));
        ui->editIOMaxValue->setText(list.at(6));

        ui->cboRegisterSection->setCurrentText(list.at(7));
        ui->editIOMinValue->setText(list.at(8));

        ui->editRegisterAddress->setText(list.at(9));
        ui->editIOInitValue->setText(list.at(10));

        ui->cboIODataType->setCurrentText(list.at(11));
        ui->editIOScale->setText(list.at(12));

        ui->editAddressOffset->setText(list.at(13));
        ui->editAddressInFrame->setText(list.at(14));
    }
    DialogFixedSize();
}


void VariableEditDialog::on_chkIsIOConnect_clicked(bool checked)
{
    ui->cboDeviceName->setEnabled(checked);
    ui->cboTypeReadWrite->setEnabled(checked);
    ui->editDeviceAddress->setEnabled(checked);
    ui->editIOMaxValue->setEnabled(checked);
    ui->cboRegisterSection->setEnabled(checked);
    ui->editIOMinValue->setEnabled(checked);

    ui->editRegisterAddress->setEnabled(checked);
    ui->editIOInitValue->setEnabled(checked);
    ui->cboIODataType->setEnabled(checked);
    ui->editIOScale->setEnabled(checked);
    ui->editAddressOffset->setEnabled(checked);
    ui->editAddressInFrame->setEnabled(checked);

}

void VariableEditDialog::on_chkIsSelfVariable_clicked(bool checked)
{
    Q_UNUSED(checked)
}

void VariableEditDialog::on_chkGlobal_clicked(bool checked)
{
    Q_UNUSED(checked)
}


/**
 * @brief VariableEditDialog::on_cboDeviceName_currentTextChanged
 * @details 设备名称改变
 * @param deviceName 设备名称
 */
void VariableEditDialog::on_cboDeviceName_currentTextChanged(const QString &deviceName) {
    QString pluginNmae = deviceName;
    if(pluginNmae.indexOf("_") >= 0)
        pluginNmae = pluginNmae.left(pluginNmae.indexOf("_"));

    QDir pluginsDir(Helper::AppDir());
    pluginsDir.cdUp();
    pluginsDir.cd("deviceplugins");
    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
#ifdef Q_OS_WIN
        QString szFileEndWith = QString(".dll");
#endif

#ifdef Q_OS_LINUX
        QString szFileEndWith = QString(".so");
#endif
        if(fileName.indexOf(pluginNmae) != -1 && fileName.endsWith(szFileEndWith)) {
            QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
            QObject *plugin = pluginLoader.instance();
            if (plugin) {
                IDevicePlugin *iDevPlugin = qobject_cast<IDevicePlugin *>(plugin);
                devPlugin_ = iDevPlugin;
                if (iDevPlugin) {
                    // 获取设备支持的所有寄存器区
                    QStringList listReg = iDevPlugin->GetDeviceSupportRegisterArea();
                    ui->cboRegisterSection->clear();
                    ui->cboRegisterSection->addItems(listReg);

                    // 获取设备支持的所有数据类型
                    QStringList listType = iDevPlugin->GetDeviceSupportDataType();
                    ui->cboIODataType->clear();
                    ui->cboIODataType->addItems(listType);
                } else {
                    QMessageBox::critical(this,
                                          tr("系统错误"),
                                          tr("插件加载失败！\n") + fileName);
                }
            }
        }
    }
}


void VariableEditDialog::SetBatchOffset(int offset)
{
    m_iRegAddrOffset = offset;
}
