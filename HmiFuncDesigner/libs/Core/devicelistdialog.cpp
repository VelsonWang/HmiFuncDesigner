#include "devicelistdialog.h"
#include "ui_devicelistdialog.h"
#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <QModelIndex>
#include <QSettings>
#include <QStandardItem>
#include <QStandardItemModel>
#include "../shared/confighelper.h"


DeviceListDialog::DeviceListDialog(QString stype, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DeviceListDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() & (~Qt::WindowContextHelpButtonHint));
    m_deviceName = "";
    m_portType = stype;
    // qDebug()<<"PortType: "<<m_strPortType;
    TreeViewInit();
}

DeviceListDialog::~DeviceListDialog()
{
    delete ui;
}

void DeviceListDialog::TreeViewInit()
{
    ui->treeView->setHeaderHidden(true);
    m_treeViewItemModel = new QStandardItemModel();

    QStandardItem *pDeviceListItem = new QStandardItem(tr("设备列表"));
    pDeviceListItem->setEditable(false);

    QString iniFileName = QApplication::applicationDirPath() + "/Config/communication_device.ini";
    // qDebug()<< "path " << iniFileName;
    QFile fileCfg(iniFileName);
    if (!fileCfg.exists()) {
        QMessageBox::critical(this, tr("提示"), tr("设备列表选型配置文件不存在！"));
    }

    m_supportDevList.clear();
    // device type
    QStringList sDevTypeList;
    ConfigHelper::getCfgList(iniFileName, "DeviceSupportList", "list", sDevTypeList);
    for (int i = 0; i < sDevTypeList.count(); i++) {
        QString sDevTypeOne = sDevTypeList.at(i).trimmed();
        QString key = sDevTypeOne.left(sDevTypeOne.indexOf("-"));
        QString nameTmp =
            sDevTypeOne.right(sDevTypeOne.length() - sDevTypeOne.indexOf("-") - 1);
        QString name = QString::fromLocal8Bit(nameTmp.toLatin1());
        // qDebug()<< key << "  " << name;
        QStandardItem *itemDevTypeOne = new QStandardItem(name);
        itemDevTypeOne->setEditable(false);

        // device series
        QStringList devSeriesList;
        ConfigHelper::getCfgList(iniFileName, key, "list", devSeriesList);
        for (int i = 0; i < devSeriesList.count(); i++) {
            QString sDevSeriesOne = devSeriesList.at(i).trimmed();
            QStandardItem *itemDevSeriesOne = new QStandardItem(sDevSeriesOne);
            itemDevSeriesOne->setEditable(false);

            // device
            QStringList devList;
            ConfigHelper::getCfgList(iniFileName, sDevSeriesOne, "list", devList);
            for (int i = 0; i < devList.count(); i++) {
                QString sDevOne = devList.at(i).trimmed();

                QString keyDev = sDevOne.left(sDevOne.indexOf("-"));
                QString nameTmpDev =
                    sDevOne.right(sDevOne.length() - sDevOne.indexOf("-") - 1);
                QString nameDev = QString::fromLocal8Bit(nameTmpDev.toLatin1());
                // qDebug()<< keyDev << "  " << nameDev;

                if (m_portType != nameDev) {
                    continue;
                }

                QStandardItem *itemDev = new QStandardItem(keyDev);
                itemDev->setEditable(false);
                m_supportDevList.append(keyDev);
                // qDebug()<<sDevOne;
                itemDevSeriesOne->appendRow(itemDev);
            }
            if (itemDevSeriesOne->rowCount()) {
                itemDevTypeOne->appendRow(itemDevSeriesOne);
            } else {
                delete itemDevSeriesOne;
            }
        }
        pDeviceListItem->appendRow(itemDevTypeOne);
    }

    m_treeViewItemModel->appendRow(pDeviceListItem);
    ui->treeView->setModel(m_treeViewItemModel);
    ui->treeView->expandAll();
}

// 取得设备名称
QString DeviceListDialog::GetDeviceName() const
{
    return m_deviceName;
}

void DeviceListDialog::on_btnOK_clicked()
{
    QModelIndex modIndex = ui->treeView->currentIndex();
    QStandardItem *item = m_treeViewItemModel->itemFromIndex(modIndex);
    if (item == NULL) {
        QMessageBox::critical(this, tr("提示"), tr("未选设备！"));
        return;
    }
    bool found = false;
    for (int i = 0; i < m_supportDevList.count(); i++) {
        if (m_supportDevList.at(i) == item->text()) {
            m_deviceName = item->text();
            found = true;
        }
    }
    if (!found) {
        QMessageBox::critical(this, tr("提示"), tr("设备错误！"));
    } else {
        accept();
    }
}

void DeviceListDialog::on_btnCancel_clicked()
{
    m_deviceName = "";
    reject();
}
