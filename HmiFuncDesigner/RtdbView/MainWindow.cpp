#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "MdiChildWindow.h"
#include "RtdbConnectDialog.h"
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
#include <QDebug>

#define PERIOD    (1000)


MainWindow::MainWindow(QString projPath, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mProjectPath(projPath),
    bConnectStatus_(false),
    ip_("127.0.0.1"),
    port_(60000),
    url_(""),
    timer_(Q_NULLPTR)
{
    ui->setupUi(this);
    ReadSettings();
    Load();
    InitWindow();
    m_networkAccessManager = new QNetworkAccessManager(this);
    connect(m_networkAccessManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(finished(QNetworkReply*)));
}

MainWindow::~MainWindow()
{
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


MdiChildWindow* MainWindow::ActiveMdiChild()
{
    if (QMdiSubWindow *activeSubWindow = ui->mdiArea->activeSubWindow())
        return qobject_cast<MdiChildWindow *>(activeSubWindow->widget());
    return Q_NULLPTR;
}


void MainWindow::SetActiveSubWindow(MdiChildWindow *window)
{
    if(!window)
        return;
    window->showMaximized();
    ui->mdiArea->setActiveSubWindow(0);//Activates the subwindow window. If window is 0, any current active window is deactivated.
    ui->mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow*>(window));
}


MdiChildWindow* MainWindow::GetActiveSubWindow()
{
    return qobject_cast<MdiChildWindow*>(ui->mdiArea->activeSubWindow()->widget());
}



MdiChildWindow* MainWindow::FindMdiChild(const QString &windowTitle)
{
    foreach(QMdiSubWindow* window, ui->mdiArea->subWindowList()) {
        MdiChildWindow * pChildWin = qobject_cast<MdiChildWindow *>(window->widget());
        if(pChildWin->windowTitle() == windowTitle)
            return pChildWin;
    }
    return Q_NULLPTR;
}


QMdiSubWindow* MainWindow::FindMdiSubWindow(const QString &windowTitle)
{
    foreach(QMdiSubWindow* window, ui->mdiArea->subWindowList()) {
        MdiChildWindow * pChildWin = qobject_cast<MdiChildWindow *>(window->widget());
        if(pChildWin->windowTitle() == windowTitle)
            return window;
    }
    return Q_NULLPTR;
}



void MainWindow::closeEvent(QCloseEvent *event)
{
    ui->mdiArea->closeAllSubWindows();
    if (ui->mdiArea->currentSubWindow()) {
        event->ignore();
    } else {
        WriteSettings();
        event->accept();
    }
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
    setCentralWidget(ui->mdiArea);
    setWindowTitle(tr("实时数据库"));
    ui->mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->statusBar->showMessage(tr("未连接..."));
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
    QMutexLocker locker(&mutex_);
    QStandardItem *item = pTagTreeViewModel->itemFromIndex(index);
    QString winTittle = item->text();
    if(!winTittle.endsWith(")")) return;

    foreach (QMdiSubWindow* window, ui->mdiArea->subWindowList()) {
        MdiChildWindow *pChildWin = qobject_cast<MdiChildWindow *>(window->widget());
        if(pChildWin->windowTitle() == winTittle) {
            SetActiveSubWindow(pChildWin);
            return;
        } else {
            if(bConnectStatus_ && timer_ != Q_NULLPTR)
                if(timer_->isActive()) timer_->stop();
            window->close();
        }
    }

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

    MdiChildWindow* pMdiChildWindow = new MdiChildWindow(tagList, winTittle, this);
    connect(pMdiChildWindow,SIGNAL(writeRtdbTag(QString)),this,SLOT(writeRtdbTag(QString)));
    ui->mdiArea->addSubWindow(pMdiChildWindow);
    SetActiveSubWindow(pMdiChildWindow);

    if(bConnectStatus_ && timer_ != Q_NULLPTR && !timer_->isActive())
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
                            MdiChildWindow* pMdiChildWindow = new MdiChildWindow(it.value(), name, this);
                            connect(pMdiChildWindow,SIGNAL(writeRtdbTag(QString)),this,SLOT(writeRtdbTag(QString)));
                            ui->mdiArea->addSubWindow(pMdiChildWindow);
                            SetActiveSubWindow(pMdiChildWindow);
                        }
                        ++it;
                    }
                }
            }
        }
        ++iter;
    }
}


/**
 * @brief MainWindow::on_actionConnect_triggered 连接实时数据库
 */
void MainWindow::on_actionConnect_triggered()
{
    QScopedPointer<RtdbConnectDialog> dlg(new RtdbConnectDialog(this));

    if(bConnectStatus_) dlg->SetConnectStatus(tr("连接状态：已连接"));
    else dlg->SetConnectStatus(tr("连接状态：连接关闭"));

    if(dlg->exec() == QDialog::Accepted) {
        if(bConnectStatus_)
            return;

        port_ = 60000;
        ip_ = dlg->GetIPAddress();
        qDebug() << ip_ << port_;
        bConnectStatus_ = true;
        ui->statusBar->showMessage(tr("连接成功！"));

        timer_ = new QTimer();
        timer_->start(PERIOD);
        connect(timer_, SIGNAL(timeout()), this, SLOT(timeout()));
    } else {
        if(dlg->GetOption() == 0) {
            bConnectStatus_ = false;
            ui->statusBar->showMessage(tr("未连接..."));
        }
    }
}

void MainWindow::on_actionClose_triggered()
{
    WriteSettings();
    qApp->exit();
}


void MainWindow::timeout()
{
    if(!bConnectStatus_) {
        if(timer_ != Q_NULLPTR) {
            if(timer_->isActive()) timer_->stop();
            delete timer_;
            timer_ = Q_NULLPTR;
        }
        return;
    }

    QList<QMdiSubWindow *> listSubWin = ui->mdiArea->subWindowList();
    if(listSubWin.size() < 1) return;
    MdiChildWindow* pMdiChildWindow =  qobject_cast<MdiChildWindow *>(listSubWin.at(0)->widget());
    QString windowTitle = "";
    if(pMdiChildWindow != Q_NULLPTR) windowTitle = pMdiChildWindow->windowTitle();
    if(windowTitle == "") return;

    QList<TagItem *> tagList = QList<TagItem *>();
    if(windowTitle.indexOf("IO设备") > -1) {
        QMap<QString, QList<TagItem *>>::const_iterator iter = mMapIoTagList.begin();
        while (iter != mMapIoTagList.end()) {
            if(windowTitle.indexOf(iter.key()) > -1) tagList = iter.value();
            ++iter;
        }
    }
    else if(windowTitle.indexOf("中间变量") > -1) tagList = mTmpTagList;
    else if(windowTitle.indexOf("系统变量") > -1) tagList = mSysTagList;
    else ;

    int start = windowTitle.indexOf("(");
    int end = windowTitle.indexOf(")");
    QString tmp =  windowTitle.mid(start+1, end-start-1);
    QStringList list = tmp.split('-');
    if(list.count() != 2) return;
    int idStart = QString(list[0]).toInt();
    int idEnd = QString(list[1]).toInt();

    int iPos = -1;
    QString szIDTmp = "";
    QString szTmp = "0";

    szIDTmp = tagList.at(idStart-1)->mId;
    iPos = szIDTmp.lastIndexOf(".");
    szTmp = szIDTmp.right(szIDTmp.length() - iPos - 1);
    idStart = szTmp.toInt();

    szIDTmp = tagList.at(idEnd-1)->mId;
    iPos = szIDTmp.lastIndexOf(".");
    szTmp = szIDTmp.right(szIDTmp.length() - iPos - 1);
    idEnd = szTmp.toInt();

    url_ = QString("http://%1:%2/%3").arg(ip_).arg(port_).arg("READ");
    QUrl url(url_);
    QJsonObject json;
    json["StartID"] = tagList.at(idStart-1)->mId;
    json["Number"] = idEnd-idStart+1;
    QJsonDocument document;
    document.setObject(json);
    QByteArray datas = document.toJson(QJsonDocument::Compact);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));
    m_networkAccessManager->post(request, datas);

#if 0
    QEventLoop eventLoop;
    QObject::connect(m_networkAccessManager, &QNetworkAccessManager::finished,
                     &eventLoop, &QEventLoop::quit);
    eventLoop.exec();
#endif
}


/*
* 插槽：写实时数据库
*/
void MainWindow::writeRtdbTag(QString cmdline)
{
    if(!bConnectStatus_) {
        QMessageBox::information(this, tr("提示"), tr("未连接实时数据库！"));
        return;
    }

    QStringList cmdLineList = cmdline.split('=');
    if(cmdLineList.count() != 2) return;
    QString idString = cmdLineList.at(0);
    QString valueString = cmdLineList.at(1);

    QList<QMdiSubWindow *> listSubWin = ui->mdiArea->subWindowList();
    if(listSubWin.size() < 1) return;
    MdiChildWindow* pMdiChildWindow =  qobject_cast<MdiChildWindow *>(listSubWin.at(0)->widget()); ;
    QString winTitle = pMdiChildWindow->windowTitle();

    url_ = QString("http://%1:%2/%3").arg(ip_).arg(port_).arg("WRITE");
    QUrl url(url_);
    QJsonObject jsonWrite;
    jsonWrite["StartID"] = idString;
    jsonWrite["Number"] = 1;
    QJsonArray tagArray;
    QJsonObject jsonObj;
    jsonObj[idString] = valueString;
    tagArray.append(jsonObj);
    jsonWrite["TagArray"] = tagArray;

    QJsonDocument document;
    document.setObject(jsonWrite);
    QByteArray datas = document.toJson(QJsonDocument::Compact);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));
    m_networkAccessManager->post(request, datas);
#if 0
    QEventLoop eventLoop;
    QObject::connect(m_networkAccessManager, &QNetworkAccessManager::finished,
                     &eventLoop, &QEventLoop::quit);
    eventLoop.exec();
#endif
}


void MainWindow::finished(QNetworkReply *reply)
{
    QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    QVariant redirectionTargetUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray bytes = reply->readAll();
        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(bytes, &err);
        if (err.error == QJsonParseError::NoError && doc.isObject()) {
            QMutexLocker locker(&mutex_);
            if(reply->url().path().toUpper() == "/READ") {
                const QJsonObject jsonDocObj = doc.object();
                QString szStartID = jsonDocObj["StartID"].toString();
                int id = 0;

                QList<QMdiSubWindow *> listSubWin = ui->mdiArea->subWindowList();
                if(listSubWin.size() < 1) return;

                MdiChildWindow* pMdiChildWindow = qobject_cast<MdiChildWindow *>(listSubWin.at(0)->widget());
                QString windowTitle = "";
                if(pMdiChildWindow != Q_NULLPTR) windowTitle = pMdiChildWindow->windowTitle();
                if(windowTitle == "") return;

                QList<TagItem *> tagList = QList<TagItem *>();
                if(windowTitle.indexOf("IO设备") > -1) {
                    QMap<QString, QList<TagItem *>>::const_iterator iter = mMapIoTagList.begin();
                    while (iter != mMapIoTagList.end()) {
                        if(windowTitle.indexOf(iter.key()) > -1) tagList = iter.value();
                        ++iter;
                    }
                }
                else if(windowTitle.indexOf("中间变量") > -1) tagList = mTmpTagList;
                else if(windowTitle.indexOf("系统变量") > -1) tagList = mSysTagList;
                else ;

                int start = windowTitle.indexOf("(");
                int end = windowTitle.indexOf(")");
                QString tmp =  windowTitle.mid(start+1, end-start-1);
                QStringList list = tmp.split('-');
                if(list.count() != 2) return;
                int idStart = QString(list[0]).toInt();
                int idEnd = QString(list[1]).toInt();

                int iPos = -1;
                QString szIDTmp = "";
                QString szTmp = "0";

                szIDTmp = tagList.at(idStart-1)->mId;
                iPos = szIDTmp.lastIndexOf(".");
                szTmp = szIDTmp.right(szIDTmp.length() - iPos - 1);
                idStart = szTmp.toInt();

                szIDTmp = tagList.at(idEnd-1)->mId;
                iPos = szIDTmp.lastIndexOf(".");
                szTmp = szIDTmp.right(szIDTmp.length() - iPos - 1);
                idEnd = szTmp.toInt();

                int num = idEnd-idStart+1;
                idEnd = idStart+num;

                QJsonArray tagArray = jsonDocObj["TagArray"].toArray();
                id = idStart;
                for (int i = 0; i < tagArray.size(); ++i) {
                    QJsonObject jsonObj = tagArray[i].toObject();
                    QString szTagID =  QString("%1%2").arg(szStartID.left(iPos + 1)).arg(QString::number(id));
                    QString datVal = jsonObj[szTagID].toString();

                    if(id >= idStart && id <= idEnd) {
                        pMdiChildWindow->SetTagLogicValueAndStatus(szTagID, datVal, "");
                    }
                    id++;
                }
            } else if(reply->url().path().toUpper() == "/WRITE") {
                const QJsonObject jsonDocObj = doc.object();
                int startID = jsonDocObj["StartID"].toInt();
                int number = jsonDocObj["Number"].toInt();
                int status = jsonDocObj["Status"].toInt();

                qDebug() << "WRITE: "
                         << "startID=" << startID
                         << "number=" << number
                         << "status=" << status;
            }
            else ;
        }
    } else {
        // handle errors here
    }

    reply->deleteLater();
}
