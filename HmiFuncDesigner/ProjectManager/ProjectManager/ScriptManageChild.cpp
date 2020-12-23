#include "ScriptManageChild.h"
#include <QApplication>
#include <QCoreApplication>
#include <QListWidgetItem>
#include <QMenu>
#include "qsoftcore.h"
#include "qprojectcore.h"
#include "ConfigUtils.h"
#include "ScriptConditionConfigForm.h"
#include "ScriptEditorDlg.h"


ScriptManageChild::ScriptManageChild(QWidget *parent) : QWidget(parent)
{
    m_pListWidgetObj = new QListWidget(this);
    m_pListWidgetObj->setViewMode(QListView::IconMode);
    m_pListWidgetObj->setIconSize(QSize(32, 32));
    m_pListWidgetObj->setGridSize(QSize(100, 100));
    m_pListWidgetObj->setWordWrap(true);
    m_pListWidgetObj->setSpacing(20);
    m_pListWidgetObj->setResizeMode(QListView::Adjust);
    m_pListWidgetObj->setMovement(QListView::Static);
    connect(m_pListWidgetObj, SIGNAL(itemDoubleClicked(QListWidgetItem *)),
            this, SLOT(ListWidgetClicked(QListWidgetItem *)));
    m_pVLayoutObj = new QVBoxLayout();
    m_pVLayoutObj->addWidget(m_pListWidgetObj);
    m_pVLayoutObj->setContentsMargins(1, 1, 1, 1);
    this->setLayout(m_pVLayoutObj);
    setContextMenuPolicy(Qt::DefaultContextMenu);
}

ScriptManageChild::~ScriptManageChild()
{
    if(m_pListWidgetObj != Q_NULLPTR) {
        delete m_pListWidgetObj;
        m_pListWidgetObj = Q_NULLPTR;
    }
    if(m_pVLayoutObj != Q_NULLPTR) {
        delete m_pVLayoutObj;
        m_pVLayoutObj = Q_NULLPTR;
    }
}


/*
* 插槽：列表视图控件单击
*/
void ScriptManageChild::ListWidgetClicked(QListWidgetItem *item)
{
    if (m_szProjectName == "") return;

    if (item->text() == "新建脚本") {
        NewScript();
    } else {
        ModifyScript();
    }
}

/*
* 右键菜单生成
*/
void ScriptManageChild::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);

    QMenu *pMenu = new QMenu(this);

    QAction *pNewAct = new QAction(QIcon(":/images/icon_new.png"), tr("新建"), this);
    pNewAct->setStatusTip(tr("新建脚本"));
    connect(pNewAct, SIGNAL(triggered()), this, SLOT(NewScript()));
    pMenu->addAction(pNewAct);

    QAction *pModifyAct = new QAction(QIcon(":/images/icon_modify.png"), tr("修改"), this);
    pModifyAct->setStatusTip(tr("修改脚本"));
    connect(pModifyAct, SIGNAL(triggered()), this, SLOT(ModifyScript()));
    pMenu->addAction(pModifyAct);

    QAction *pDeleteAct = new QAction(QIcon(":/images/icon_delete.png"), tr("删除"), this);
    pDeleteAct->setStatusTip(tr("删除脚本"));
    connect(pDeleteAct, SIGNAL(triggered()), this, SLOT(DeleteScript()));
    pMenu->addAction(pDeleteAct);

    if(!pMenu->isEmpty()) {
        pMenu->move(cursor().pos());
        pMenu->exec();
        pMenu->clear();
    }
    delete pMenu;
}

/*
* 插槽：新建
*/
void ScriptManageChild::NewScript()
{
    if (m_szProjectName == "") return;

    QString strProjectPath = QSoftCore::getCore()->getProjectCore()->getProjectPath(m_szProjectName);
    QListWidgetItem *pCurItem = m_pListWidgetObj->currentItem();

    /////////////////////////////////////////////////////////////////////////////

    ScriptConditionConfigForm *pDlg = new ScriptConditionConfigForm(this);
    pDlg->setWindowTitle(tr("脚本属性"));
    pDlg->SetName(pCurItem->text());
    if (pDlg->exec() == QDialog::Accepted) {
        ScriptObject *pObj = new ScriptObject();
        pObj->m_szName = pDlg->GetName();
        pObj->m_bInUse = pDlg->isInUse();
        pObj->m_szDescription = pDlg->GetDescription();
        pObj->m_szRunMode = pDlg->GetRunMode();
        pObj->m_szRunModeArgs = pDlg->GetRunModeArgs();
        QSoftCore::getCore()->getProjectCore()->script_.AddScriptObject(pObj);
        updateUI();
    }
    delete pDlg;
}

/*
* 插槽：修改
*/
void ScriptManageChild::ModifyScript()
{
    if (m_szProjectName == "") return;

    QListWidgetItem *pCurItem = m_pListWidgetObj->currentItem();
    ScriptConditionConfigForm *pDlg = new ScriptConditionConfigForm(this);
    pDlg->setWindowTitle(tr("脚本属性"));
    ScriptObject *pObj = QSoftCore::getCore()->getProjectCore()->script_.GetScriptObject(pCurItem->text());
    pDlg->SetName(pObj->m_szName);
    pDlg->SetInUse(pObj->m_bInUse);
    pDlg->SetDescription(pObj->m_szDescription);
    pDlg->SetRunMode(pObj->m_szRunMode);
    pDlg->SetRunModeArgs(pObj->m_szRunModeArgs);
    if (pDlg->exec() == QDialog::Accepted) {
        pObj->m_szName = pDlg->GetName();
        pObj->m_bInUse = pDlg->isInUse();
        pObj->m_szDescription = pDlg->GetDescription();
        pObj->m_szRunMode = pDlg->GetRunMode();
        pObj->m_szRunModeArgs = pDlg->GetRunModeArgs();
        pCurItem->setText(pObj->m_szName);
        updateUI();

        /////////////////////////////////////////////////////////////////////////////

        ScriptEditorDlg *pScriptEditorDlg = new ScriptEditorDlg(this);
        pScriptEditorDlg->setScriptText(pObj->m_szScriptText);
        if (pScriptEditorDlg->exec() == QDialog::Accepted) {
            pObj->m_szScriptText = pScriptEditorDlg->getScriptText();
        }
        delete pScriptEditorDlg;
    }
    delete pDlg;
}

/*
* 插槽：删除
*/
void ScriptManageChild::DeleteScript()
{
    QListWidgetItem *pCurItem = m_pListWidgetObj->currentItem();
    ScriptObject *pObj = QSoftCore::getCore()->getProjectCore()->script_.GetScriptObject(pCurItem->text());
    QSoftCore::getCore()->getProjectCore()->script_.DeleteScriptObject(pObj);
    m_pListWidgetObj->removeItemWidget(pCurItem);

    updateUI();
}


void ScriptManageChild::updateUI()
{
    m_pListWidgetObj->clear();
    QListWidgetItem *pNewItemObj = new QListWidgetItem(QIcon(":/images/pm_script.png"), tr("新建脚本"));
    m_pListWidgetObj->addItem(pNewItemObj);
    for (int i = 0; i < QSoftCore::getCore()->getProjectCore()->script_.m_listScriptObjects.count(); i++) {
        ScriptObject *pObj = QSoftCore::getCore()->getProjectCore()->script_.m_listScriptObjects.at(i);
        QListWidgetItem *pItemObj = new QListWidgetItem(QIcon(":/images/pm_script.png"), pObj->m_szName);
        m_pListWidgetObj->addItem(pItemObj);
    }
}


void ScriptManageChild::buildUserInterface(QMainWindow* pMainWin)
{
    Q_UNUSED(pMainWin)
    updateUI();
}

void ScriptManageChild::removeUserInterface(QMainWindow* pMainWin)
{
    Q_UNUSED(pMainWin)
}

QString ScriptManageChild::wndTitle() const
{
    return this->windowTitle();
}




