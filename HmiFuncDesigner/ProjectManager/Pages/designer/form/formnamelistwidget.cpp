#include "formnamelistwidget.h"
#include "qsoftcore.h"
#include <QMenu>
#include <QAction>
#include <QInputDialog>

#include "../../../libs/core/qsoftcore.h"
#include "../../../libs/shared/qprojectcore.h"
#include "../../../libs/shared/qpagemanager.h"
#include "../../../libs/shared/property/qabstractproperty.h"
#include "../../../libs/shared/host/qabstracthost.h"

FormNameListWidget::FormNameListWidget(QWidget *parent)
    : QListWidget(parent) {
    setIconSize(QSize(32, 32));
    setContextMenuPolicy(Qt::DefaultContextMenu);
}


/**
 * @brief FormNameListWidget::contextMenuEvent
 * @details 右键菜单
 */
void FormNameListWidget::contextMenuEvent(QContextMenuEvent * event) {
    QMenu *pMenu = new QMenu(this);

    QAction *pNewAct = new QAction(tr("新建"), this);
    pNewAct->setStatusTip(tr("新建画面"));
    connect(pNewAct, SIGNAL(triggered()), this, SLOT(onSlotNewFormPage()));

    QAction *pRenameAct = new QAction(tr("重命名"), this);
    pRenameAct->setStatusTip(tr("重命名画面"));
    //connect(pRenameAct, SIGNAL(triggered()), this, SLOT(onRenameFormPage()));

    QAction *pDeleteAct = new QAction(tr("删除"), this);
    pDeleteAct->setStatusTip(tr("删除画面"));
    //connect(pDeleteAct, SIGNAL(triggered()), this, SLOT(onDeleteFormPage()));

    QAction *pCopyAct = new QAction(tr("复制"), this);
    pCopyAct->setStatusTip(tr("复制画面"));
    //connect(pCopyAct, SIGNAL(triggered()), this, SLOT(onCopyFormPage()));

    QAction *pPasteAct = new QAction(tr("粘贴"), this);
    pPasteAct->setStatusTip(tr("粘贴画面"));
    pPasteAct->setEnabled(false);
    //connect(pPasteAct, SIGNAL(triggered()), this, SLOT(onPasteFormPage()));

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
 * @brief FormNameListWidget::onSlotNewFormPage
 * @details 新建画面
 */
void FormNameListWidget::onSlotNewFormPage()
{
    QString szName = QString("form");
    QInputDialog dlg(this);
    dlg.setWindowTitle(tr("画面名称"));
    dlg.setLabelText(tr("请输入画面名称"));
    dlg.setOkButtonText(tr("确定"));
    dlg.setCancelButtonText(tr("取消"));
    dlg.setTextValue(szName);

reInput:
    dlg.setLabelText(tr("画面名称为空或画面名称重复"));
    if ( dlg.exec() == QDialog::Accepted ) {
        szName = dlg.textValue();
        QList<QListWidgetItem*> listWidgetItem = this->findItems(szName, Qt::MatchCaseSensitive);
        if (listWidgetItem.size() > 0 || szName == "") {
            goto reInput;
        }

        QPageManager *pPageMgrObj = QSoftCore::getCore()->getProjectCore()->getPageManager();
        pPageMgrObj->newPage(szName);

        QListWidgetItem *pItemObj = new QListWidgetItem(QIcon(":/images/GraphPage.png"), szName);
        this->addItem(pItemObj);
        emit notifyCreateFormPageUseName(szName);
    }
}

void FormNameListWidget::updateUI()
{
    while (this->count() > 0) {
        QListWidgetItem *pItemObj = this->takeItem(0);
        if(pItemObj) delete pItemObj;
    }
    this->clear();

    QPageManager *pPageMgrObj = QSoftCore::getCore()->getProjectCore()->getPageManager();

    foreach(QAbstractHost* pHostObj, pPageMgrObj->getPages()) {
        QAbstractProperty* pPropObj = pHostObj->getProperty("objectName");
        if(pPropObj != Q_NULLPTR) {
            QListWidgetItem *pItemObj = new QListWidgetItem(QIcon(":/images/GraphPage.png"), pPropObj->get_value().toString());
            this->addItem(pItemObj);
        }
    }
}
