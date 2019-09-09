#include "DrawPageWin.h"
#include <QAction>
#include <QDebug>
#include <QFile>
#include <QInputDialog>
#include <QMenu>
#include <QMessageBox>
#include <QModelIndex>
#include <QProcess>
#include "../Public/Public.h"
#include "ConfigUtils.h"
#include "DrawListUtils.h"
#include "Helper.h"
#include "ProjectData.h"
#include "ProjectInfoManager.h"
#include "ProjectMgrUtils.h"
#include "Singleton.h"
#include "ui_DrawPageWin.h"

DrawPageWin::DrawPageWin(QWidget *parent, const QString &itemName,
                         const QString &projName)
    : ChildBase(parent, itemName, projName),
      ui(new Ui::DrawPageWin)
{
    ui->setupUi(this);
    this->setWindowTitle(itemName);
    m_strItemName = itemName;
    szProjPath_ = ProjectMgrUtils::getProjectPath(projName);
    szProjName_ = ProjectMgrUtils::getProjectNameWithOutSuffix(m_strProjectName);
    setContextMenuPolicy(Qt::DefaultContextMenu);
    pListDrawPageModel = new QStandardItemModel();
    ui->listViewDrawPage->setModel(pListDrawPageModel);
}

DrawPageWin::~DrawPageWin()
{
    delete ui;
    if (pListDrawPageModel != nullptr) {
        delete pListDrawPageModel;
        pListDrawPageModel = nullptr;
    }
}

void DrawPageWin::init()
{
    if (!m_strProjectName.isEmpty()) {
        szProjPath_ = ProjectMgrUtils::getProjectPath(m_strProjectName);
        open();
        ListViewUpdate();
        connect(&fileSystemWatcher_, SIGNAL(fileChanged(QString)), this, SLOT(onFileChanged(QString)));
        fileSystemWatcher_.addPath(szProjPath_ + "/DrawList.xml");
    }
}

/*
*  打开文件
*/
void DrawPageWin::open()
{
    DrawListUtils::loadDrawList(szProjPath_);
}

/*
* 保存文件
*/
void DrawPageWin::save()
{
    if (getModifiedFlag()) {
        setModifiedFlag(false);
        DrawListUtils::saveDrawList(szProjPath_);
    }
}

/*
* 显示大图标
*/
void DrawPageWin::showLargeIcon()
{
    ui->listViewDrawPage->setIconSize(QSize(32, 32));
}

/*
* 显示小图标
*/
void DrawPageWin::showSmallIcon()
{
    ui->listViewDrawPage->setIconSize(QSize(24, 24));
}

/**
 * @brief DrawPageWin::createEmptyGraphpage
 * @details 创建空的画面页
 * @param projPath 工程路径
 * @param graphPageName 画面名称
 * @param width 画面宽度
 * @param height 画面高度
 */
void DrawPageWin::createEmptyGraphpage(const QString &projPath,
                                       const QString &graphPageName,
                                       int width,
                                       int height)
{
    QString fileName = projPath + "/" + graphPageName + ".drw";
    QString szContent = QString(
                "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                "<graphPage fileName=\"%1.drw\" graphPageId=\"%1\" "
                "width=\"%2\" height=\"%3\" background=\"#ffffff\">\n"
                "</graphPage>")
            .arg(graphPageName)
            .arg(QString::number(width))
            .arg(QString::number(height));

    Helper::writeString(fileName, szContent);
}

/*
* 新建画面
*/
void DrawPageWin::NewDrawPage()
{
    int last = DrawListUtils::getMaxDrawPageNum("draw");
    QString szGraphPageName = QString("draw%1").arg(last);

    ProjectInfoManager &projInfoMgr = ProjectData::getInstance()->projInfoMgr_;
    projInfoMgr.load(ProjectData::getInstance()->dbData_);
    int width = projInfoMgr.getGraphPageWidth();
    int height = projInfoMgr.getGraphPageHeight();

    createEmptyGraphpage(szProjPath_, szGraphPageName, width, height);
    DrawListUtils::drawList_.append(szGraphPageName);
    setModifiedFlag(true);
    save();
    ListViewUpdate();
}

/*
* 修改画面名称
*/
void DrawPageWin::ModifyDrawPage()
{
    QModelIndex idx = ui->listViewDrawPage->selectionModel()->currentIndex();
    QStandardItem *item = this->pListDrawPageModel->itemFromIndex(idx);
    QString strOldName = item->text();

    QInputDialog dlg(this);
    dlg.setWindowTitle(tr("修改画面名称"));
    dlg.setLabelText(tr("请输入画面名称"));
    dlg.setOkButtonText(tr("确定"));
    dlg.setCancelButtonText(tr("取消"));
    dlg.setTextValue(item->text());

reinput:
    if (dlg.exec() == QDialog::Accepted) {
        QString strNewName = dlg.textValue();

        if (strNewName == "")
            goto reinput;

        for (int i = 0; i < DrawListUtils::drawList_.count(); i++) {
            if (strOldName == DrawListUtils::drawList_.at(i)) {
                DrawListUtils::drawList_.replace(i, strNewName);
                // rename file
                QString oldName = szProjPath_ + "/" + strOldName + ".drw";
                QString newName = szProjPath_ + "/" + strNewName + ".drw";
                QFile::rename(oldName, newName);
                break;
            }
        }

        setModifiedFlag(true);
        save();
        ListViewUpdate();
    }
}

/*
* 删除画面
*/
void DrawPageWin::DeleteDrawPage()
{
    QModelIndex idx = ui->listViewDrawPage->selectionModel()->currentIndex();
    QStandardItem *item = this->pListDrawPageModel->itemFromIndex(idx);
    QString strName = item->text();

    for (int i = 0; i < DrawListUtils::drawList_.count(); i++) {
        if (strName == DrawListUtils::drawList_.at(i)) {
            DrawListUtils::drawList_.removeAt(i);
            // delete file
            QString fileName = szProjPath_ + "/" + strName + ".drw";
            QFile file(fileName);
            if (file.exists())
                file.remove();
            break;
        }
    }

    setModifiedFlag(true);
    save();
    ListViewUpdate();
}

/*
* 复制画面
*/
void DrawPageWin::CopyDrawPage()
{
    QModelIndex idx = ui->listViewDrawPage->selectionModel()->currentIndex();
    QStandardItem *item = this->pListDrawPageModel->itemFromIndex(idx);
    szCopyDrawPageFileName_ = item->text();
}

/*
* 粘贴画面
*/
void DrawPageWin::PasteDrawPage()
{
    int last = 0;

regetnum:
    last = DrawListUtils::getMaxDrawPageNum(szCopyDrawPageFileName_);
    QString strDrawPageName = szCopyDrawPageFileName_ + QString("-%1").arg(last);
    if (DrawListUtils::drawList_.contains(strDrawPageName)) {
        szCopyDrawPageFileName_ = strDrawPageName;
        goto regetnum;
    }

    DrawListUtils::drawList_.append(strDrawPageName);

    // copy file
    QString fileName = szProjPath_ + "/" + szCopyDrawPageFileName_ + ".drw";
    QFile file(fileName);

    QString pasteFileName = szProjPath_ + "/" + strDrawPageName + ".drw";
    file.copy(pasteFileName);
    setModifiedFlag(true);
    save();
    ListViewUpdate();
}

/*
* 右键菜单
*/
void DrawPageWin::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event)
    QMenu *pMenu = new QMenu(this);

    QAction *pNewAct = new QAction(QIcon(":/images/icon_new.png"), tr("新建"), this);
    pNewAct->setShortcut(QKeySequence(Qt::ALT + Qt::Key_A));
    pNewAct->setStatusTip(tr("新建(Alt+A)"));
    connect(pNewAct, SIGNAL(triggered()), this, SLOT(NewDrawPage()));
    pMenu->addAction(pNewAct);

    QAction *pModifyAct = new QAction(QIcon(":/images/icon_modify.png"), tr("修改"), this);
    pModifyAct->setShortcut(QKeySequence(Qt::ALT + Qt::Key_M));
    pModifyAct->setStatusTip(tr("修改(Alt+M)"));
    connect(pModifyAct, SIGNAL(triggered()), this, SLOT(ModifyDrawPage()));
    pMenu->addAction(pModifyAct);

    QAction *pDeleteAct = new QAction(QIcon(":/images/icon_delete.png"), tr("删除"), this);
    pDeleteAct->setShortcut(QKeySequence(Qt::ALT + Qt::Key_D));
    pDeleteAct->setStatusTip(tr("删除(Alt+D)"));
    connect(pDeleteAct, SIGNAL(triggered()), this, SLOT(DeleteDrawPage()));
    pMenu->addAction(pDeleteAct);

    QAction *pCopyAct = new QAction(tr("复制"), this);
    pCopyAct->setShortcut(QKeySequence(Qt::ALT + Qt::Key_C));
    pCopyAct->setStatusTip(tr("复制(Alt+C)"));
    connect(pCopyAct, SIGNAL(triggered()), this, SLOT(CopyDrawPage()));
    pMenu->addAction(pCopyAct);

    QAction *pPasteAct = new QAction(tr("粘贴"), this);
    pPasteAct->setShortcut(QKeySequence(Qt::ALT + Qt::Key_V));
    pPasteAct->setStatusTip(tr("粘贴(Alt+V)"));
    connect(pPasteAct, SIGNAL(triggered()), this, SLOT(PasteDrawPage()));
    pMenu->addAction(pPasteAct);

    pMenu->move(cursor().pos());
    pMenu->show();
}

void DrawPageWin::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event)
    QString strProjPath = ProjectMgrUtils::getProjectPath(m_strProjectName);
    DrawListUtils::saveDrawList(strProjPath);
}

void DrawPageWin::ListViewUISetting()
{
    ui->listViewDrawPage->setViewMode(QListView::IconMode);
    ui->listViewDrawPage->setIconSize(QSize(32, 32));
    ui->listViewDrawPage->setGridSize(QSize(60, 60));
    ui->listViewDrawPage->setWordWrap(true);
    ui->listViewDrawPage->setSpacing(10);
    // 设置QListView大小改变时，图标的调整模式，默认是固定的，但可以改成自动调整：
    ui->listViewDrawPage->setResizeMode(QListView::Adjust);
    //设置图标可不可以移动，默认是可移动的，但可以改成静态的：
    ui->listViewDrawPage->setMovement(QListView::Static);
}

void DrawPageWin::ListViewUpdate()
{
    DrawListUtils::loadDrawList(szProjPath_);
    pListDrawPageModel->clear();
    ListViewUISetting();

    QStandardItem *pNewWin = new QStandardItem(QIcon(":/images/drawexec.png"), tr("新建画面"));
    pNewWin->setEditable(false);
    pListDrawPageModel->appendRow(pNewWin);

    for (int i = 0; i < DrawListUtils::drawList_.count(); i++) {
        QStandardItem *pDrawPage = new QStandardItem(QIcon(":/images/drawexec.png"), DrawListUtils::drawList_.at(i));
        pDrawPage->setEditable(false);
        pListDrawPageModel->appendRow(pDrawPage);
    }
}

void DrawPageWin::on_listViewDrawPage_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index)
    QModelIndex idx = ui->listViewDrawPage->selectionModel()->currentIndex();
    QStandardItem *item = this->pListDrawPageModel->itemFromIndex(idx);

    if (item == nullptr)
        return;

    if (item->text() == tr("新建画面")) {
        NewDrawPage();
        return;
    }

    QString fileDrawApplication = "";
#ifdef Q_OS_WIN
    fileDrawApplication = Helper::AppDir() + "/DrawApplication.exe";
#endif

#ifdef Q_OS_LINUX
    fileDrawApplication = Helper::AppDir() + "/DrawApplication";
#endif

    QFile file(fileDrawApplication);
    if (file.exists()) {
        QProcess *process = new QProcess();
        QStringList argv;
        argv << szProjPath_ << szProjName_ << item->text();
        process->start(fileDrawApplication, argv);
    }
}


void DrawPageWin::onFileChanged(const QString &path)
{
    Q_UNUSED(path)
    ListViewUpdate();
}




