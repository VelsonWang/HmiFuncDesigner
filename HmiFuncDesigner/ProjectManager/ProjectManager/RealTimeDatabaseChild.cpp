#include "RealTimeDatabaseChild.h"
#include <QAction>
#include <QDebug>
#include <QFile>
#include <QIcon>
#include <QKeySequence>
#include <QMenu>
#include <QModelIndex>
#include "ConfigUtils.h"
#include "Helper.h"
#include "qsoftcore.h"
#include "qprojectcore.h"

RealTimeDatabaseChild::RealTimeDatabaseChild(QWidget *parent) : QWidget(parent)
{

}

RealTimeDatabaseChild::~RealTimeDatabaseChild()
{
    if(m_pListViewObj != Q_NULLPTR) {
        delete m_pListViewObj;
        m_pListViewObj = Q_NULLPTR;
    }
    if(m_pListViewModelObj != Q_NULLPTR) {
        delete m_pListViewModelObj;
        m_pListViewModelObj = Q_NULLPTR;
    }
}

void RealTimeDatabaseChild::ListViewInitUi()
{
    QVBoxLayout *pVLayoutObj = new QVBoxLayout(this);
    pVLayoutObj->setSpacing(1);
    pVLayoutObj->setObjectName(QString::fromUtf8("verticalLayout"));
    pVLayoutObj->setContentsMargins(1, 1, 1, 1);
    m_pListViewObj = new ListViewEx(this);
    m_pListViewObj->setObjectName(QString::fromUtf8("listView"));
    m_pListViewObj->setViewMode(QListView::IconMode);
    m_pListViewObj->setIconSize(QSize(32, 32));
    m_pListViewObj->setGridSize(QSize(120, 120));
    m_pListViewObj->setWordWrap(true);
    m_pListViewObj->setSpacing(20);
    // 设置QListView大小改变时，图标的调整模式，默认是固定的，但可以改成自动调整：
    m_pListViewObj->setResizeMode(QListView::Adjust);
    //设置图标可不可以移动，默认是可移动的，但可以改成静态的：
    m_pListViewObj->setMovement(QListView::Static);

    /////////////////////////////////////////////////

    m_pListViewModelObj = new QStandardItemModel();
    QStandardItem *pRtdb = new QStandardItem(QIcon(":/images/db_rtdbview.png"), tr("实时数据库"));
    pRtdb->setEditable(false);
    m_pListViewModelObj->appendRow(pRtdb);
    m_pListViewObj->setModel(m_pListViewModelObj);

    pVLayoutObj->addWidget(m_pListViewObj);
    connect(m_pListViewObj, &QAbstractItemView::doubleClicked, this,
            &RealTimeDatabaseChild::onSlotListViewProjectDoubleClicked);
}


void RealTimeDatabaseChild::onSlotListViewProjectDoubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index)

    QModelIndex idx = m_pListViewObj->selectionModel()->currentIndex();
    QStandardItem *item = m_pListViewModelObj->itemFromIndex(idx);

    if (item == Q_NULLPTR) return;

    QString program = "";

#ifdef Q_OS_WIN
    program = Helper::AppDir() + "/RtdbView.exe";
#endif

#ifdef Q_OS_LINUX
    program = Helper::AppDir() + "/RtdbView";
#endif

    QFile file(program);
    if (file.exists()) {
        QProcess *rtdbViewProc = new QProcess();
        rtdbViewProc->setWorkingDirectory(Helper::AppDir());
        QStringList arguments;
        arguments << m_szProjPath;
        rtdbViewProc->start(program, arguments);
        if (rtdbViewProc->waitForStarted()) {
        }
    }
}

void RealTimeDatabaseChild::buildUserInterface(QMainWindow* pMainWin)
{
    if(pMainWin == Q_NULLPTR) return;

    if(m_pMainWinObj == Q_NULLPTR) {
        m_szProjPath = QSoftCore::getCore()->getProjectCore()->getProjectPath(m_szProjectName);
        ListViewInitUi();
    }
}

void RealTimeDatabaseChild::removeUserInterface(QMainWindow* pMainWin)
{
    if(pMainWin == Q_NULLPTR) return;

    if(m_pMainWinObj) {

    }
}

QString RealTimeDatabaseChild::wndTitle() const
{
    return this->windowTitle();
}




