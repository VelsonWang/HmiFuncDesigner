#include "DeviceListDialog.h"
#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <QModelIndex>
#include <QSettings>
#include <QStandardItem>
#include <QStandardItemModel>
#include "ConfigUtils.h"
#include "Helper.h"
#include "ui_DeviceListDialog.h"

DeviceListDialog::DeviceListDialog(QString stype, QWidget *parent)
    : QDialog(parent), ui(new Ui::DeviceListDialog)
{
  ui->setupUi(this);
  this->setWindowFlags(this->windowFlags() & (~Qt::WindowContextHelpButtonHint));
  m_DeviceName = "";
  m_strPortType = stype;
  // qDebug()<<"PortType: "<<m_strPortType;
  TreeViewInit();
}

DeviceListDialog::~DeviceListDialog() { delete ui; }

void DeviceListDialog::TreeViewInit() {
  ui->treeView->setHeaderHidden(true);
  pTreeViewItemModel = new QStandardItemModel();

  QStandardItem *pDeviceListItem = new QStandardItem(tr("设备列表"));
  pDeviceListItem->setEditable(false);

  QString iniFileName = Helper::AppDir() + "/Config/communication_device.ini";
  // qDebug()<< "path " << iniFileName;
  QFile fileCfg(iniFileName);
  if (!fileCfg.exists())
    QMessageBox::critical(this, tr("提示"), tr("设备列表选型配置文件不存在！"));

  m_SupportDevList.clear();
  // device type
  QStringList sDevTypeList;
  ConfigUtils::getCfgList(iniFileName, "DeviceSupportList", "list",
                          sDevTypeList);
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
    ConfigUtils::getCfgList(iniFileName, key, "list", devSeriesList);
    for (int i = 0; i < devSeriesList.count(); i++) {
      QString sDevSeriesOne = devSeriesList.at(i).trimmed();
      QStandardItem *itemDevSeriesOne = new QStandardItem(sDevSeriesOne);
      itemDevSeriesOne->setEditable(false);

      // device
      QStringList devList;
      ConfigUtils::getCfgList(iniFileName, sDevSeriesOne, "list", devList);
      for (int i = 0; i < devList.count(); i++) {
        QString sDevOne = devList.at(i).trimmed();

        QString keyDev = sDevOne.left(sDevOne.indexOf("-"));
        QString nameTmpDev =
            sDevOne.right(sDevOne.length() - sDevOne.indexOf("-") - 1);
        QString nameDev = QString::fromLocal8Bit(nameTmpDev.toLatin1());
        // qDebug()<< keyDev << "  " << nameDev;

        if (m_strPortType != nameDev) continue;

        QStandardItem *itemDev = new QStandardItem(keyDev);
        itemDev->setEditable(false);
        m_SupportDevList.append(keyDev);
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

  pTreeViewItemModel->appendRow(pDeviceListItem);
  ui->treeView->setModel(pTreeViewItemModel);
  ui->treeView->expandAll();
}

// 取得设备名称
QString DeviceListDialog::GetDeviceName() const { return m_DeviceName; }

void DeviceListDialog::on_btnOK_clicked() {
  QModelIndex modIndex = ui->treeView->currentIndex();
  QStandardItem *item = pTreeViewItemModel->itemFromIndex(modIndex);
  if (item == NULL) {
    QMessageBox::critical(this, tr("提示"), tr("未选设备！"));
    return;
  }
  bool found = false;
  for (int i = 0; i < m_SupportDevList.count(); i++) {
    if (m_SupportDevList.at(i) == item->text()) {
      m_DeviceName = item->text();
      found = true;
    }
  }
  if (!found)
    QMessageBox::critical(this, tr("提示"), tr("设备错误！"));
  else
    accept();
}

void DeviceListDialog::on_btnCancel_clicked() {
  m_DeviceName = "";
  reject();
}
