#include "systemparameters.h"
#include "newprojectdialog.h"
#include "netsettingdialog.h"
#include "databasesettingdialog.h"
#include "userauthoritydialog.h"
#include <QFileInfo>
#include <QApplication>
#include <QFileInfo>
#include <QDir>
#include <QPluginLoader>

SystemParameters::SystemParameters(QWidget *parent) : QWidget(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);

    m_pListViewProjectObj = new ListViewEx(this);
    m_pListViewProjectObj->setViewMode(QListView::IconMode);
    m_pListViewProjectObj->setViewportMargin(0, 0, 0, 0);
    m_pListViewProjectObj->setIconSize(QSize(32, 32));
    m_pListViewProjectObj->setGridSize(QSize(120, 120));
    m_pListViewProjectObj->setWordWrap(true);
    m_pListViewProjectObj->setSpacing(20);
    m_pListViewProjectObj->setResizeMode(QListView::Adjust);
    m_pListViewProjectObj->setMovement(QListView::Static);
    connect(m_pListViewProjectObj, &QAbstractItemView::doubleClicked,
            this, &SystemParameters::onSlotListViewProjectDoubleClicked);

    QVBoxLayout *pVLayoutObj = new QVBoxLayout(this);
    pVLayoutObj->setSpacing(0);
    pVLayoutObj->setContentsMargins(0, 0, 0, 0);
    pVLayoutObj->addWidget(m_pListViewProjectObj);
    this->setLayout(pVLayoutObj);

    m_pListViewProjectModelObj = new QStandardItemModel();
    QStandardItem *pRunSystemObj = new QStandardItem(QIcon(":/images/pm_runcon.png"), tr("运行系统"));
    pRunSystemObj->setEditable(false);
    m_pListViewProjectModelObj->appendRow(pRunSystemObj);

    QStandardItem *pNetSettingObj = new QStandardItem(QIcon(":/images/pm_lancon.png"), tr("组网设置"));
    pNetSettingObj->setEditable(false);
    m_pListViewProjectModelObj->appendRow(pNetSettingObj);

    QStandardItem *pDataBaseSettingObj = new QStandardItem(QIcon(":/images/pm_db.png"), tr("数据库设置"));
    pDataBaseSettingObj->setEditable(false);
    m_pListViewProjectModelObj->appendRow(pDataBaseSettingObj);

    QStandardItem *pUserAuthorityObj = new QStandardItem(QIcon(":/images/pm_user.png"), tr("用户权限"));
    pUserAuthorityObj->setEditable(false);
    m_pListViewProjectModelObj->appendRow(pUserAuthorityObj);

    m_pListViewProjectObj->setModel(m_pListViewProjectModelObj);
}

SystemParameters::~SystemParameters()
{
    if(m_pListViewProjectModelObj != NULL) {
        delete m_pListViewProjectModelObj;
        m_pListViewProjectModelObj = NULL;
    }
    if(m_pListViewProjectObj != NULL) {
        delete m_pListViewProjectObj;
        m_pListViewProjectObj = NULL;
    }
}

void SystemParameters::onSlotListViewProjectDoubleClicked(const QModelIndex &index)
{
    QStandardItem *pItemObj = m_pListViewProjectModelObj->itemFromIndex(index);
    if(pItemObj->text() == tr("运行系统")) {
        NewProjectDialog *pNewProjectDlgObj = new NewProjectDialog(this);
        pNewProjectDlgObj->load();
        if(pNewProjectDlgObj->exec() == QDialog::Accepted) {
            pNewProjectDlgObj->save();
        }
    } else if(pItemObj->text() == tr("组网设置")) {
        NetSettingDialog *pNetSettingDialogObj = new NetSettingDialog(this);
        pNetSettingDialogObj->load();
        if(pNetSettingDialogObj->exec() == QDialog::Accepted) {
            pNetSettingDialogObj->save();
        }
    } else if(pItemObj->text() == tr("数据库设置")) {
        DatabaseSettingDialog *pDBSettingDlgObj = new DatabaseSettingDialog(this);
        pDBSettingDlgObj->load();
        if(pDBSettingDlgObj->exec() == QDialog::Accepted) {
            pDBSettingDlgObj->save();
        }
    } else if(pItemObj->text() == tr("用户权限")) {
        UserAuthorityDialog *pUserAuthorityDlgObj = new UserAuthorityDialog(this);
        pUserAuthorityDlgObj->load();
        if(pUserAuthorityDlgObj->exec() == QDialog::Accepted) {
            pUserAuthorityDlgObj->save();
        }
    }

}




