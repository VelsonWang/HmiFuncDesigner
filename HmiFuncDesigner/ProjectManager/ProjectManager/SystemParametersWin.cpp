#include "SystemParametersWin.h"
#include "ui_SystemParametersWin.h"
#include "NewProjectDialog.h"
#include "NetSettingDialog.h"
#include "DatabaseSettingDialog.h"
#include "UserAuthorityDialog.h"
#include "ProjectMgrUtils.h"
#include <QFileInfo>
#include <QApplication>
#include <QFileInfo>
#include <QDir>
#include <QPluginLoader>

SystemParametersWin::SystemParametersWin(QWidget *parent,
                                         const QString &itemName,
                                         const QString &projName) :
    ChildBase(parent, itemName, projName),
    ui(new Ui::SystemParametersWin)
{
    ui->setupUi(this);
    this->setWindowTitle(itemName);
    setAttribute(Qt::WA_DeleteOnClose);

    ui->listViewProject->setViewMode(QListView::IconMode);
    ui->listViewProject->setIconSize(QSize(32, 32));
    ui->listViewProject->setGridSize(QSize(120, 120));
    ui->listViewProject->setWordWrap(true);
    ui->listViewProject->setSpacing(20);
    ui->listViewProject->setResizeMode(QListView::Adjust);
    ui->listViewProject->setMovement(QListView::Static);

    pListViewProjectModel = new QStandardItemModel();
    QStandardItem *pRunSystem = new QStandardItem(QIcon(":/images/pm_runcon.png"), tr("运行系统"));
    pRunSystem->setEditable(false);
    pListViewProjectModel->appendRow(pRunSystem);

    QStandardItem *pNetSetting = new QStandardItem(QIcon(":/images/pm_lancon.png"), tr("组网设置"));
    pNetSetting->setEditable(false);
    pListViewProjectModel->appendRow(pNetSetting);

    QStandardItem *pDataBaseSetting = new QStandardItem(QIcon(":/images/pm_db.png"), tr("数据库设置"));
    pDataBaseSetting->setEditable(false);
    pListViewProjectModel->appendRow(pDataBaseSetting);

    QStandardItem *pUserAuthority = new QStandardItem(QIcon(":/images/pm_user.png"), tr("用户权限"));
    pUserAuthority->setEditable(false);
    pListViewProjectModel->appendRow(pUserAuthority);

    ui->listViewProject->setModel(pListViewProjectModel);
}

SystemParametersWin::~SystemParametersWin()
{
    delete ui;
}

void SystemParametersWin::on_listViewProject_doubleClicked(const QModelIndex &index)
{
    QStandardItem *item = pListViewProjectModel->itemFromIndex(index);

    if(m_strProjectName == "")
        return;

    QString strProjectPath = ProjectMgrUtils::getProjectPath(m_strProjectName);
    if(item->text() == tr("运行系统")) {
        NewProjectDialog *pNewProjectDlg = new NewProjectDialog(this, strProjectPath);
        pNewProjectDlg->load();
        if(pNewProjectDlg->exec() == QDialog::Accepted) {
            pNewProjectDlg->save();
        }
    } else if(item->text() == tr("组网设置")) {
        NetSettingDialog *pNetSettingDialog = new NetSettingDialog(this, strProjectPath);
        pNetSettingDialog->load();
        if(pNetSettingDialog->exec() == QDialog::Accepted) {
            pNetSettingDialog->save();
        }
    } else if(item->text() == tr("数据库设置")) {
        DatabaseSettingDialog *pDatabaseSettingDialog = new DatabaseSettingDialog(this, strProjectPath);
        pDatabaseSettingDialog->load();
        if(pDatabaseSettingDialog->exec() == QDialog::Accepted) {
            pDatabaseSettingDialog->save();
        }
    } else if(item->text() == tr("用户权限")) {
        UserAuthorityDialog *pUserAuthorityDialog = new UserAuthorityDialog(this, strProjectPath);
        pUserAuthorityDialog->load();
        if(pUserAuthorityDialog->exec() == QDialog::Accepted) {
            pUserAuthorityDialog->save();
        }
    }

}

/*
 * 打开文件
 */
void SystemParametersWin::open()
{

}

/*
 * 保存文件
 */
void SystemParametersWin::save()
{

}

/*
* 显示大图标
*/
void SystemParametersWin::showLargeIcon()
{
    ui->listViewProject->setIconSize(QSize(32, 32));
}

/*
* 显示小图标
*/
void SystemParametersWin::showSmallIcon()
{
    ui->listViewProject->setIconSize(QSize(24, 24));
}


