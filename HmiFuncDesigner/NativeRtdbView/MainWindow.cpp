#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "../Public/Public.h"
#include "ProjectData.h"
#include <QApplication>
#include <QSettings>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QScopedPointer>
#include <QMessageBox>
#include <QEventLoop>
#include <QMutexLocker>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "GetRtdbWriteValueDialog.h"
#include "RealTimeDB.h"
#include <QHeaderView>
#include <QMenu>
#include <QAction>
#include <QIcon>
#include <QModelIndex>
#include <QTime>
#include <QDebug>

#define PERIOD    (500)



MainWindow::MainWindow(QString projPath, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mProjectPath(projPath),
    timer_(Q_NULLPTR)
{
    ui->setupUi(this);
    ReadSettings();
    Load();

    RealTimeDB::instance();

    setWindowIcon(QIcon(":/images/pm_rtdb.png"));
    setContextMenuPolicy(Qt::DefaultContextMenu);

    pTagTableModel = Q_NULLPTR;
    pTableView = new QTableView(this);
    QHeaderView *pVHeaderView = pTableView->verticalHeader();
    pVHeaderView->setSectionResizeMode(QHeaderView::ResizeToContents);
    QHeaderView *pHHeaderView = pTableView->horizontalHeader();
    pHHeaderView->setSectionResizeMode(QHeaderView::ResizeToContents);
    pTableView->resizeRowsToContents();
    pTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    pTableView->setWordWrap(false);
    pTableView->horizontalHeader()->setStretchLastSection(true);
    pTableView->horizontalHeader()->setHighlightSections(false);

    InitWindow();

    timer_ = new QTimer;
    connect(timer_, SIGNAL(timeout()), this, SLOT(timeout()));
}

MainWindow::~MainWindow()
{
    if(pTagTableModel != Q_NULLPTR) {
        pTagTableModel->ClearItems();
        delete pTagTableModel;
        pTagTableModel = Q_NULLPTR;
    }

    if(pTableView != Q_NULLPTR) {
        delete pTableView;
        pTableView = Q_NULLPTR;
    }

    delete ui;
    QMap<QString, QList<TagItem *>>::const_iterator iter = mMapIoTagList.begin();
    while (iter != mMapIoTagList.end()) {
        QString sItemName = iter.key();
        QList<TagItem *> itemList = iter.value();
        qDeleteAll(itemList);
        itemList.clear();
        ++iter;
    }
    qDeleteAll(mTmpTagList);
    mTmpTagList.clear();
    qDeleteAll(mSysTagList);
    mSysTagList.clear();

    if(timer_ != Q_NULLPTR) {
        if(timer_->isActive()) timer_->stop();
        delete timer_;
        timer_ = Q_NULLPTR;
    }
}


/*
* 右键菜单
*/
void MainWindow::contextMenuEvent(QContextMenuEvent * /*event*/)
{
    if(this->windowTitle().contains("系统变量"))
        return;

    QMenu *pMenu = new QMenu(this);

    QAction *pWriteTagAct = new QAction(tr("写实时数据库"), this);
    pWriteTagAct->setStatusTip(tr("写实时数据库"));
    connect(pWriteTagAct, SIGNAL(triggered()), this, SLOT(onWriteRtdbTag()));
    pMenu->addAction(pWriteTagAct);

    pMenu->move(cursor().pos());
    pMenu->show();
}


QStandardItem *MainWindow::CreateTreeItem(const QString name)
{
    QStandardItem *pItem = new QStandardItem(name);
    pItem->setEditable(false);
    return pItem;
}


void MainWindow::InitTreeViewUi()
{
    ui->TagTreeView->setHeaderHidden(true);
    pTagTreeViewModel = new QStandardItemModel();

    pTreeItemRoot = new QStandardItem(tr("实时数据库"));
    pTreeItemRoot->setEditable(false);
    pTreeIoItem = CreateTreeItem(tr("IO变量"));
    pTreeItemRoot->appendRow(pTreeIoItem);

    QMap<QString, QList<TagItem *>>::const_iterator iter = mMapIoTagList.begin();
    while (iter != mMapIoTagList.end()) {
        QString sItemName = iter.key();
        QList<TagItem *> itemList = iter.value();
        if(itemList.count() > 0) {
            int tableCnt = itemList.count()/64;
            if(itemList.count()%64) tableCnt++;
            for(int i=0; i<tableCnt; i++) {
                QString name = "IO设备-" + sItemName;
                if(i*64+63<itemList.count()) name += QString(" (%1-%2)").arg(i*64+1).arg(i*64+64);
                else name += QString(" (%1-%2)").arg(i*64+1).arg(i*64+itemList.count()%64);
                pTreeIoItem->appendRow(CreateTreeItem(name));
                mMapIoTagPageId[name] = mMapIoTagPageGroupId[sItemName];
                mMapIoTagPageGroupId.clear();
            }
        }
        ++iter;
    }

    pTreeTmpItem = CreateTreeItem(tr("中间变量"));
    pTreeItemRoot->appendRow(pTreeTmpItem);

    if(mTmpTagList.count() > 0) {
        int tableTmpCnt = mTmpTagList.count()/64;
        if(mTmpTagList.count()%64) tableTmpCnt++;
        for(int i=0; i<tableTmpCnt; i++) {
            QString name = "中间变量";
            if(i*64+63<mTmpTagList.count()) name += QString(" (%1-%2)").arg(i*64+1).arg(i*64+64);
            else name += QString(" (%1-%2)").arg(i*64+1).arg(i*64+mTmpTagList.count()%64);
            pTreeTmpItem->appendRow(CreateTreeItem(name));
        }
    }

    pTreeSysItem = CreateTreeItem(tr("系统变量"));
    pTreeItemRoot->appendRow(pTreeSysItem);

    if(mSysTagList.count() > 0) {
        int tableSysCnt = mSysTagList.count()/64;
        if(mSysTagList.count()%64) tableSysCnt++;
        for(int i=0; i<tableSysCnt; i++) {
            QString name = "系统变量";
            if(i*64+63<mSysTagList.count()) name += QString(" (%1-%2)").arg(i*64+1).arg(i*64+64);
            else name += QString(" (%1-%2)").arg(i*64+1).arg(i*64+mSysTagList.count()%64);
            pTreeSysItem->appendRow(CreateTreeItem(name));
        }
    }

    pTagTreeViewModel->appendRow(pTreeItemRoot);
    ui->TagTreeView->setModel(pTagTreeViewModel);
    ui->TagTreeView->expandAll();
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    WriteSettings();
    event->accept();
}

void MainWindow::WriteSettings()
{
    QSettings settings("Organization", "RtdbView");
    settings.setValue("pos", pos());
    settings.setValue("size", size());
}

void MainWindow::ReadSettings()
{
    QSettings settings("Organization", "RtdbView");
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(800, 600)).toSize();
    move(pos);
    resize(size);
}

void MainWindow::InitWindow()
{
    setCentralWidget(pTableView);
    setWindowTitle(tr("实时数据库"));
    InitTreeViewUi();
}

/**
 * @brief MainWindow::getProjectName
 * @details 获取工程名称
 * @param szProjectPath
 * @return
 */
QString MainWindow::getProjectName(const QString &szProjectPath)
{
    QFileInfo srcFileInfo(szProjectPath);
    QString szProjName = "";
    if (srcFileInfo.isDir()) {
        QDir sourceDir(szProjectPath);
        QDir::Filters filters = QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System;
        QStringList fileNames = sourceDir.entryList(filters);
        foreach (const QString &fileName, fileNames) {
            if(fileName.endsWith("pdt")) {
                QFileInfo info(fileName);
                szProjName = info.baseName();
            }
        }
    }

    return szProjName;
}

bool MainWindow::Load()
{
    if(mProjectPath == "")
        return false;

    QDir projDir(mProjectPath);
    if(!projDir.exists())
        return false;

    QString szProjName = getProjectName(mProjectPath);

    if(szProjName == "") {
        QMessageBox::information(this, tr("提示"), tr("未找到工程文件！"));
        return false;
    } else {
        ProjectData::getInstance()->createOrOpenProjectData(mProjectPath, szProjName);

        //-----------------系统变量------------------//
        TagSys &tagSys = ProjectData::getInstance()->tagSys_;
        tagSys.load(ProjectData::getInstance()->dbData_);

        foreach (TagSysDBItem * itemTagSys, tagSys.listTagSysDBItem_) {
            TagItem *pItem = new TagItem();
            pItem->mId = itemTagSys->m_szTagID;
            pItem->mName = itemTagSys->m_szName;
            pItem->mDescription = itemTagSys->m_szDescription;
            mSysTagList.append(pItem);
        }

        qDeleteAll(tagSys.listTagSysDBItem_);
        tagSys.listTagSysDBItem_.clear();

        //-----------------中间变量------------------//
        TagTmp &tagTmp = ProjectData::getInstance()->tagTmp_;
        tagTmp.load(ProjectData::getInstance()->dbData_);

        foreach (TagTmpDBItem * itemTagTmp, tagTmp.listTagTmpDBItem_) {
            TagItem *pItem = new TagItem();
            pItem->mId = itemTagTmp->m_szTagID;
            pItem->mName = itemTagTmp->m_szName;
            pItem->mDescription = itemTagTmp->m_szDescription;
            mTmpTagList.append(pItem);;
        }

        qDeleteAll(tagTmp.listTagTmpDBItem_);
        tagTmp.listTagTmpDBItem_.clear();

        //-----------------设备变量------------------//

        TagIO &tagIO = ProjectData::getInstance()->tagIO_;
        TagIOGroup &tagIOGroup = ProjectData::getInstance()->tagIOGroup_;
        tagIOGroup.load(ProjectData::getInstance()->dbData_);
        tagIO.load(ProjectData::getInstance()->dbData_);

        foreach (TagIOGroupDBItem * itemTagIOGroup, tagIOGroup.listTagIOGroupDBItem_) {
            QString szGroup = itemTagIOGroup->m_szGroupName;
            foreach (TagIODBItem * itemTagIO, tagIO.listTagIODBItem_) {
                if(szGroup != itemTagIO->m_szGroupName)
                    continue;
                QString sTableName = itemTagIOGroup->m_szShowName;
                mMapIoTagPageGroupId[sTableName] = itemTagIOGroup->m_id;
                mIoTagTableNameList.append(sTableName);
                TagItem *pItem = new TagItem();
                pItem->mId = itemTagIO->m_szTagID;
                pItem->mName = itemTagIO->m_szName;
                pItem->mDescription = itemTagIO->m_szDescription;
                QString szIOInfo = itemTagIO->m_szDeviceAddr;
                szIOInfo = szIOInfo + ", " + itemTagIO->m_szRegisterArea;
                szIOInfo = szIOInfo + ", " + itemTagIO->m_szRegisterAddr;
                szIOInfo = szIOInfo + ", " + itemTagIO->m_szAddrOffset;
                szIOInfo = szIOInfo + ", " + itemTagIO->m_szReadWriteType;
                szIOInfo = szIOInfo + ", " + itemTagIO->m_szDataType;
                pItem->mDeviceInfo = szIOInfo;
                mMapIoTagList[sTableName].append(pItem);
            }
        }

        qDeleteAll(tagIO.listTagIODBItem_);
        tagIO.listTagIODBItem_.clear();
        qDeleteAll(tagIOGroup.listTagIOGroupDBItem_);
        tagIOGroup.listTagIOGroupDBItem_.clear();
    }

    return true;
}

/**
 * @brief MainWindow::on_TagTreeView_clicked 根据单击树选项显示子窗口
 * @param index
 */
void MainWindow::on_TagTreeView_clicked(const QModelIndex &index)
{
    QStandardItem *item = pTagTreeViewModel->itemFromIndex(index);
    QString winTittle = item->text();
    if(!winTittle.endsWith(")")) return;

    QList<TagItem *> tagList = QList<TagItem *>();
    if(winTittle.indexOf("IO设备") > -1) {
        QMap<QString, QList<TagItem *>>::const_iterator iter = mMapIoTagList.begin();
        while (iter != mMapIoTagList.end()) {
            if(winTittle.indexOf(iter.key()) > -1) tagList = iter.value();
            ++iter;
        }
    }
    else if(winTittle.indexOf("中间变量") > -1) tagList = mTmpTagList;
    else if(winTittle.indexOf("系统变量") > -1) tagList = mSysTagList;
    else ;

    mCurTagList.clear();
    mCurTagList = tagList;
    setTableView(tagList, winTittle);

    if(timer_ != Q_NULLPTR && !timer_->isActive())
        timer_->start(PERIOD);
}


void MainWindow::ShowFirstPage()
{
    QMap<QString, QList<TagItem *>>::const_iterator iter = mMapIoTagList.begin();
    while (iter != mMapIoTagList.end()) {
        QString sItemName = iter.key();
        QList<TagItem *> itemList = iter.value();
        if(itemList.count() > 0) {
            int tableCnt = itemList.count()/64;
            if(itemList.count()%64) tableCnt++;
            for(int i=0; i<tableCnt; i++) {
                QString name = "IO设备-" + sItemName;
                if(i*64+63<itemList.count()) name += QString(" (%1-%2)").arg(i*64+1).arg(i*64+64);
                else name += QString(" (%1-%2)").arg(i*64+1).arg(i*64+itemList.count()%64);
                if(i==0) {
                    QMap<QString, QList<TagItem *>>::const_iterator it = mMapIoTagList.begin();
                    while (it != mMapIoTagList.end()) {
                        if(name.indexOf(it.key()) > -1) {
                            QList<TagItem *> tagList = it.value();
                            mCurTagList.clear();
                            mCurTagList = tagList;
                            setTableView(tagList, name);
                        }
                        ++it;
                    }
                }
            }
        }
        ++iter;
    }
    if(timer_ != Q_NULLPTR && !timer_->isActive())
        timer_->start(PERIOD);
}


void MainWindow::timeout()
{
    if(RealTimeDB::instance()->m_memStatus) {
        foreach(TagItem *pTagObj, mCurTagList) {
            QString szVal = RealTimeDB::instance()->GetDataString(pTagObj->mId);
            SetTagLogicValueAndStatus(pTagObj->mId, szVal, "");
        }
    } else {
        QMessageBox::information(this, QObject::tr("提示"), QObject::tr("打开共享内存失败！"));
        if(timer_ != Q_NULLPTR) {
            if(timer_->isActive()) timer_->stop();
        }
    }
}


/*
* 插槽：写实时数据库
*/
void MainWindow::onWriteRtdbTag()
{
    QString cmdline = "";
    QModelIndex ModelIndex = pTableView->selectionModel()->currentIndex();
    int row = ModelIndex.row();
    TagItem *pTagItem = pTagTableModel->GetRow(row);

    QScopedPointer<GetRtdbWriteValueDialog> dlg(new GetRtdbWriteValueDialog(pTagItem, this));
    if(dlg->exec() == QDialog::Accepted) {
        cmdline = QString("%1=%2").arg(pTagItem->mId).arg(dlg->getWriteValue());
    }
    else {
        return;
    }

    QStringList cmdLineList = cmdline.split('=');
    if(cmdLineList.count() != 2) return;
    QString idString = cmdLineList.at(0);
    QString valueString = cmdLineList.at(1);

    qDebug() << __FILE__ << __FUNCTION__ << __LINE__ << "id: " << idString << "value: " << valueString;
    RealTimeDB::instance()->SetDataString(idString, valueString);
}


void MainWindow::SetTagLogicValueAndStatus(QString id, QString logicValue, QString status)
{
    pTagTableModel->UpdateLogicValueAndStatus(id, logicValue, status);
}

void MainWindow::setTableView(QList<TagItem *> &tagList, const QString &szTittle)
{
    setWindowTitle(szTittle);

    int start = 0;
    int end = 0;
    QString tmp = szTittle.mid(szTittle.indexOf("(")+1, szTittle.indexOf(")") - szTittle.indexOf("(") - 1);
    start = tmp.left(tmp.indexOf("-")).toInt()-1;
    end = tmp.right(tmp.length() - tmp.indexOf("-") - 1).toInt()-1;

    if(pTagTableModel != Q_NULLPTR) {
        delete pTagTableModel;
        pTagTableModel = Q_NULLPTR;
    }
    pTagTableModel = new TagTableModel();
    pTagTableModel->load(tagList, start, end);
    pTableView->setModel(pTagTableModel);
}
