#include "GraphPageListWidget.h"
#include "Helper.h"
#include "DrawListUtils.h"
#include "ProjectData.h"
#include <QMenu>
#include <QAction>

GraphPageListWidget::GraphPageListWidget(QWidget *parent) : QListWidget(parent)
{
    setContextMenuPolicy(Qt::DefaultContextMenu);

}


/**
 * @brief GraphPageListWidget::contextMenuEvent
 * @details 右键菜单
 */
void GraphPageListWidget::contextMenuEvent(QContextMenuEvent * event)
{
    QMenu *pMenu = new QMenu(this);

    QAction *pNewAct = new QAction(tr("新建"), this);
    pNewAct->setStatusTip(tr("新建画面"));
    connect(pNewAct, SIGNAL(triggered()), this, SLOT(onSlotNewDrawPage()));

    QAction *pRenameAct = new QAction(tr("重命名"), this);
    pRenameAct->setStatusTip(tr("重命名画面"));
    //connect(pRenameAct, SIGNAL(triggered()), this, SLOT(onRenameGraphPage()));

    QAction *pDeleteAct = new QAction(tr("删除"), this);
    pDeleteAct->setStatusTip(tr("删除画面"));
    //connect(pDeleteAct, SIGNAL(triggered()), this, SLOT(onDeleteGraphPage()));

    QAction *pCopyAct = new QAction(tr("复制"), this);
    pCopyAct->setStatusTip(tr("复制画面"));
    //connect(pCopyAct, SIGNAL(triggered()), this, SLOT(onCopyGraphPage()));

    QAction *pPasteAct = new QAction(tr("粘贴"), this);
    pPasteAct->setStatusTip(tr("粘贴画面"));
    pPasteAct->setEnabled(false);
    //connect(pPasteAct, SIGNAL(triggered()), this, SLOT(onPasteGraphPage()));

    QModelIndex index = indexAt(event->pos());
    if(!index.isValid()) { // 单击空白部分
        pMenu->addAction(pNewAct); // 新建
        pMenu->addAction(pPasteAct); // 粘贴
    } else {
        pMenu->addAction(pNewAct); // 新建
        pMenu->addAction(pRenameAct); // 重命名
        pMenu->addAction(pDeleteAct); // 删除
        pMenu->addAction(pCopyAct); // 复制
        pMenu->addAction(pPasteAct); // 粘贴
    }

    pMenu->move(cursor().pos());
    pMenu->show();

    if(!pMenu->isEmpty()) {
        pMenu->move(cursor().pos());
        pMenu->exec();
        pMenu->clear();
    }
    delete pMenu;
}


/**
 * @brief GraphPageListWidget::createEmptyGraphpage
 * @details 创建空的画面页
 * @param graphPageName 画面名称
 * @param width 画面宽度
 * @param height 画面高度
 */
//void GraphPageListWidget::createEmptyGraphpage(const QString &szGraphPageName, int iWidth, int Hheight)
//{
//    QString fileName = m_szProjectPath + "/" + szGraphPageName + ".drw";
//    QString szContent = QString(
//                "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
//                "<graphPage fileName=\"%1.drw\" graphPageId=\"%1\" "
//                "width=\"%2\" height=\"%3\" background=\"#ffffff\">\n"
//                "</graphPage>")
//            .arg(szGraphPageName)
//            .arg(QString::number(iWidth))
//            .arg(QString::number(Hheight));

//    Helper::writeString(fileName, szContent);
//}


/**
 * @brief GraphPageListWidget::onSlotNewDrawPage
 * @details 新建画面
 */
void GraphPageListWidget::onSlotNewDrawPage()
{
    int last = DrawListUtils::getMaxDrawPageNum("draw");
    QString szGraphPageName = QString("draw%1").arg(last);

    ProjectInfoManager &projInfoMgr = ProjectData::getInstance()->projInfoMgr_;
    int iWidth = projInfoMgr.getGraphPageWidth();
    int iHeight = projInfoMgr.getGraphPageHeight();

    //createEmptyGraphpage(szGraphPageName, iWidth, iHeight);
    DrawListUtils::drawList_.append(szGraphPageName);
    this->addItem(szGraphPageName);
    //DrawListUtils::saveDrawList(m_szProjectPath);
}




#if 0


void DrawPageWin::init()
{
    if (!m_strProjectName.isEmpty()) {
        szProjPath_ = ProjectMgrUtils::getProjectPath(m_strProjectName);
        szProjName_ = ProjectMgrUtils::getProjectNameWithOutSuffix(m_strProjectName);
        open();
        ListViewUpdate();
        connect(&fileSystemWatcher_, SIGNAL(fileChanged(QString)), this, SLOT(onFileChanged(QString)));
        fileSystemWatcher_.addPath(szProjPath_ + "/DrawList.xml");
    }
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

void DrawPageWin::on_listViewDrawPage_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index)
    QModelIndex idx = ui->listViewDrawPage->selectionModel()->currentIndex();
    QStandardItem *item = this->pListDrawPageModel->itemFromIndex(idx);

    if (item == Q_NULLPTR)
        return;

    if (item->text() == tr("新建画面")) {
        NewDrawPage();
        return;
    }
    QMessageBox::information(this, tr("提示"), tr("code DrawMainWindow need add!!!"));

#if 0
    if(pDrawMainWindow_ != Q_NULLPTR) {
        delete pDrawMainWindow_;
        pDrawMainWindow_ = Q_NULLPTR;
    }

    if(pDrawMainWindow_ == Q_NULLPTR) {
        pDrawMainWindow_ = new DrawMainWindow(szProjPath_, szProjName_, item->text(), this);
        pDrawMainWindow_->openGraphPage(szProjPath_, szProjName_, item->text());
        pDrawMainWindow_->show();
    }
#endif
}


#endif
