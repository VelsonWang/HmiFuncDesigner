#include "ProjectTreeView.h"
#include <QStandardItem>
#include <QModelIndex>
#include <QApplication>
#include <QMessageBox>
#include <QDebug>
#include "qsoftcore.h"

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

///
/// \brief ProjectTreeView::onSlotClicked
/// \details 叶节点被单击
/// \param index
///
void ProjectTreeView::onSlotClicked(const QModelIndex &index)
{
    QStandardItemModel *pModelObj = dynamic_cast<QStandardItemModel *>(this->model());
    if(pModelObj != Q_NULLPTR) {
        QStandardItem *pItemObj = pModelObj->itemFromIndex(index);
        if(pItemObj != Q_NULLPTR) {
            QVariant userDat = pItemObj->data(Qt::UserRole + 1);
            emit sigNotifyClicked(userDat.toStringList().join("|"));
        }
    }
}

///
/// \brief ProjectTreeView::onSlotDoubleClicked
/// \details 叶节点被双击
/// \param index
///
void ProjectTreeView::onSlotDoubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index)

}


///
/// \brief ProjectTreeView::updateUI
/// \details 更新树形控件
///
void ProjectTreeView::updateUI()
{
    this->setHeaderHidden(true);
    QStringList szListUserData;

    QStandardItemModel *pModelObj = dynamic_cast<QStandardItemModel *>(this->model());
    if(pModelObj == Q_NULLPTR) return;
    pModelObj->clear();

    m_pProjectItemObj = new QStandardItem(QIcon(":/images/pj_pro.png"), tr("未创建工程"));
    m_pProjectItemObj->setEditable(false);
    szListUserData.clear();
    szListUserData << QString("Project").toUpper()
                   << m_pProjectItemObj->text()
                   << "";
    m_pProjectItemObj->setData(szListUserData, Qt::UserRole + 1);

    m_pSystemParametersObj = new QStandardItem(QIcon(":/images/pj_sys.png"), tr("系统参数"));
    m_pSystemParametersObj->setEditable(false);
    szListUserData.clear();
    szListUserData << QString("SystemParameters").toUpper()
                   << m_pSystemParametersObj->text()
                   << "";
    m_pSystemParametersObj->setData(szListUserData, Qt::UserRole + 1);
    m_pProjectItemObj->appendRow(m_pSystemParametersObj);

    //////////////////////////////////////////////////////

    m_pCommunicationDeviceObj = new QStandardItem(QIcon(":/images/pj_sys.png"), tr("通讯设备"));
    m_pCommunicationDeviceObj->setEditable(false);
    szListUserData.clear();
    szListUserData << QString("CommunicationDevice").toUpper()
                   << m_pCommunicationDeviceObj->text()
                   << QString("CommunicationDevice").toUpper();
    m_pCommunicationDeviceObj->setData(szListUserData, Qt::UserRole + 1);

    m_pComDeviceObj = new QStandardItem(QIcon(":/images/pj_com.png"), tr("串口设备"));
    m_pComDeviceObj->setEditable(false);
    szListUserData.clear();
    szListUserData << QString("CommunicationDevice").toUpper()
                   << m_pComDeviceObj->text()
                   << QString("ComDevice").toUpper();
    m_pComDeviceObj->setData(szListUserData, Qt::UserRole + 1);
    m_pCommunicationDeviceObj->appendRow(m_pComDeviceObj);

    m_pNetDeviceObj = new QStandardItem(QIcon(":/images/pj_net.png"), tr("网络设备"));
    m_pNetDeviceObj->setEditable(false);
    szListUserData.clear();
    szListUserData << QString("CommunicationDevice").toUpper()
                   << m_pNetDeviceObj->text()
                   << QString("NetDevice").toUpper();
    m_pNetDeviceObj->setData(szListUserData, Qt::UserRole + 1);
    m_pCommunicationDeviceObj->appendRow(m_pNetDeviceObj);

    m_pProjectItemObj->appendRow(m_pCommunicationDeviceObj);

    m_pTagMgrObj = new QStandardItem(QIcon(":/images/pj_sys.png"), tr("变量管理"));
    m_pTagMgrObj->setEditable(false);
    szListUserData.clear();
    szListUserData << QString("TagMgr").toUpper()
                   << m_pTagMgrObj->text()
                   << "";
    m_pTagMgrObj->setData(szListUserData, Qt::UserRole + 1);
    m_pProjectItemObj->appendRow(m_pTagMgrObj);

    m_pDataBaseManagerObj = new QStandardItem(QIcon(":/images/pj_sys.png"), tr("数据库管理"));
    m_pDataBaseManagerObj->setEditable(false);
    szListUserData.clear();
    szListUserData << QString("DataBaseManager").toUpper() << m_pDataBaseManagerObj->text();
    m_pDataBaseManagerObj->setData(szListUserData, Qt::UserRole + 1);

    m_pRealTimeDatabaseObj = new QStandardItem(QIcon(":/images/db_rtdbview.png"), tr("实时数据库"));
    m_pRealTimeDatabaseObj->setEditable(false);
    szListUserData.clear();
    szListUserData << QString("RTDBWin").toUpper()
                   << m_pRealTimeDatabaseObj->text()
                   << "";
    m_pRealTimeDatabaseObj->setData(szListUserData, Qt::UserRole + 1);
    m_pDataBaseManagerObj->appendRow(m_pRealTimeDatabaseObj);

    //m_pHistoryDatabaseObj = new QStandardItem(QIcon(":/images/db_hisdbview.png"), tr("历史数据库"));
    //m_pHistoryDatabaseObj->setEditable(false);
    //szListUserData.clear();
    //szListUserData << QString("HistoryDatabase").toUpper() << m_pHistoryDatabaseObj->text();
    //m_pHistoryDatabaseObj->setData(szListUserData, Qt::UserRole + 1);
    //m_pDataBaseManagerObj->appendRow(m_pHistoryDatabaseObj);
    m_pProjectItemObj->appendRow(m_pDataBaseManagerObj);

    m_pLogicProgramObj = new QStandardItem(QIcon(":/images/pm_script.png"), tr("逻辑编程"));
    m_pLogicProgramObj->setEditable(false);
    szListUserData.clear();
    szListUserData << QString("LogicProgram").toUpper()
                   << m_pLogicProgramObj->text()
                   << "";
    m_pLogicProgramObj->setData(szListUserData, Qt::UserRole + 1);

    m_pScriptEditorObj = new QStandardItem(QIcon(":/images/pj_script.png"), tr("脚本编辑器"));
    m_pScriptEditorObj->setEditable(false);
    szListUserData.clear();
    szListUserData << QString("ScriptEditor").toUpper()
                   << m_pScriptEditorObj->text()
                   << "";
    m_pScriptEditorObj->setData(szListUserData, Qt::UserRole + 1);
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
    return (m_pProjectItemObj != Q_NULLPTR) ? m_pProjectItemObj->text() : "";
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





