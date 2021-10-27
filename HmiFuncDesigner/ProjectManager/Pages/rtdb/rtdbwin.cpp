#include "rtdbwin.h"
#include <QFile>
#include <QIcon>
#include <QModelIndex>
#include <QVBoxLayout>
#include <QApplication>
#include "confighelper.h"
#include "qsoftcore.h"
#include "qprojectcore.h"
#include "../../Public/userevent.h"


RTDBWin::RTDBWin(QWidget *parent) : QWidget(parent)
{
    ListViewInitUi();
}

RTDBWin::~RTDBWin()
{
    if(m_pListViewObj != NULL) {
        delete m_pListViewObj;
        m_pListViewObj = NULL;
    }
    if(m_pListViewModelObj != NULL) {
        delete m_pListViewModelObj;
        m_pListViewModelObj = NULL;
    }
}

void RTDBWin::ListViewInitUi()
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
            &RTDBWin::onSlotListViewProjectDoubleClicked);
}


void RTDBWin::onSlotListViewProjectDoubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index)

    QModelIndex idx = m_pListViewObj->selectionModel()->currentIndex();
    QStandardItem *item = m_pListViewModelObj->itemFromIndex(idx);

    if (item == NULL) {
        return;
    }

    QString program = "";

#ifdef Q_OS_WIN
    program = QApplication::applicationDirPath() + "/RtdbView.exe";
#endif

#ifdef Q_OS_LINUX
    program = QApplication::applicationDirPath() + "/RtdbView";
#endif

    QFile file(program);
    if (file.exists()) {
        QProcess *rtdbViewProc = new QProcess();
        rtdbViewProc->setWorkingDirectory(QApplication::applicationDirPath());
        QStringList arguments;
        arguments << QSoftCore::getCore()->getProjectCore()->m_szProjPath;
        rtdbViewProc->start(program, arguments);
        if (rtdbViewProc->waitForStarted()) {
        }
    }
}

bool RTDBWin::event(QEvent *ev)
{
    if(ev->type() == UserEvent::EVT_USER_SHOW_UPDATE) {
        UserEvent *pEvObj = dynamic_cast<UserEvent *>(ev);
        if(pEvObj) {

        }
        return true;
    } else if(ev->type() == UserEvent::EVT_USER_HIDE_UPDATE) {
        return true;
    }
    return QWidget::event(ev);
}





