#include "RealTimeDatabaseWin.h"
#include <QAction>
#include <QDebug>
#include <QFile>
#include <QIcon>
#include <QKeySequence>
#include <QMenu>
#include <QModelIndex>
#include "ConfigUtils.h"
#include "Helper.h"
#include "ProjectMgrUtils.h"
#include "ui_RealTimeDatabaseWin.h"

RealTimeDatabaseWin::RealTimeDatabaseWin(QWidget *parent,
                                         const QString &itemName,
                                         const QString &projName)
    : ChildBase(parent, itemName, projName), ui(new Ui::RealTimeDatabaseWin) {
  ui->setupUi(this);
  this->setWindowTitle(itemName);
  m_strItemName = itemName;
  m_ProjPath = ProjectMgrUtils::getProjectPath(projName);
  ListViewInitUi();
}

RealTimeDatabaseWin::~RealTimeDatabaseWin() {
  delete ui;
  if (pListViewModel != nullptr) {
    delete pListViewModel;
    pListViewModel = nullptr;
  }
}

void RealTimeDatabaseWin::init() {
  m_ProjPath = ProjectMgrUtils::getProjectPath(m_strProjectName);
}

void RealTimeDatabaseWin::ListViewInitUi() {
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
  QStandardItem *pRtdb =
      new QStandardItem(QIcon(":/images/db_rtdbview.png"), tr("实时数据库"));
  pRtdb->setEditable(false);
  pListViewModel->appendRow(pRtdb);

  ui->listView->setModel(pListViewModel);
}

/*
 * 打开文件
 */
void RealTimeDatabaseWin::open() {}

/*
 * 保存文件
 */
void RealTimeDatabaseWin::save() {}

/*
* 显示大图标
*/
void RealTimeDatabaseWin::showLargeIcon() {
  ui->listView->setIconSize(QSize(32, 32));
}

/*
* 显示小图标
*/
void RealTimeDatabaseWin::showSmallIcon() {
  ui->listView->setIconSize(QSize(24, 24));
}

void RealTimeDatabaseWin::on_listView_clicked(const QModelIndex &index) {
  Q_UNUSED(index)
}

void RealTimeDatabaseWin::on_listView_doubleClicked(const QModelIndex &index) {
  Q_UNUSED(index)

  QModelIndex idx = ui->listView->selectionModel()->currentIndex();
  QStandardItem *item = pListViewModel->itemFromIndex(idx);

  if (item == NULL) return;

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
    arguments << m_ProjPath;
    rtdbViewProc->start(program, arguments);
    if (rtdbViewProc->waitForStarted()) {
    }
  }
}
