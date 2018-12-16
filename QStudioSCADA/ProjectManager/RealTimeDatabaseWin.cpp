#include "RealTimeDatabaseWin.h"
#include "ui_RealTimeDatabaseWin.h"
#include "configutils.h"
#include <QMenu>
#include <QAction>
#include <QIcon>
#include <QKeySequence>
#include <QModelIndex>
#include <QFile>
#include <QProcess>

#include <QDebug>

RealTimeDatabaseWin::RealTimeDatabaseWin(QWidget *parent, QString itemName, QString projName) :
    ChildBase(parent, itemName, projName),
    ui(new Ui::RealTimeDatabaseWin)
{
    ui->setupUi(this);
    this->setWindowTitle(itemName);
    m_strItemName = itemName;
    m_ProjPath = projName.left(projName.lastIndexOf("/"));
    ListViewInitUi();
}

RealTimeDatabaseWin::~RealTimeDatabaseWin()
{
    delete ui;
    if(pListViewModel != NULL)
    {
        delete pListViewModel;
        pListViewModel = NULL;
    }
}


void RealTimeDatabaseWin::ListViewInitUi()
{
    ui->listView->setViewMode(QListView::IconMode);
    ui->listView->setIconSize(QSize(32, 32));
    ui->listView->setGridSize(QSize(120, 120));
    ui->listView->setWordWrap(true);
    ui->listView->setSpacing(20);
    // 设置QListView大小改变时，图标的调整模式，默认是固定的，但可以改成自动调整：
    ui->listView->setResizeMode(QListView::Adjust);
    //设置图标可不可以移动，默认是可移动的，但可以改成静态的：
    ui->listView->setMovement(QListView::Static);

    /////////////////////////////////////////////////

    pListViewModel = new QStandardItemModel();
    QStandardItem *pRtdb = new QStandardItem(QIcon(":/images/pm_rtdb.png"), tr("实时数据库"));
    pRtdb->setEditable(false);
    pListViewModel->appendRow(pRtdb);

    ui->listView->setModel(pListViewModel);
}


/*
 * 打开文件
 */
void RealTimeDatabaseWin::open()
{

}

/*
 * 保存文件
 */
void RealTimeDatabaseWin::save()
{

}

/*
* 显示大图标
*/
void RealTimeDatabaseWin::ShowLargeIcon()
{
    ui->listView->setIconSize(QSize(32, 32));
}

/*
* 显示小图标
*/
void RealTimeDatabaseWin::ShowSmallIcon()
{
    ui->listView->setIconSize(QSize(24, 24));
}

void RealTimeDatabaseWin::on_listView_clicked(const QModelIndex &index)
{

}


void RealTimeDatabaseWin::on_listView_doubleClicked(const QModelIndex &index)
{
    QModelIndex idx = ui->listView->selectionModel()->currentIndex();
    QStandardItem *item = pListViewModel->itemFromIndex(idx);

    if(item == NULL)
        return;

    QString program = "";
#ifdef Q_OS_WIN
    program = ConfigUtils::AppDir() + "/RtdbView.exe";
#elif Q_OS_LINUX
    program = ConfigUtils::AppDir() + "/RtdbView";
#else

#endif
    QFile file(program);
    if(file.exists())
    {
        QProcess *RtdbViewProc = new QProcess;
        // 设置进程工作目录
        RtdbViewProc->setWorkingDirectory(ConfigUtils::AppDir());
        QStringList arguments;
        arguments << m_ProjPath;
        RtdbViewProc->start(program, arguments);
        if(RtdbViewProc->waitForStarted())
        {
            /*
            //qDebug()<< "process start.";
            if(RtdbViewProc->waitForFinished())
            {
                //qDebug()<< "process finished.";
                // 压缩完成准备传输文件
            }
            if(RtdbViewProc->exitStatus() == QProcess::NormalExit)
            {
                //qDebug()<< "process exitStatus is QProcess::NormalExit.";
            }
            else // QProcess::CrashExit
            {
                //qDebug()<< "process exitStatus is QProcess::CrashExit.";
            }
            */
        }
        //delete RtdbViewProc;
    }
}
