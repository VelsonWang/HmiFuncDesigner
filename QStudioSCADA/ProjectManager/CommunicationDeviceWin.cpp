#include "CommunicationDeviceWin.h"
#include "ui_CommunicationDeviceWin.h"
#include "NewComDeviceDialog.h"
#include "NewNetDeviceDialog.h"
#include "ProjectMgrUtils.h"
#include "ProjectData.h"
#include <QMenu>
#include <QAction>
#include <QIcon>
#include <QKeySequence>
#include <QFile>

CommunicationDeviceWin::CommunicationDeviceWin(QWidget *parent,
                                               const QString &itemName,
                                               const QString &projName) :
    ChildBase(parent, itemName, projName),
    ui(new Ui::CommunicationDeviceWin)
{
    ui->setupUi(this);
    pCommDevModel_ = nullptr;
    this->setWindowTitle(itemName);
    m_strItemName = itemName;
    setContextMenuPolicy(Qt::DefaultContextMenu);
}

CommunicationDeviceWin::~CommunicationDeviceWin()
{
    delete ui;
    if(pCommDevModel_ != nullptr) {
        delete pCommDevModel_;
        pCommDevModel_ = nullptr;
    }
}

void CommunicationDeviceWin::ListViewUISetting()
{
    ui->listViewCommunicationDevice->setViewMode(QListView::IconMode);
    ui->listViewCommunicationDevice->setIconSize(QSize(32, 32));
    ui->listViewCommunicationDevice->setGridSize(QSize(120, 120));
    ui->listViewCommunicationDevice->setWordWrap(true);
    ui->listViewCommunicationDevice->setSpacing(20);
    ui->listViewCommunicationDevice->setResizeMode(QListView::Adjust);
    ui->listViewCommunicationDevice->setMovement(QListView::Static);
}

void CommunicationDeviceWin::ListViewUpdate(const QString &it)
{
    DeviceInfo &deviceInfo = ProjectData::getInstance()->deviceInfo_;
    deviceInfo.load(ProjectData::getInstance()->dbData_);

    if(it == tr("通讯设备")) {
        ListViewCommunicationDeviceUpdate();
    } else if(it == tr("串口设备")) {
        ListViewCOMDeviceUpdate();
    } else if(it == tr("网络设备")) {
        ListViewNetDeviceUpdate();
    } else if(it == tr("总线设备")) {
        ListViewBusDeviceUpdate();
    } else if(it == tr("OPC设备")) {
        ListViewOpcDeviceUpdate();
    }
}


/*
* 所有通讯设备列表视图
*/
void CommunicationDeviceWin::ListViewCommunicationDeviceUpdate()
{
    ListViewUISetting();

    pCommDevModel_ = new QStandardItemModel();
    QStandardItem *pNewComDevice = new QStandardItem(QIcon(":/images/pj_com.png"), tr("新建串口设备"));
    pNewComDevice->setEditable(false);
    pCommDevModel_->appendRow(pNewComDevice);

    QStandardItem *pNewNetDevice = new QStandardItem(QIcon(":/images/pj_net.png"), tr("新建网络设备"));
    pNewNetDevice->setEditable(false);
    pCommDevModel_->appendRow(pNewNetDevice);

    QStandardItem *pNewBusDevice = new QStandardItem(QIcon(":/images/pm_bus.png"), tr("新建总线设备"));
    pNewBusDevice->setEditable(false);
    pCommDevModel_->appendRow(pNewBusDevice);

    QStandardItem *pNewOPCDevice = new QStandardItem(QIcon(":/images/pm_opc.PNG"), tr("新建OPC设备"));
    pNewOPCDevice->setEditable(false);
    pCommDevModel_->appendRow(pNewOPCDevice);

    //////////////////////////////////////////////////

    DeviceInfo &deviceInfo = ProjectData::getInstance()->deviceInfo_;
    for(int i=0; i<deviceInfo.listDeviceInfoObject_.count(); i++) {
        DeviceInfoObject *pObj = deviceInfo.listDeviceInfoObject_.at(i);
        if(pObj->szDeviceType_ == "COM") {
            QStandardItem *pNewComDevice = new QStandardItem(QIcon(":/images/pj_com.png"), pObj->szDeviceName_);
            pNewComDevice->setEditable(false);
            pCommDevModel_->appendRow(pNewComDevice);
        } else if(pObj->szDeviceType_ == "NET") {
            QStandardItem *pNewNetDevice = new QStandardItem(QIcon(":/images/pj_net.png"), pObj->szDeviceName_);
            pNewNetDevice->setEditable(false);
            pCommDevModel_->appendRow(pNewNetDevice);
        } else if(pObj->szDeviceType_ == "BUS") {
            QStandardItem *pNewBusDevice = new QStandardItem(QIcon(":/images/pm_bus.png"), pObj->szDeviceName_);
            pNewBusDevice->setEditable(false);
            pCommDevModel_->appendRow(pNewBusDevice);
        } else if(pObj->szDeviceType_ == "OPC") {
            QStandardItem *pNewOPCDevice = new QStandardItem(QIcon(":/images/pm_opc.PNG"), pObj->szDeviceName_);
            pNewOPCDevice->setEditable(false);
            pCommDevModel_->appendRow(pNewOPCDevice);
        }
    }

    ui->listViewCommunicationDevice->setModel(pCommDevModel_);
}

/*
* 串口通讯设备列表视图
*/
void CommunicationDeviceWin::ListViewCOMDeviceUpdate()
{
    ListViewUISetting();

    pCommDevModel_ = new QStandardItemModel();
    QStandardItem *pNewComDevice = new QStandardItem(QIcon(":/images/pj_com.png"), tr("新建串口设备"));
    pNewComDevice->setEditable(false);
    pCommDevModel_->appendRow(pNewComDevice);

    DeviceInfo &deviceInfo = ProjectData::getInstance()->deviceInfo_;
    for(int i=0; i<deviceInfo.listDeviceInfoObject_.count(); i++) {
        DeviceInfoObject *pObj = deviceInfo.listDeviceInfoObject_.at(i);
        if(pObj->szDeviceType_ == "COM") {
            QStandardItem *pComDevice = new QStandardItem(QIcon(":/images/pj_com.png"), pObj->szDeviceName_);
            pComDevice->setEditable(false);
            pCommDevModel_->appendRow(pComDevice);
        }
    }

    ui->listViewCommunicationDevice->setModel(pCommDevModel_);
}

/*
* 网络通讯设备列表视图
*/
void CommunicationDeviceWin::ListViewNetDeviceUpdate()
{
    ListViewUISetting();

    pCommDevModel_ = new QStandardItemModel();
    QStandardItem *pNewNetDevice = new QStandardItem(QIcon(":/images/pj_net.png"), tr("新建网络设备"));
    pNewNetDevice->setEditable(false);
    pCommDevModel_->appendRow(pNewNetDevice);

    DeviceInfo &deviceInfo = ProjectData::getInstance()->deviceInfo_;
    for(int i=0; i<deviceInfo.listDeviceInfoObject_.count(); i++) {
        DeviceInfoObject *pObj = deviceInfo.listDeviceInfoObject_.at(i);
        if(pObj->szDeviceType_ == "NET") {
            QStandardItem *pNetDevice = new QStandardItem(QIcon(":/images/pj_net.png"), pObj->szDeviceName_);
            pNetDevice->setEditable(false);
            pCommDevModel_->appendRow(pNetDevice);
        }
    }

    ui->listViewCommunicationDevice->setModel(pCommDevModel_);
}

/*
* 总线通讯设备列表视图
*/
void CommunicationDeviceWin::ListViewBusDeviceUpdate()
{

}

/*
* OPC通讯设备列表视图
*/
void CommunicationDeviceWin::ListViewOpcDeviceUpdate()
{

}

void CommunicationDeviceWin::on_listViewCommunicationDevice_doubleClicked(const QModelIndex &index)
{
    QStandardItem *item = pCommDevModel_->itemFromIndex(index);
    if(m_strProjectName == "")
        return;
    QString strProjectPath = ProjectMgrUtils::getProjectPath(m_strProjectName);
    if(item->text() == tr("新建串口设备")) {
        NewComDeviceDialog *pNewComDeviceDlg = new NewComDeviceDialog(this);
        QStringList list;

        DeviceInfo &deviceInfo = ProjectData::getInstance()->deviceInfo_;
        for(int i=0; i<deviceInfo.listDeviceInfoObject_.count(); i++) {
            list << deviceInfo.listDeviceInfoObject_.at(i)->szDeviceName_;
        }
        pNewComDeviceDlg->SetListDeviceName(list);
        pNewComDeviceDlg->load(-1);
        if(pNewComDeviceDlg->exec() == QDialog::Accepted) {
            deviceInfo.listDeviceInfoObject_.append((DeviceInfoObject *)pNewComDeviceDlg->GetComDevice());
            pNewComDeviceDlg->save(-1);
        }
    } else if(item->text() == tr("新建网络设备")) {
        NewNetDeviceDialog *pNewNetDeviceDlg = new NewNetDeviceDialog(this);
        QStringList list;
        DeviceInfo &deviceInfo = ProjectData::getInstance()->deviceInfo_;
        for(int i=0; i<deviceInfo.listDeviceInfoObject_.count(); i++) {
            DeviceInfoObject *pObj = deviceInfo.listDeviceInfoObject_.at(i);
            list << pObj->szDeviceName_;
        }
        pNewNetDeviceDlg->SetListDeviceName(list);
        pNewNetDeviceDlg->load(-1);
        if(pNewNetDeviceDlg->exec() == QDialog::Accepted) {
            deviceInfo.listDeviceInfoObject_.append((DeviceInfoObject *)pNewNetDeviceDlg->GetNetDevice());
            pNewNetDeviceDlg->save(-1);
        }
    } else if(item->text() == tr("新建总线设备")) {

    } else if(item->text() == tr("新建OPC设备")) {

    } else {
        DeviceInfo &deviceInfo = ProjectData::getInstance()->deviceInfo_;
        for(int i=0; i<deviceInfo.listDeviceInfoObject_.count(); i++) {
            DeviceInfoObject *pObj = deviceInfo.listDeviceInfoObject_.at(i);
            if(item->text() == pObj->szDeviceName_) {
                if(pObj->szDeviceType_ == "COM") {
                    NewComDeviceDialog *pNewComDeviceDlg = new NewComDeviceDialog(this);
                    pNewComDeviceDlg->load(pObj->iID_);
                    QStringList list;
                    for(int i=0; i<deviceInfo.listDeviceInfoObject_.count(); i++) {
                        list << deviceInfo.listDeviceInfoObject_.at(i)->szDeviceName_;
                    }
                    pNewComDeviceDlg->SetListDeviceName(list);
                    if(pNewComDeviceDlg->exec() == QDialog::Accepted) {
                        pObj->szDeviceName_ = pNewComDeviceDlg->GetDeviceName();
                        pNewComDeviceDlg->save(pObj->iID_);
                        ListViewCOMDeviceUpdate();
                    }
                } else if(pObj->szDeviceType_ == "NET") {
                    NewNetDeviceDialog *pNewNetDeviceDlg = new NewNetDeviceDialog(this);
                    pNewNetDeviceDlg->load(pObj->iID_);
                    QStringList list;
                    for(int i=0; i<deviceInfo.listDeviceInfoObject_.count(); i++) {
                        list << deviceInfo.listDeviceInfoObject_.at(i)->szDeviceName_;
                    }
                    pNewNetDeviceDlg->SetListDeviceName(list);
                    if(pNewNetDeviceDlg->exec() == QDialog::Accepted) {
                        pObj->szDeviceName_ = pNewNetDeviceDlg->GetDeviceName();
                        pNewNetDeviceDlg->save(pObj->iID_);
                        ListViewCOMDeviceUpdate();
                    }
                } else if(pObj->szDeviceType_ == "BUS") {

                } else if(pObj->szDeviceType_ == "OPC") {

                }
            }
        }
    }
    ListViewUpdate(m_strItemName);
}

/*
* 右键菜单
*/
void CommunicationDeviceWin::contextMenuEvent(QContextMenuEvent * event)
{
    Q_UNUSED(event);

    QMenu *pMenu = new QMenu(this);

    QAction *pModifyAct = new QAction(QIcon(":/images/icon_modify.png"), tr("修改"), this);
    //pModifyAct->setShortcut(QKeySequence(Qt::ALT + Qt::Key_M));
    pModifyAct->setStatusTip(tr("修改设备参数"));
    connect(pModifyAct, SIGNAL(triggered()), this, SLOT(ModifyDevice()));
    pMenu->addAction(pModifyAct);

    QAction *pDeleteAct = new QAction(QIcon(":/images/icon_delete.png"), tr("删除"), this);
    //pDeleteAct->setShortcut(QKeySequence(Qt::ALT + Qt::Key_D));
    pDeleteAct->setStatusTip(tr("删除通讯设备"));
    connect(pDeleteAct, SIGNAL(triggered()), this, SLOT(DeleteDevice()));
    pMenu->addAction(pDeleteAct);

    pMenu->move(cursor().pos());
    pMenu->show();
}

/*
* 插槽：新建通讯设备
*/
void CommunicationDeviceWin::NewDevice()
{
    if(m_strProjectName == "")
        return;

    QString strProjectPath = ProjectMgrUtils::getProjectPath(m_strProjectName);
    QList<QStandardItem *> itemList;

    if(m_strItemName == tr("串口设备")) {
        itemList = pCommDevModel_->findItems(tr("新建串口设备"));
        if(itemList.size() == 0)
            return;
        NewComDeviceDialog *pNewComDeviceDlg = new NewComDeviceDialog(this);
        QStringList list;
        DeviceInfo &deviceInfo = ProjectData::getInstance()->deviceInfo_;
        for(int i=0; i<deviceInfo.listDeviceInfoObject_.count(); i++) {
            DeviceInfoObject *pObj = deviceInfo.listDeviceInfoObject_.at(i);
            list << pObj->szDeviceName_;
        }
        pNewComDeviceDlg->SetListDeviceName(list);
        pNewComDeviceDlg->load(-1);
        if(pNewComDeviceDlg->exec() == QDialog::Accepted) {
            deviceInfo.listDeviceInfoObject_.append((DeviceInfoObject *)pNewComDeviceDlg->GetComDevice());
            pNewComDeviceDlg->save(-1);
        }
    } else if(m_strItemName == tr("网络设备")) {
        itemList = pCommDevModel_->findItems(tr("新建网络设备"));
        if(itemList.size() == 0)
            return;
        NewNetDeviceDialog *pNewNetDeviceDlg = new NewNetDeviceDialog(this);
        QStringList list;
        DeviceInfo &deviceInfo = ProjectData::getInstance()->deviceInfo_;
        for(int i=0; i<deviceInfo.listDeviceInfoObject_.count(); i++) {
            DeviceInfoObject *pObj = deviceInfo.listDeviceInfoObject_.at(i);
            list << pObj->szDeviceName_;
        }
        pNewNetDeviceDlg->SetListDeviceName(list);
        pNewNetDeviceDlg->load(-1);
        if(pNewNetDeviceDlg->exec() == QDialog::Accepted) {
            deviceInfo.listDeviceInfoObject_.append((DeviceInfoObject *)pNewNetDeviceDlg->GetNetDevice());
            pNewNetDeviceDlg->save(-1);
        }
    } else if(m_strItemName == tr("总线设备")) {
        itemList = pCommDevModel_->findItems(tr("新建总线设备"));
    } else if(m_strItemName == tr("OPC设备")) {
        itemList = pCommDevModel_->findItems(tr("新建OPC设备"));
    }

    ListViewUpdate(m_strItemName);
}


/*
* 插槽：修改通讯设备参数
*/
void CommunicationDeviceWin::ModifyDevice()
{
    QModelIndex idx = ui->listViewCommunicationDevice->selectionModel()->currentIndex();
    QStandardItem *item = pCommDevModel_->itemFromIndex(idx);
    if(m_strProjectName == "")
        return;
    QString strProjectPath = ProjectMgrUtils::getProjectPath(m_strProjectName);

    DeviceInfo &deviceInfo = ProjectData::getInstance()->deviceInfo_;
    for(int i=0; i<deviceInfo.listDeviceInfoObject_.count(); i++) {
        DeviceInfoObject *pObj = deviceInfo.listDeviceInfoObject_.at(i);
        if(item->text() == pObj->szDeviceName_) {
            if(pObj->szDeviceType_ == "COM") {
                NewComDeviceDialog *pNewComDeviceDlg = new NewComDeviceDialog(this);
                pNewComDeviceDlg->load(pObj->iID_);
                pNewComDeviceDlg->exec();                
                pObj->szDeviceName_ = pNewComDeviceDlg->GetDeviceName();
                pNewComDeviceDlg->save(pObj->iID_);
            } else if(pObj->szDeviceType_ == "NET") {
                NewNetDeviceDialog *pNewDeviceDlg = new NewNetDeviceDialog(this);
                pNewDeviceDlg->load(pObj->iID_);
                pNewDeviceDlg->exec();
                pObj->szDeviceName_ = pNewDeviceDlg->GetDeviceName();
                pNewDeviceDlg->save(pObj->iID_);
            } else if(pObj->szDeviceType_ == "BUS") {

            } else if(pObj->szDeviceType_ == "OPC") {

            }
        }
    }

    ListViewUpdate(m_strItemName);
}

/*
* 插槽：删除通讯设备
*/
void CommunicationDeviceWin::DeleteDevice()
{
    QModelIndex idx = ui->listViewCommunicationDevice->selectionModel()->currentIndex();
    QStandardItem *item = pCommDevModel_->itemFromIndex(idx);

    DeviceInfo &deviceInfo = ProjectData::getInstance()->deviceInfo_;
    for(int i=0; i<deviceInfo.listDeviceInfoObject_.count(); i++) {
        DeviceInfoObject *pObj = deviceInfo.listDeviceInfoObject_.at(i);
        if(item->text() == pObj->szDeviceName_) {
            deviceInfo.del(ProjectData::getInstance()->dbData_, pObj);
            break;
        }
    }

    ListViewUpdate(m_strItemName);
}

/*
 * 打开文件
 */
void CommunicationDeviceWin::open()
{

}

/*
 * 保存文件
 */
void CommunicationDeviceWin::save()
{

}

/*
* 显示大图标
*/
void CommunicationDeviceWin::showLargeIcon()
{
    ui->listViewCommunicationDevice->setIconSize(QSize(32, 32));
}

/*
* 显示小图标
*/
void CommunicationDeviceWin::showSmallIcon()
{
    ui->listViewCommunicationDevice->setIconSize(QSize(24, 24));
}
