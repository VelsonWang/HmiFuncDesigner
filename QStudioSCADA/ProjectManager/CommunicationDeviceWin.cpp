#include "CommunicationDeviceWin.h"
#include "ui_CommunicationDeviceWin.h"
#include "NewComDeviceDialog.h"
#include "NewNetDeviceDialog.h"
#include "DeviceBase.h"
#include "ProjectMgrUtils.h"
#include <QMenu>
#include <QAction>
#include <QIcon>
#include <QKeySequence>
#include <QFile>

#include <QDebug>

CommunicationDeviceWin::CommunicationDeviceWin(QWidget *parent,
                                               const QString &itemName,
                                               const QString &projName) :
    ChildBase(parent, itemName, projName),
    ui(new Ui::CommunicationDeviceWin)
{
    ui->setupUi(this);
    pListViewCommDevModel = nullptr;
    this->setWindowTitle(itemName);
    m_strItemName = itemName;
    m_pLinkManager = nullptr;
    setContextMenuPolicy(Qt::DefaultContextMenu);
}

CommunicationDeviceWin::~CommunicationDeviceWin()
{
    delete ui;
    if(pListViewCommDevModel != nullptr) {
        delete pListViewCommDevModel;
        pListViewCommDevModel = nullptr;
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
    //设置图标可不可以移动，默认是可移动的，但可以改成静态的：
    ui->listViewCommunicationDevice->setMovement(QListView::Static);
}

void CommunicationDeviceWin::ListViewUpdate(const QString &it)
{
    if(m_pLinkManager == nullptr)
        m_pLinkManager = new LinkManager(ProjectMgrUtils::getProjectPath(m_strProjectName));

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

    pListViewCommDevModel = new QStandardItemModel();
    QStandardItem *pNewComDevice = new QStandardItem(QIcon(":/images/pj_com.png"), tr("新建串口设备"));
    pNewComDevice->setEditable(false);
    pListViewCommDevModel->appendRow(pNewComDevice);

    QStandardItem *pNewNetDevice = new QStandardItem(QIcon(":/images/pj_net.png"), tr("新建网络设备"));
    pNewNetDevice->setEditable(false);
    pListViewCommDevModel->appendRow(pNewNetDevice);

    QStandardItem *pNewBusDevice = new QStandardItem(QIcon(":/images/pm_bus.png"), tr("新建总线设备"));
    pNewBusDevice->setEditable(false);
    pListViewCommDevModel->appendRow(pNewBusDevice);

    QStandardItem *pNewOPCDevice = new QStandardItem(QIcon(":/images/pm_opc.PNG"), tr("新建OPC设备"));
    pNewOPCDevice->setEditable(false);
    pListViewCommDevModel->appendRow(pNewOPCDevice);

    //////////////////////////////////////////////////

    m_pLinkManager->devList.clear();
    m_pLinkManager->loadFromFile(DATA_SAVE_FORMAT);
    for(int i=0; i<m_pLinkManager->devList.count(); i++) {
        DeviceBase *dev = m_pLinkManager->devList.at(i);
        if(dev->m_sDeviceType == "COM") {
            QStandardItem *pNewComDevice = new QStandardItem(QIcon(":/images/pj_com.png"), dev->m_sDeviceName);
            pNewComDevice->setEditable(false);
            pListViewCommDevModel->appendRow(pNewComDevice);
        } else if(dev->m_sDeviceType == "NET") {
            QStandardItem *pNewNetDevice = new QStandardItem(QIcon(":/images/pj_net.png"), dev->m_sDeviceName);
            pNewNetDevice->setEditable(false);
            pListViewCommDevModel->appendRow(pNewNetDevice);
        } else if(dev->m_sDeviceType == "BUS") {
            QStandardItem *pNewBusDevice = new QStandardItem(QIcon(":/images/pm_bus.png"), dev->m_sDeviceName);
            pNewBusDevice->setEditable(false);
            pListViewCommDevModel->appendRow(pNewBusDevice);
        } else if(dev->m_sDeviceType == "OPC") {
            QStandardItem *pNewOPCDevice = new QStandardItem(QIcon(":/images/pm_opc.PNG"), dev->m_sDeviceName);
            pNewOPCDevice->setEditable(false);
            pListViewCommDevModel->appendRow(pNewOPCDevice);
        }
    }

    ui->listViewCommunicationDevice->setModel(pListViewCommDevModel);
}

/*
* 串口通讯设备列表视图
*/
void CommunicationDeviceWin::ListViewCOMDeviceUpdate()
{
    ListViewUISetting();

    pListViewCommDevModel = new QStandardItemModel();
    QStandardItem *pNewComDevice = new QStandardItem(QIcon(":/images/pj_com.png"), tr("新建串口设备"));
    pNewComDevice->setEditable(false);
    pListViewCommDevModel->appendRow(pNewComDevice);

    m_pLinkManager->devList.clear();
    m_pLinkManager->loadFromFile(DATA_SAVE_FORMAT);
    for(int i=0; i<m_pLinkManager->devList.count(); i++) {
        DeviceBase *dev = m_pLinkManager->devList.at(i);
        if(dev->m_sDeviceType == "COM") {
            QStandardItem *pComDevice = new QStandardItem(QIcon(":/images/pj_com.png"), dev->m_sDeviceName);
            pComDevice->setEditable(false);
            pListViewCommDevModel->appendRow(pComDevice);
        }
    }

    ui->listViewCommunicationDevice->setModel(pListViewCommDevModel);
}

/*
* 网络通讯设备列表视图
*/
void CommunicationDeviceWin::ListViewNetDeviceUpdate()
{
    ListViewUISetting();

    pListViewCommDevModel = new QStandardItemModel();
    QStandardItem *pNewNetDevice = new QStandardItem(QIcon(":/images/pj_net.png"), tr("新建网络设备"));
    pNewNetDevice->setEditable(false);
    pListViewCommDevModel->appendRow(pNewNetDevice);

    m_pLinkManager->devList.clear();
    m_pLinkManager->loadFromFile(DATA_SAVE_FORMAT);
    for(int i=0; i<m_pLinkManager->devList.count(); i++) {
        DeviceBase *dev = m_pLinkManager->devList.at(i);
        if(dev->m_sDeviceType == "NET") {
            QStandardItem *pNetDevice = new QStandardItem(QIcon(":/images/pj_net.png"), dev->m_sDeviceName);
            pNetDevice->setEditable(false);
            pListViewCommDevModel->appendRow(pNetDevice);
        }
    }

    ui->listViewCommunicationDevice->setModel(pListViewCommDevModel);
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
    QStandardItem *item = pListViewCommDevModel->itemFromIndex(index);
    if(m_strProjectName == "")
        return;
    QString strProjectPath = ProjectMgrUtils::getProjectPath(m_strProjectName);
    if(item->text() == tr("新建串口设备")) {
        NewComDeviceDialog *pNewComDeviceDlg = new NewComDeviceDialog(this, strProjectPath);
        QStringList list;
        for(int i=0; i<m_pLinkManager->devList.count(); i++) {
            DeviceBase *dev = m_pLinkManager->devList.at(i);
            list << dev->m_sDeviceName;
        }
        pNewComDeviceDlg->SetListDeviceName(list);
        if(pNewComDeviceDlg->exec() == QDialog::Accepted) {
            m_pLinkManager->addDevice((DeviceBase *)pNewComDeviceDlg->GetComDevice());
            m_pLinkManager->saveToFile(DATA_SAVE_FORMAT);            
        }
    } else if(item->text() == tr("新建网络设备")) {
        NewNetDeviceDialog *pNewNetDeviceDlg = new NewNetDeviceDialog(this, strProjectPath);
        QStringList list;
        for(int i=0; i<m_pLinkManager->devList.count(); i++) {
            DeviceBase *dev = m_pLinkManager->devList.at(i);
            list << dev->m_sDeviceName;
        }
        pNewNetDeviceDlg->SetListDeviceName(list);
        if(pNewNetDeviceDlg->exec() == QDialog::Accepted) {
            m_pLinkManager->addDevice((DeviceBase *)pNewNetDeviceDlg->GetNetDevice());
            m_pLinkManager->saveToFile(DATA_SAVE_FORMAT);
        }
    } else if(item->text() == tr("新建总线设备")) {

    } else if(item->text() == tr("新建OPC设备")) {

    } else {
        for(int i=0; i<m_pLinkManager->devList.count(); i++) {
            DeviceBase *dev = m_pLinkManager->devList.at(i);
            if(item->text() == dev->m_sDeviceName) {
                if(dev->m_sDeviceType == "COM") {
                    NewComDeviceDialog *pNewComDeviceDlg = new NewComDeviceDialog(this, strProjectPath);
                    pNewComDeviceDlg->loadFromFile(DATA_SAVE_FORMAT, strProjectPath + "/" + dev->m_sDeviceName + ".odb");
                    QStringList list;
                    for(int i=0; i<m_pLinkManager->devList.count(); i++) {
                        DeviceBase *dev = m_pLinkManager->devList.at(i);
                        list << dev->m_sDeviceName;
                    }
                    pNewComDeviceDlg->SetListDeviceName(list);
                    if(pNewComDeviceDlg->exec() == QDialog::Accepted) {
                        dev->m_sDeviceName = pNewComDeviceDlg->GetDeviceName();
                        m_pLinkManager->saveToFile(DATA_SAVE_FORMAT);
                        ListViewCOMDeviceUpdate();
                    }
                } else if(dev->m_sDeviceType == "NET") {
                    NewNetDeviceDialog *pNewNetDeviceDlg = new NewNetDeviceDialog(this, strProjectPath);
                    pNewNetDeviceDlg->loadFromFile(DATA_SAVE_FORMAT, strProjectPath + "/" + dev->m_sDeviceName + ".odb");
                    QStringList list;
                    for(int i=0; i<m_pLinkManager->devList.count(); i++) {
                        DeviceBase *dev = m_pLinkManager->devList.at(i);
                        list << dev->m_sDeviceName;
                    }
                    pNewNetDeviceDlg->SetListDeviceName(list);
                    if(pNewNetDeviceDlg->exec() == QDialog::Accepted) {
                        dev->m_sDeviceName = pNewNetDeviceDlg->GetDeviceName();
                        m_pLinkManager->saveToFile(DATA_SAVE_FORMAT);
                        ListViewCOMDeviceUpdate();
                    }
                } else if(dev->m_sDeviceType == "BUS") {

                } else if(dev->m_sDeviceType == "OPC") {

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
        itemList = pListViewCommDevModel->findItems(tr("新建串口设备"));
        if(itemList.size() == 0)
            return;
        //QStandardItem *item = itemList.at(0);
        NewComDeviceDialog *pNewComDeviceDlg = new NewComDeviceDialog(this, strProjectPath);
        QStringList list;
        for(int i=0; i<m_pLinkManager->devList.count(); i++) {
            DeviceBase *dev = m_pLinkManager->devList.at(i);
            list << dev->m_sDeviceName;
        }
        pNewComDeviceDlg->SetListDeviceName(list);
        if(pNewComDeviceDlg->exec() == QDialog::Accepted) {
            m_pLinkManager->addDevice((DeviceBase *)pNewComDeviceDlg->GetComDevice());
            m_pLinkManager->saveToFile(DATA_SAVE_FORMAT);
        }
    } else if(m_strItemName == tr("网络设备")) {
        itemList = pListViewCommDevModel->findItems(tr("新建网络设备"));
        if(itemList.size() == 0)
            return;
        NewNetDeviceDialog *pNewNetDeviceDlg = new NewNetDeviceDialog(this, strProjectPath);
        QStringList list;
        for(int i=0; i<m_pLinkManager->devList.count(); i++) {
            DeviceBase *dev = m_pLinkManager->devList.at(i);
            list << dev->m_sDeviceName;
        }
        pNewNetDeviceDlg->SetListDeviceName(list);
        if(pNewNetDeviceDlg->exec() == QDialog::Accepted) {
            m_pLinkManager->addDevice((DeviceBase *)pNewNetDeviceDlg->GetNetDevice());
            m_pLinkManager->saveToFile(DATA_SAVE_FORMAT);
        }
    } else if(m_strItemName == tr("总线设备")) {
        itemList = pListViewCommDevModel->findItems(tr("新建总线设备"));
    } else if(m_strItemName == tr("OPC设备")) {
        itemList = pListViewCommDevModel->findItems(tr("新建OPC设备"));
    }

    ListViewUpdate(m_strItemName);
}


/*
* 插槽：修改通讯设备参数
*/
void CommunicationDeviceWin::ModifyDevice()
{
    QModelIndex idx = ui->listViewCommunicationDevice->selectionModel()->currentIndex();
    QStandardItem *item = pListViewCommDevModel->itemFromIndex(idx);
    if(m_strProjectName == "")
        return;
    QString strProjectPath = ProjectMgrUtils::getProjectPath(m_strProjectName);

    for(int i=0; i<m_pLinkManager->devList.count(); i++) {
        DeviceBase *dev = m_pLinkManager->devList.at(i);
        if(item->text() == dev->m_sDeviceName) {
            if(dev->m_sDeviceType == "COM") {
                NewComDeviceDialog *pNewComDeviceDlg = new NewComDeviceDialog(this, strProjectPath);
                pNewComDeviceDlg->loadFromFile(DATA_SAVE_FORMAT, strProjectPath + "/" + dev->m_sDeviceName + ".odb");
                pNewComDeviceDlg->exec();                
                dev->m_sDeviceName = pNewComDeviceDlg->GetDeviceName();
            } else if(dev->m_sDeviceType == "NET") {
                NewNetDeviceDialog *pNewDeviceDlg = new NewNetDeviceDialog(this, strProjectPath);
                pNewDeviceDlg->loadFromFile(DATA_SAVE_FORMAT, strProjectPath + "/" + dev->m_sDeviceName + ".odb");
                pNewDeviceDlg->exec();
                dev->m_sDeviceName = pNewDeviceDlg->GetDeviceName();
            } else if(dev->m_sDeviceType == "BUS") {

            } else if(dev->m_sDeviceType == "OPC") {

            }
            m_pLinkManager->saveToFile(DATA_SAVE_FORMAT);
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
    QStandardItem *item = pListViewCommDevModel->itemFromIndex(idx);

    for(int i=0; i<m_pLinkManager->devList.count(); i++) {
        DeviceBase *dev = m_pLinkManager->devList.at(i);
        if(item->text() == dev->m_sDeviceName) {
            QString strProjPath = ProjectMgrUtils::getProjectPath(m_strProjectName);
            QFile file(strProjPath + "/" + dev->m_sDeviceName + ".odb");
            if(file.exists())
                file.remove();
            m_pLinkManager->delDevice(dev);
            m_pLinkManager->saveToFile(DATA_SAVE_FORMAT);
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
