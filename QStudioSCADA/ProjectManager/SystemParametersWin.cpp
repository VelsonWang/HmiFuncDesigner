#include "SystemParametersWin.h"
#include "ui_SystemParametersWin.h"
#include "NewProjectDialog.h"
#include "NetSettingDialog.h"
#include "DatabaseSettingDialog.h"
#include "UserAuthorityDialog.h"

#include <QFileInfo>
#include <QApplication>
#include <QFileInfo>
#include <QDir>
#include <QPluginLoader>

#include <QDebug>

SystemParametersWin::SystemParametersWin(QWidget *parent, QString itemName, QString projName) :
    ChildBase(parent, itemName, projName),
    ui(new Ui::SystemParametersWin)
{
    ui->setupUi(this);
    this->setWindowTitle(itemName);
    setAttribute(Qt::WA_DeleteOnClose);    //  子窗口关闭时销毁这个类对象
    isUntitled = true;    // not saved

    /*
    static int sequenceNumber = 1;
    isUntitled = true;
    curFile = tr("未命名文档%1.txt").arg(sequenceNumber++);
    setWindowTitle(curFile + "[*]" + tr(" -多文档编辑器"));
    */

    ui->listViewProject->setViewMode(QListView::IconMode);
    ui->listViewProject->setIconSize(QSize(32, 32));
    ui->listViewProject->setGridSize(QSize(120, 120));
    ui->listViewProject->setWordWrap(true);
    ui->listViewProject->setSpacing(20);
    // 设置QListView大小改变时，图标的调整模式，默认是固定的，但可以改成自动调整：
    ui->listViewProject->setResizeMode(QListView::Adjust);
    //设置图标可不可以移动，默认是可移动的，但可以改成静态的：
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

    /*
    QStandardItem *pMQSetting = new QStandardItem(QIcon(":/images/pm_mq.png"), tr("MQ设置"));
    pMQSetting->setEditable(false);
    pListViewProjectModel->appendRow(pMQSetting);

    QStandardItem *pPageSetting = new QStandardItem(QIcon(":/images/pm_setpage.png"), tr("页面设置"));
    pPageSetting->setEditable(false);
    pListViewProjectModel->appendRow(pPageSetting);
    */

    ui->listViewProject->setModel(pListViewProjectModel);
}

SystemParametersWin::~SystemParametersWin()
{
    delete ui;
}




QString SystemParametersWin::userFriendlyCurrentFile() // 提取文件名
{
    return QFileInfo(curFile).fileName();  // 从文件路径中提取文件名
}

QString SystemParametersWin::currentFile()
{
    return curFile;
}

void SystemParametersWin::setCurrentFile(const QString &fileName)
{
    curFile = QFileInfo(fileName).canonicalFilePath(); // 去除路径中的符号链接,".",".."等
    isUntitled = false;
    setWindowModified(false);
    setWindowTitle(QFileInfo(curFile).fileName() + "[*]");
}

void SystemParametersWin::on_listViewProject_doubleClicked(const QModelIndex &index)
{
    QStandardItem *item = pListViewProjectModel->itemFromIndex(index);
    if(m_strProjectName == "")
        return;
    QString strProjectPath = m_strProjectName.left(m_strProjectName.lastIndexOf("/"));
    if(item->text() == "运行系统")
    {
        NewProjectDialog *pNewProjectDlg = new NewProjectDialog(this);
        pNewProjectDlg->loadFromFile(DATA_SAVE_FORMAT, m_strProjectName);
        if(pNewProjectDlg->exec() == QDialog::Accepted)
        {

        }
    }
    else if(item->text() == "组网设置")
    {
        NetSettingDialog *pNetSettingDialog = new NetSettingDialog(this, strProjectPath);
        if(pNetSettingDialog->exec() == QDialog::Accepted)
        {

        }
    }
    else if(item->text() == "数据库设置")
    {
        DatabaseSettingDialog *pDatabaseSettingDialog = new DatabaseSettingDialog(this, strProjectPath);
        if(pDatabaseSettingDialog->exec() == QDialog::Accepted)
        {

        }
    }
    else if(item->text() == "用户权限")
    {
        UserAuthorityDialog *pUserAuthorityDialog = new UserAuthorityDialog(this, strProjectPath);
        if(pUserAuthorityDialog->exec() == QDialog::Accepted)
        {

        }
    }
    else if(item->text() == "MQ设置")
    {

    }
    else if(item->text() == "页面设置")
    {

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
void SystemParametersWin::ShowLargeIcon()
{
    ui->listViewProject->setIconSize(QSize(32, 32));
}

/*
* 显示小图标
*/
void SystemParametersWin::ShowSmallIcon()
{
    ui->listViewProject->setIconSize(QSize(24, 24));
}


