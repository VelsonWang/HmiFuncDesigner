#include "ProjectTreeView.h"
#include <QMenu>
#include <QStandardItem>
#include <QModelIndex>
#include <QApplication>
#include <QMessageBox>
#include <QDebug>
#include "ProjectData.h"
#include "DBTag.h"
#include "NewVariableGroupDialog.h"

ProjectTreeView::ProjectTreeView(QWidget *parent) : QTreeView(parent)
{
    m_pItemModelObj = new QStandardItemModel();
    this->setModel(m_pItemModelObj);
    setContextMenuPolicy(Qt::DefaultContextMenu);
    connect(this, &QAbstractItemView::clicked, this, &ProjectTreeView::onSlotClicked);
    connect(this, &QAbstractItemView::doubleClicked, this, &ProjectTreeView::onSlotDoubleClicked);
}

ProjectTreeView::~ProjectTreeView()
{
    if(m_pItemModelObj != Q_NULLPTR) {
        delete m_pItemModelObj;
        m_pItemModelObj = Q_NULLPTR;
    }
}


/**
 * @brief ProjectTreeView::contextMenuEvent
 * @details 右键菜单
 * @param event
 */
void ProjectTreeView::contextMenuEvent(QContextMenuEvent * event)
{ 
    QModelIndex index = indexAt(event->pos());
    if(!index.isValid()) { // 单击空白部分
        return;
    }

    bool found = false;
    QStandardItemModel *pModelObj = dynamic_cast<QStandardItemModel *>(this->model());
    QString szCurItemText = pModelObj->itemFromIndex(index)->text();

    if (szCurItemText == tr("设备变量"))
        found = true;

    TagIOGroup &tagIOGroup = ProjectData::getInstance()->tagIOGroup_;
    tagIOGroup.load(ProjectData::getInstance()->dbData_);

    foreach (TagIOGroupDBItem *pObj, tagIOGroup.listTagIOGroupDBItem_) {
        if (szCurItemText == pObj->m_szShowName)
            found = true;
    }

    qDeleteAll(tagIOGroup.listTagIOGroupDBItem_);
    tagIOGroup.listTagIOGroupDBItem_.clear();

    if (!found) return;

    QMenu *pMenu = new QMenu(this);

    QAction *pAddGroupAct = new QAction(tr("增加组"), this);
    pAddGroupAct->setStatusTip(tr("增加组"));
    connect(pAddGroupAct, SIGNAL(triggered()), this, SLOT(onSlotTagIOGroupAdd()));
    pMenu->addAction(pAddGroupAct);

    QAction *pRenameGroupAct = new QAction(tr("重命名"), this);
    pRenameGroupAct->setStatusTip(tr("重命名"));
    connect(pRenameGroupAct, SIGNAL(triggered()), this, SLOT(onSlotTagIOGroupRename()));
    pMenu->addAction(pRenameGroupAct);

    QAction *pDeleteGroupAct = new QAction(tr("删除组"), this);
    pDeleteGroupAct->setStatusTip(tr("删除组"));
    connect(pDeleteGroupAct, SIGNAL(triggered()), this, SLOT(onSlotTagIODeleteGroup()));
    pMenu->addAction(pDeleteGroupAct);

    QAction *pCopyGroupAct = new QAction(tr("复制组"), this);
    pCopyGroupAct->setStatusTip(tr("复制组"));
    connect(pCopyGroupAct, SIGNAL(triggered()), this, SLOT(onSlotTagIOGroupCopy()));
    pMenu->addAction(pCopyGroupAct);

    if(!pMenu->isEmpty()) {
        pMenu->move(cursor().pos());
        pMenu->exec();
        pMenu->clear();
    }
    delete pMenu;
}

///
/// \brief ProjectTreeView::onSlotClicked
/// \details 叶节点被单击
/// \param index
///
void ProjectTreeView::onSlotClicked(const QModelIndex &index)
{
#if 0
    QString szBox = "";
    QString szPort = "";
    QString szDevice = "";
    QString szGroup = "";
    QStandardItemModel *pModel = dynamic_cast<QStandardItemModel *>(this->model());
    if(pModel != Q_NULLPTR) {
        QStandardItem *pItem = pModel->itemFromIndex(index);
        if(pItem != Q_NULLPTR) {
            QString szTreeItemType = pItem->data(Qt::UserRole + 1).toString();
            QStringList listVal = pItem->data(Qt::UserRole + 2).toStringList();
            if(listVal.count() > 3) {
                m_szCurBoxName = listVal.at(0);
                m_szCurPortName = listVal.at(1);
                m_szCurDeviceName = listVal.at(2);
                m_szCurTagGroupName = listVal.at(3);
            }
            else {
                m_szCurBoxName = "";
                m_szCurPortName = "";
                m_szCurDeviceName = "";
                m_szCurTagGroupName = "";
            }
            if(szTreeItemType == QString("DEVICE") || szTreeItemType == QString("GROUP")) {
                if(listVal.count() == 4) {
                    szBox = listVal.at(0);
                    szPort = listVal.at(1);
                    szDevice = listVal.at(2);
                    szGroup = listVal.at(3);
                }
            }
        }
    }

    if(m_bDoingMonitor) {
        Device *pDeviceObj = ConfigData::getInstance()->getCommDeviceObj(currentBoxName(), currentDeviceName());
        bool bMasterDevice = true;
        if(pDeviceObj != Q_NULLPTR) {
            CollectorBox *pBoxObj = ConfigData::getInstance()->getBoxObj(currentBoxName());
            QString szCfgFileName = QString("%1.xml").arg(pBoxObj->m_szDevModelType);
            bMasterDevice = DeviceConfigReader::instance()->isMasterProtocol(szCfgFileName, pDeviceObj->m_szDeviceModel);
        }
        if(!bMasterDevice) {
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setWindowTitle(tr("提示"));
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setButtonText (QMessageBox::Ok, QString("确定"));
            msgBox.setText("从站设备不支持监视功能!");
            msgBox.exec();
            return;
        }
    }
    emit sigUpdateTagTable(szBox, szPort, szDevice, szGroup);
#endif
}

///
/// \brief ProjectTreeView::onSlotDoubleClicked
/// \details 叶节点被双击
/// \param index
///
void ProjectTreeView::onSlotDoubleClicked(const QModelIndex &index)
{
#if 0
    QStandardItemModel *pModel = dynamic_cast<QStandardItemModel *>(this->model());
    if(pModel != Q_NULLPTR) {
        QStandardItem *pItem = pModel->itemFromIndex(index);
        if(pItem != Q_NULLPTR) {
            QString szTreeItemType = pItem->data(Qt::UserRole + 1).toString();
            QStringList listVal = pItem->data(Qt::UserRole + 2).toStringList();
            if(listVal.count() > 3) {
                m_szCurBoxName = listVal.at(0);
                m_szCurPortName = listVal.at(1);
                m_szCurDeviceName = listVal.at(2);
                m_szCurTagGroupName = listVal.at(3);
            }
            else {
                m_szCurBoxName = "";
                m_szCurPortName = "";
                m_szCurDeviceName = "";
                m_szCurTagGroupName = "";
            }
            if(szTreeItemType == QString("PROJECT")) { // 鼠标双击 "工程名称"
                onSetProjectName();
            }
            else if(szTreeItemType == QString("BOX")) { // 鼠标双击 "BOX名称"
                onEditDeviceProperty();
            }
            else if(szTreeItemType == QString("RS232") || szTreeItemType == QString("RS485")) { // 鼠标双击 "端口RS232名称", 鼠标右键单击 "端口RS485名称"
                onSetSerialPortParameter();
            }
            else if(szTreeItemType == QString("WAN") || szTreeItemType == QString("LAN")) { // 鼠标双击 "端口WAN名称", 鼠标右键单击 "端口LAN名称"
                onSetNetWorkParameter();
            }
            else if(szTreeItemType == QString("DEVICE")) { // 鼠标双击 "通讯设备名称"
                onEditCommDeviceProperty();
            }
            else if(szTreeItemType == QString("GROUP")) { // 鼠标双击 "变量组名称"
                onEditTagGroupProperty();
            }
            updateUI();
        }
    }
#endif
}


///
/// \brief ProjectTreeView::updateUI
/// \details 更新树形控件
///
void ProjectTreeView::updateUI()
{
    this->setHeaderHidden(true);

    QStandardItemModel *pModelObj = dynamic_cast<QStandardItemModel *>(this->model());
    if(pModelObj == Q_NULLPTR) return;
    pModelObj->clear();

    m_pProjectItemObj = new QStandardItem(QIcon(":/images/pj_pro.png"), tr("未创建工程"));
    m_pProjectItemObj->setEditable(false);
    m_pSystemParametersObj = new QStandardItem(QIcon(":/images/pj_sys.png"), tr("系统参数"));
    m_pSystemParametersObj->setEditable(false);
    m_pProjectItemObj->appendRow(m_pSystemParametersObj);

    //////////////////////////////////////////////////////

    m_pCommunicationDeviceObj = new QStandardItem(QIcon(":/images/pj_sys.png"), tr("通讯设备"));
    m_pCommunicationDeviceObj->setEditable(false);
    m_pComDeviceObj = new QStandardItem(QIcon(":/images/pj_com.png"), tr("串口设备"));
    m_pComDeviceObj->setEditable(false);
    m_pCommunicationDeviceObj->appendRow(m_pComDeviceObj);
    m_pNetDeviceObj = new QStandardItem(QIcon(":/images/pj_net.png"), tr("网络设备"));
    m_pNetDeviceObj->setEditable(false);
    m_pCommunicationDeviceObj->appendRow(m_pNetDeviceObj);

#if 0
    m_pBusDeviceObj = new QStandardItem(QIcon(":/images/pj_bus.png"), tr("总线设备"));
    m_pBusDeviceObj->setEditable(false);
    m_pCommunicationDeviceObj->appendRow(m_pBusDeviceObj);
    m_pOPCDeviceObj = new QStandardItem(QIcon(":/images/pj_opc.png"), tr("OPC设备"));
    m_pOPCDeviceObj->setEditable(false);
    m_pCommunicationDeviceObj->appendRow(m_pOPCDeviceObj);
#endif

    m_pProjectItemObj->appendRow(m_pCommunicationDeviceObj);

    m_pDataBaseConfigObj = new QStandardItem(QIcon(":/images/pj_sys.png"), tr("变量管理"));
    m_pDataBaseConfigObj->setEditable(false);
    m_pDevTagObj = new QStandardItem(QIcon(":/images/pj_zone.png"), tr("设备变量"));
    m_pDevTagObj->setEditable(false);

    m_pDataBaseConfigObj->appendRow(m_pDevTagObj);
    m_pTmpVariableObj = new QStandardItem(QIcon(":/images/pj_zone.png"), tr("中间变量"));
    m_pTmpVariableObj->setEditable(false);
    m_pDataBaseConfigObj->appendRow(m_pTmpVariableObj);
    m_pSysVariableObj = new QStandardItem(QIcon(":/images/pj_zone.png"), tr("系统变量"));
    m_pSysVariableObj->setEditable(false);
    m_pDataBaseConfigObj->appendRow(m_pSysVariableObj);
    m_pProjectItemObj->appendRow(m_pDataBaseConfigObj);

    m_pDataBaseManagerObj = new QStandardItem(QIcon(":/images/pj_sys.png"), tr("数据库管理"));
    m_pDataBaseManagerObj->setEditable(false);
    m_pRealTimeDatabaseObj = new QStandardItem(QIcon(":/images/db_rtdbview.png"), tr("实时数据库"));
    m_pRealTimeDatabaseObj->setEditable(false);
    m_pDataBaseManagerObj->appendRow(m_pRealTimeDatabaseObj);
    m_pHistoryDatabaseObj = new QStandardItem(QIcon(":/images/db_hisdbview.png"), tr("历史数据库"));
    m_pHistoryDatabaseObj->setEditable(false);
    m_pDataBaseManagerObj->appendRow(m_pHistoryDatabaseObj);
    m_pProjectItemObj->appendRow(m_pDataBaseManagerObj);

    m_pLogicProgramObj = new QStandardItem(QIcon(":/images/pm_script.png"), tr("逻辑编程"));
    m_pLogicProgramObj->setEditable(false);

    m_pScriptEditorObj = new QStandardItem(QIcon(":/images/pj_script.png"), tr("脚本编辑器"));
    m_pScriptEditorObj->setEditable(false);
    m_pLogicProgramObj->appendRow(m_pScriptEditorObj);
    m_pProjectItemObj->appendRow(m_pLogicProgramObj);

    pModelObj->appendRow(m_pProjectItemObj);
    this->expandAll();
}

/**
 * @brief ProjectTreeView::getDevTagGroupCount
 * @details 获取设备变量组的个数
 * @return 设备变量组的个数
 */
int ProjectTreeView::getDevTagGroupCount()
{
    return (m_pDevTagObj != Q_NULLPTR) ? m_pDevTagObj->rowCount() : 0;
}


/**
 * @brief ProjectTreeView::getProjectName
 * @details 获取工程名称
 * @return 工程名称
 */
QString ProjectTreeView::getProjectName()
{
    return (m_pProjectItemObj != Q_NULLPTR) ? m_pProjectItemObj->text() : 0;
}

/**
 * @brief ProjectTreeView::setProjectName
 * @details 设置工程名称
 * @param szName 工程名称
 */
void ProjectTreeView::setProjectName(const QString &szName)
{
    if(m_pProjectItemObj != Q_NULLPTR) {
        m_pProjectItemObj->setText(szName);
    }
}


/**
 * @brief ProjectTreeView::updateDeviceTagGroup
 * @details 更新设备变量组
 */
void ProjectTreeView::updateDeviceTagGroup()
{
    if(m_pDevTagObj == Q_NULLPTR) return;

    while(!m_pDevVariableTabListObj.empty())
        m_pDevVariableTabListObj.takeFirst();
    m_pDevTagObj->removeRows(0, m_pDevTagObj->rowCount());

    TagIOGroup &tagIOGroup = ProjectData::getInstance()->tagIOGroup_;
    tagIOGroup.load(ProjectData::getInstance()->dbData_);

    foreach (TagIOGroupDBItem *pObj, tagIOGroup.listTagIOGroupDBItem_) {
        QStandardItem *pDevVarTab = new QStandardItem(QIcon(":/images/pj_zone.png"), pObj->m_szShowName);
        pDevVarTab->setEditable(false);
        m_pDevVariableTabListObj.append(pDevVarTab);
        m_pDevTagObj->appendRow(pDevVarTab);
    }
    QApplication::processEvents();

    qDeleteAll(tagIOGroup.listTagIOGroupDBItem_);
    tagIOGroup.listTagIOGroupDBItem_.clear();
}


/**
 * @brief ProjectTreeView::onSlotTagIOGroupAdd
 * @details 增加组
 */
void ProjectTreeView::onSlotTagIOGroupAdd()
{
    NewVariableGroupDialog *pDlg = new NewVariableGroupDialog();
    pDlg->SetDialogName("新建数据组");
    pDlg->SetLabelName("数据组名：");

    if (pDlg->exec() == QDialog::Accepted) {
        TagIOGroup &tagIOGroup = ProjectData::getInstance()->tagIOGroup_;
        TagIOGroupDBItem *pObj = new TagIOGroupDBItem();
        pObj->m_id = tagIOGroup.getGroupCount(ProjectData::getInstance()->dbData_) + 1;
        pObj->m_szGroupName = QString("group%1").arg(pObj->m_id);
        pObj->m_szShowName = pDlg->GetGroupName();
        tagIOGroup.saveTagTmpDBItem(ProjectData::getInstance()->dbData_, pObj);
        updateDeviceTagGroup();

        QString szTitleNew = QString("%1%2%3").arg("设备变量").arg("-").arg(pDlg->GetGroupName());
        emit sigNotifySetWindowSetTitle(szTitleNew);

        if(pObj != Q_NULLPTR) {
            delete pObj;
            pObj = Q_NULLPTR;
        }
    }
}


/**
 * @brief ProjectTreeView::onSlotTagIOGroupRename
 * @details 重命名组
 */
void ProjectTreeView::onSlotTagIOGroupRename()
{
    QModelIndex index = this->currentIndex();
    QStandardItemModel *pModelObj = dynamic_cast<QStandardItemModel *>(this->model());
    QStandardItem *pItemObj = pModelObj->itemFromIndex(index);
    QString szText = pItemObj->text();

    NewVariableGroupDialog *pDlg = new NewVariableGroupDialog();
    pDlg->SetDialogName("新建数据组");
    pDlg->SetLabelName("数据组名：");
    pDlg->SetGroupName(szText);
    if (pDlg->exec() == QDialog::Accepted) {
        TagIOGroup &tagIOGroup = ProjectData::getInstance()->tagIOGroup_;
        TagIOGroupDBItem *pObj = tagIOGroup.getGroupObjByShowName(ProjectData::getInstance()->dbData_, szText);

        QString szTitleNew = QString("%1%2%3")
                .arg("设备变量")
                .arg("-")
                .arg(pDlg->GetGroupName());
        emit sigNotifySetWindowSetTitle(szTitleNew);

        pObj->m_szShowName = pDlg->GetGroupName();
        tagIOGroup.saveTagTmpDBItem(ProjectData::getInstance()->dbData_, pObj);
        if(pObj != Q_NULLPTR) {
            delete pObj;
            pObj = Q_NULLPTR;
        }
        updateDeviceTagGroup();
    }
}


/**
 * @brief ProjectTreeView::onSlotTagIODeleteGroup
 * @details 删除组
 */
void ProjectTreeView::onSlotTagIODeleteGroup()
{
    QModelIndex ModelIndex = this->selectionModel()->currentIndex();
    QStandardItemModel *pModelObj = dynamic_cast<QStandardItemModel *>(this->model());
    QStandardItem *pItemObj = pModelObj->itemFromIndex(ModelIndex);

    TagIOGroup &tagIOGroup = ProjectData::getInstance()->tagIOGroup_;
    TagIOGroupDBItem *pObj = tagIOGroup.getGroupObjByShowName(ProjectData::getInstance()->dbData_, pItemObj->text());
    tagIOGroup.del(ProjectData::getInstance()->dbData_, pObj);
    if(pObj != Q_NULLPTR) {
        delete pObj;
        pObj = Q_NULLPTR;
    }
    updateDeviceTagGroup();
}


/**
 * @brief ProjectTreeView::onSlotTagIOGroupCopy
 * @details 复制组
 */
void ProjectTreeView::onSlotTagIOGroupCopy()
{
    QModelIndex ModelIndex = this->selectionModel()->currentIndex();
    QStandardItemModel *pModelObj = dynamic_cast<QStandardItemModel *>(this->model());
    QStandardItem *pItemObj = pModelObj->itemFromIndex(ModelIndex);

    TagIOGroup &tagIOGroup = ProjectData::getInstance()->tagIOGroup_;
    // check the same name first
    QString szName = QString("复制_%1").arg(pItemObj->text());
    int iCnt = tagIOGroup.getGroupCountByShowName(ProjectData::getInstance()->dbData_, szName);
    if(iCnt > 0) {
        QMessageBox::information(this, "系统提示", "同名文件存在，请先修改名称！");
        return;
    }

    TagIOGroupDBItem *pObj = new TagIOGroupDBItem();
    pObj->m_id = tagIOGroup.getGroupCount(ProjectData::getInstance()->dbData_) + 1;
    pObj->m_szGroupName = QString("group%1").arg(pObj->m_id);
    pObj->m_szShowName = szName;
    tagIOGroup.saveTagTmpDBItem(ProjectData::getInstance()->dbData_, pObj);
    updateDeviceTagGroup();
    if(pObj != Q_NULLPTR) {
        delete pObj;
        pObj = Q_NULLPTR;
    }
}



