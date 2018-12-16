#include "MdiChildWindow.h"
#include "GetRtdbWriteValueDialog.h"
#include <QHeaderView>
#include <QMenu>
#include <QAction>
#include <QIcon>
#include <QModelIndex>
#include <QDebug>


MdiChildWindow::MdiChildWindow(QList<TagItem *> tagList, QString tittle, QWidget *parent) :
    QWidget(parent),
    mTagList(tagList)
{
    setWindowTitle(tittle);
    setWindowIcon(QIcon(":/images/pm_rtdb.png"));
    setContextMenuPolicy(Qt::DefaultContextMenu); // 右键菜单生效

    pTagTableModel = new TagTableModel();

    int start = 0;
    int end = 0;
    QString tmp = tittle.mid(tittle.indexOf("(")+1, tittle.indexOf(")") - tittle.indexOf("(") - 1);
    start = tmp.left(tmp.indexOf("-")).toInt()-1;
    end = tmp.right(tmp.length() - tmp.indexOf("-") - 1).toInt()-1;

    pTagTableModel->load(mTagList, start, end);
    pTableView = new QTableView(this);
    QHeaderView *pVHeaderView = pTableView->verticalHeader();
    pVHeaderView->setSectionResizeMode(QHeaderView::ResizeToContents);
    QHeaderView *pHHeaderView = pTableView->horizontalHeader();
    pHHeaderView->setSectionResizeMode(QHeaderView::ResizeToContents);
    pTableView->resizeRowsToContents();
    pTableView->setModel(pTagTableModel);
    pTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    pTableView->setWordWrap(false);

    verticalLayout = new QVBoxLayout(this);
    verticalLayout->setSpacing(1);
    verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
    verticalLayout->setContentsMargins(1, 1, 1, 1);
    verticalLayout->addWidget(pTableView);



}


MdiChildWindow::~MdiChildWindow()
{
    pTagTableModel->ClearItems();
    delete pTagTableModel;
    delete pTableView;
}


void MdiChildWindow::SetTagLogicValueAndStatus(int id, QString logicValue, QString status)
{
    //qDebug()<< id<<logicValue<<status;
    pTagTableModel->UpdateLogicValueAndStatus(id, logicValue, status);
}



/*
* 右键菜单
*/
void MdiChildWindow::contextMenuEvent(QContextMenuEvent * /*event*/)
{
    if(this->windowTitle().contains("系统变量"))
        return;

    QMenu *pMenu = new QMenu(this);

    QAction *pWriteTagAct = new QAction(tr("写实时数据库"), this);
    pWriteTagAct->setStatusTip(tr("写实时数据库"));
    connect(pWriteTagAct, SIGNAL(triggered()), this, SLOT(writeRtdbTagAct()));
    pMenu->addAction(pWriteTagAct);

    pMenu->move(cursor().pos());
    pMenu->show();
}


/*
* 插槽:写实时数据库
*/
void MdiChildWindow::writeRtdbTagAct()
{
    QModelIndex ModelIndex = pTableView->selectionModel()->currentIndex();
    int row = ModelIndex.row();
    TagItem *pTagItem = pTagTableModel->GetRow(row);

    QScopedPointer<GetRtdbWriteValueDialog> dlg(new GetRtdbWriteValueDialog(pTagItem, this));
    if(dlg->exec() == QDialog::Accepted)
    {
        QString cmdline = QString("%1=%2").arg(pTagItem->mId).arg(dlg->GetWriteValue());
        emit writeRtdbTag(cmdline);
    }
}
