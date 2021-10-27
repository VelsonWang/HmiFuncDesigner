#include "communicationdevice.h"
#include "newcomdevicedialog.h"
#include "newnetdevicedialog.h"
#include "qsoftcore.h"
#include "qprojectcore.h"
#include "../../Public/userevent.h"
#include <QMenu>
#include <QAction>
#include <QIcon>
#include <QKeySequence>
#include <QContextMenuEvent>
#include <QFile>

CommunicationDevice::CommunicationDevice(QWidget *parent) : QWidget(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);

    m_pListViewCommDevObj = new ListViewEx(this);
    m_pListViewCommDevObj->setViewMode(QListView::IconMode);
    m_pListViewCommDevObj->setViewportMargin(0, 0, 0, 0);
    m_pListViewCommDevObj->setIconSize(QSize(32, 32));
    m_pListViewCommDevObj->setGridSize(QSize(120, 120));
    m_pListViewCommDevObj->setWordWrap(true);
    m_pListViewCommDevObj->setSpacing(20);
    m_pListViewCommDevObj->setResizeMode(QListView::Adjust);
    m_pListViewCommDevObj->setMovement(QListView::Static);
    connect(m_pListViewCommDevObj, &QAbstractItemView::doubleClicked, this,
            &CommunicationDevice::onSlotListViewProjectDoubleClicked);

    QVBoxLayout *pVLayoutObj = new QVBoxLayout(this);
    pVLayoutObj->setSpacing(0);
    pVLayoutObj->setContentsMargins(0, 0, 0, 0);
    pVLayoutObj->addWidget(m_pListViewCommDevObj);
    this->setLayout(pVLayoutObj);

    m_pCommDevModelObj = new QStandardItemModel();
    m_pListViewCommDevObj->setModel(m_pCommDevModelObj);

    setContextMenuPolicy(Qt::DefaultContextMenu);
}

CommunicationDevice::~CommunicationDevice()
{
    if(m_pCommDevModelObj != NULL) {
        delete m_pCommDevModelObj;
        m_pCommDevModelObj = NULL;
    }
    if(m_pListViewCommDevObj != NULL) {
        delete m_pListViewCommDevObj;
        m_pListViewCommDevObj = NULL;
    }
}


void CommunicationDevice::listViewUISetting()
{
    m_pListViewCommDevObj->setViewMode(QListView::IconMode);
    m_pListViewCommDevObj->setViewportMargin(0, 0, 0, 0);
    m_pListViewCommDevObj->setIconSize(QSize(32, 32));
    m_pListViewCommDevObj->setGridSize(QSize(120, 120));
    m_pListViewCommDevObj->setWordWrap(true);
    m_pListViewCommDevObj->setSpacing(20);
    m_pListViewCommDevObj->setResizeMode(QListView::Adjust);
    m_pListViewCommDevObj->setMovement(QListView::Static);
}

void CommunicationDevice::listViewUpdate()
{
    if(m_szItemName == QString("CommunicationDevice").toUpper()) { // 通讯设备
        listViewCommunicationDeviceUpdate();
    } else if(m_szItemName == QString("ComDevice").toUpper()) { // 串口设备
        listViewCOMDeviceUpdate();
    } else if(m_szItemName == QString("NetDevice").toUpper()) { // 网络设备
        listViewNetDeviceUpdate();
    }
}

/**
 * @brief CommunicationDevice::listViewCommunicationDeviceUpdate
 * @details 所有通讯设备列表视图
 */
void CommunicationDevice::listViewCommunicationDeviceUpdate()
{
    m_pCommDevModelObj->clear();
    listViewUISetting();

    QStandardItem *pNewComDevice = new QStandardItem(QIcon(":/images/pj_com.png"), tr("新建串口设备"));
    pNewComDevice->setEditable(false);
    pNewComDevice->setData(QString("NewComDevice").toUpper(), Qt::UserRole + 1);
    m_pCommDevModelObj->appendRow(pNewComDevice);

    QStandardItem *pNewNetDevice = new QStandardItem(QIcon(":/images/pj_net.png"), tr("新建网络设备"));
    pNewNetDevice->setEditable(false);
    pNewNetDevice->setData(QString("NewNetDevice").toUpper(), Qt::UserRole + 1);
    m_pCommDevModelObj->appendRow(pNewNetDevice);

    QStandardItem *pNewBusDevice = new QStandardItem(QIcon(":/images/pj_bus.png"), tr("新建总线设备"));
    pNewBusDevice->setEditable(false);
    pNewBusDevice->setData(QString("NewBusDevice").toUpper(), Qt::UserRole + 1);
    //m_pCommDevModelObj->appendRow(pNewBusDevice);

    QStandardItem *pNewOPCDevice = new QStandardItem(QIcon(":/images/pj_opc.png"), tr("新建OPC设备"));
    pNewOPCDevice->setEditable(false);
    pNewOPCDevice->setData(QString("NewOPCDevice").toUpper(), Qt::UserRole + 1);
    //m_pCommDevModelObj->appendRow(pNewOPCDevice);

    ////////////////////////////////////////////////////////////////////////////

    DeviceInfo &deviceInfo = QSoftCore::getCore()->getProjectCore()->deviceInfo_;
    for(int i = 0; i < deviceInfo.listDeviceInfoObject_.count(); i++) {
        DeviceInfoObject *pObj = deviceInfo.listDeviceInfoObject_.at(i);
        if(pObj->szDeviceType_ == "COM") {
            QStandardItem *pNewComDevice = new QStandardItem(QIcon(":/images/pj_com.png"), pObj->szName_);
            pNewComDevice->setEditable(false);
            pNewComDevice->setData(pObj->szName_, Qt::UserRole + 1);
            m_pCommDevModelObj->appendRow(pNewComDevice);
        } else if(pObj->szDeviceType_ == "NET") {
            QStandardItem *pNewNetDevice = new QStandardItem(QIcon(":/images/pj_net.png"), pObj->szName_);
            pNewNetDevice->setEditable(false);
            pNewNetDevice->setData(pObj->szName_, Qt::UserRole + 1);
            m_pCommDevModelObj->appendRow(pNewNetDevice);
        }
    }
}

/**
 * @brief CommunicationDevice::listViewCOMDeviceUpdate
 * @details 串口通讯设备列表视图
 */
void CommunicationDevice::listViewCOMDeviceUpdate()
{
    m_pCommDevModelObj->clear();
    listViewUISetting();

    QStandardItem *pNewComDevice = new QStandardItem(QIcon(":/images/pj_com.png"), tr("新建串口设备"));
    pNewComDevice->setEditable(false);
    pNewComDevice->setData(QString("NewComDevice").toUpper(), Qt::UserRole + 1);
    m_pCommDevModelObj->appendRow(pNewComDevice);

    DeviceInfo &deviceInfo = QSoftCore::getCore()->getProjectCore()->deviceInfo_;
    for(int i = 0; i < deviceInfo.listDeviceInfoObject_.count(); i++) {
        DeviceInfoObject *pObj = deviceInfo.listDeviceInfoObject_.at(i);
        if(pObj->szDeviceType_ == "COM") {
            QStandardItem *pComDevice = new QStandardItem(QIcon(":/images/pj_com.png"), pObj->szName_);
            pComDevice->setEditable(false);
            pComDevice->setData(pObj->szName_, Qt::UserRole + 1);
            m_pCommDevModelObj->appendRow(pComDevice);
        }
    }
}

/**
 * @brief CommunicationDevice::listViewNetDeviceUpdate
 * @details 网络通讯设备列表视图
 */
void CommunicationDevice::listViewNetDeviceUpdate()
{
    m_pCommDevModelObj->clear();
    listViewUISetting();

    QStandardItem *pNewNetDevice = new QStandardItem(QIcon(":/images/pj_net.png"), tr("新建网络设备"));
    pNewNetDevice->setEditable(false);
    pNewNetDevice->setData(QString("NewNetDevice").toUpper(), Qt::UserRole + 1);
    m_pCommDevModelObj->appendRow(pNewNetDevice);

    DeviceInfo &deviceInfo = QSoftCore::getCore()->getProjectCore()->deviceInfo_;
    for(int i = 0; i < deviceInfo.listDeviceInfoObject_.count(); i++) {
        DeviceInfoObject *pObj = deviceInfo.listDeviceInfoObject_.at(i);
        if(pObj->szDeviceType_ == "NET") {
            QStandardItem *pNetDevice = new QStandardItem(QIcon(":/images/pj_net.png"), pObj->szName_);
            pNetDevice->setEditable(false);
            pNetDevice->setData(pObj->szName_, Qt::UserRole + 1);
            m_pCommDevModelObj->appendRow(pNetDevice);
        }
    }
}


/*
* 右键菜单
*/
void CommunicationDevice::contextMenuEvent(QContextMenuEvent * event)
{
    QModelIndex index = m_pListViewCommDevObj->indexAt(event->pos());
    if(!index.isValid()) { // 单击空白部分
        return;
    }

    QMenu *pMenu = new QMenu(this);

    QAction *pModifyAct = new QAction(QIcon(":/images/icon_modify.png"), tr("修改"), this);
    //pModifyAct->setShortcut(QKeySequence(Qt::ALT + Qt::Key_M));
    pModifyAct->setStatusTip(tr("修改设备参数"));
    connect(pModifyAct, SIGNAL(triggered()), this, SLOT(onSlotModifyDevice()));
    pMenu->addAction(pModifyAct);

    QAction *pDeleteAct = new QAction(QIcon(":/images/icon_delete.png"), tr("删除"), this);
    //pDeleteAct->setShortcut(QKeySequence(Qt::ALT + Qt::Key_D));
    pDeleteAct->setStatusTip(tr("删除通讯设备"));
    connect(pDeleteAct, SIGNAL(triggered()), this, SLOT(onSlotDeleteDevice()));
    pMenu->addAction(pDeleteAct);

    if(!pMenu->isEmpty()) {
        pMenu->move(cursor().pos());
        pMenu->exec();
        pMenu->clear();
    }
    delete pMenu;
}

/*
* 插槽：新建通讯设备
*/
void CommunicationDevice::onSlotNewDevice()
{
    if(QSoftCore::getCore()->getProjectCore()->m_szProjName == "") {
        return;
    }

    QList<QStandardItem *> itemList;

    if(m_szItemName == QString("NewComDevice").toUpper()) { // 串口设备
        itemList = m_pCommDevModelObj->findItems(tr("新建串口设备"));
        if(itemList.size() == 0) {
            return;
        }
        NewComDeviceDialog *pNewComDeviceDlg = new NewComDeviceDialog(this);
        pNewComDeviceDlg->load("");
        if(pNewComDeviceDlg->exec() == QDialog::Accepted) {
            pNewComDeviceDlg->save("");
        }
    } else if(m_szItemName == QString("NewNetDevice").toUpper()) { // 网络设备
        itemList = m_pCommDevModelObj->findItems(tr("新建网络设备"));
        if(itemList.size() == 0) {
            return;
        }
        NewNetDeviceDialog *pNewNetDeviceDlg = new NewNetDeviceDialog(this);
        pNewNetDeviceDlg->load("");
        if(pNewNetDeviceDlg->exec() == QDialog::Accepted) {
            pNewNetDeviceDlg->save("");
        }
    }

    listViewUpdate();
}


/*
* 插槽：修改通讯设备参数
*/
void CommunicationDevice::onSlotModifyDevice()
{
    QModelIndex idx = m_pListViewCommDevObj->selectionModel()->currentIndex();
    QStandardItem *pItemObj = m_pCommDevModelObj->itemFromIndex(idx);
    if(QSoftCore::getCore()->getProjectCore()->m_szProjName == "") {
        return;
    }

    DeviceInfo &deviceInfo = QSoftCore::getCore()->getProjectCore()->deviceInfo_;
    DeviceInfoObject *pObj = deviceInfo.getObjectByName(pItemObj->text());
    if(pObj != NULL) {
        if(pObj->szDeviceType_ == "COM") {
            NewComDeviceDialog *pNewComDeviceDlg = new NewComDeviceDialog(this);
            pNewComDeviceDlg->load(pObj->szName_);
            pNewComDeviceDlg->exec();
            pObj->szDeviceName_ = pNewComDeviceDlg->GetDeviceName();
            pNewComDeviceDlg->save(pObj->szName_);
        } else if(pObj->szDeviceType_ == "NET") {
            NewNetDeviceDialog *pNewDeviceDlg = new NewNetDeviceDialog(this);
            pNewDeviceDlg->load(pObj->szName_);
            pNewDeviceDlg->exec();
            pObj->szDeviceName_ = pNewDeviceDlg->GetDeviceName();
            pNewDeviceDlg->save(pObj->szName_);
        }
    }

    listViewUpdate();
}

/*
* 插槽：删除通讯设备
*/
void CommunicationDevice::onSlotDeleteDevice()
{
    QModelIndex idx = m_pListViewCommDevObj->selectionModel()->currentIndex();
    QStandardItem *pItemObj = m_pCommDevModelObj->itemFromIndex(idx);

    DeviceInfo &deviceInfo = QSoftCore::getCore()->getProjectCore()->deviceInfo_;
    for(int i = 0; i < deviceInfo.listDeviceInfoObject_.count(); i++) {
        DeviceInfoObject *pObj = deviceInfo.listDeviceInfoObject_.at(i);
        if(pItemObj->text() == pObj->szName_) {
            deviceInfo.listDeviceInfoObject_.removeOne(pObj);
            break;
        }
    }

    listViewUpdate();
}


void CommunicationDevice::onSlotListViewProjectDoubleClicked(const QModelIndex &index)
{
    QStandardItem *pItemObj = m_pCommDevModelObj->itemFromIndex(index);
    if(QSoftCore::getCore()->getProjectCore()->m_szProjName == "") {
        return;
    }
    QString szItemText = pItemObj->data(Qt::UserRole + 1).toString();

    if(szItemText == QString("NewComDevice").toUpper()) { // 串口设备
        NewComDeviceDialog *pNewComDeviceDlg = new NewComDeviceDialog(this);
        pNewComDeviceDlg->load("");
        if(pNewComDeviceDlg->exec() == QDialog::Accepted) {
            pNewComDeviceDlg->save("");
        }
    } else if(szItemText == QString("NewNetDevice").toUpper()) { // 网络设备
        NewNetDeviceDialog *pNewNetDeviceDlg = new NewNetDeviceDialog(this);
        pNewNetDeviceDlg->load("");
        if(pNewNetDeviceDlg->exec() == QDialog::Accepted) {
            pNewNetDeviceDlg->save("");
        }
    } else {
        DeviceInfo &deviceInfo = QSoftCore::getCore()->getProjectCore()->deviceInfo_;
        DeviceInfoObject *pObj = deviceInfo.getObjectByName(pItemObj->text());
        if(pObj != NULL) {
            if(pObj->szDeviceType_ == "COM") {
                NewComDeviceDialog *pNewComDeviceDlg = new NewComDeviceDialog(this);
                pNewComDeviceDlg->load(pObj->szName_);
                pNewComDeviceDlg->exec();
                pObj->szDeviceName_ = pNewComDeviceDlg->GetDeviceName();
                pNewComDeviceDlg->save(pObj->szName_);
            } else if(pObj->szDeviceType_ == "NET") {
                NewNetDeviceDialog *pNewDeviceDlg = new NewNetDeviceDialog(this);
                pNewDeviceDlg->load(pObj->szName_);
                pNewDeviceDlg->exec();
                pObj->szDeviceName_ = pNewDeviceDlg->GetDeviceName();
                pNewDeviceDlg->save(pObj->szName_);
            }
        }
    }
    listViewUpdate();
}


bool CommunicationDevice::event(QEvent *ev)
{
    if(ev->type() == UserEvent::EVT_USER_SHOW_UPDATE) {
        UserEvent *pEvObj = dynamic_cast<UserEvent *>(ev);
        if(pEvObj) {
            m_szItemName = pEvObj->data().toString();
        }
        listViewUpdate();
        return true;
    } else if(ev->type() == UserEvent::EVT_USER_HIDE_UPDATE) {
        return true;
    }
    return QWidget::event(ev);
}


