#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "MdiChildWindow.h"
#include "RtdbConnectDialog.h"
#include "CommandLineParser.h"
#include "../Public/Public.h"
#include <QApplication>
#include <QSettings>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QJsonObject>
#include <QJsonDocument>
#include <QScopedPointer>
#include <QJsonArray>
#include <QJsonObject>
#include <QMessageBox>
#include <QEventLoop>
#include <QMutexLocker>
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
    timer_(nullptr)
{
    ui->setupUi(this);
    ReadSettings();
    Load(DATA_SAVE_FORMAT);
    InitWindow();
    m_networkAccessManager = new QNetworkAccessManager(this);
    connect(m_networkAccessManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(finished(QNetworkReply*)));
}

MainWindow::~MainWindow()
{
    delete ui;
    QMap<QString, QList<TagItem *>>::const_iterator iter = mMapIoTagList.begin();
    while (iter != mMapIoTagList.end())
    {
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

    if(timer_ != nullptr)
    {
        if(timer_->isActive()) timer_->stop();
        delete timer_;
        timer_ = nullptr;
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
    while (iter != mMapIoTagList.end())
    {
        QString sItemName = iter.key();
        QList<TagItem *> itemList = iter.value();
        if(itemList.count() > 0)
        {
            int tableCnt = itemList.count()/64;
            if(itemList.count()%64) tableCnt++;
            for(int i=0; i<tableCnt; i++)
            {
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

    if(mTmpTagList.count() > 0)
    {
        int tableTmpCnt = mTmpTagList.count()/64;
        if(mTmpTagList.count()%64) tableTmpCnt++;
        for(int i=0; i<tableTmpCnt; i++)
        {
            QString name = "中间变量";
            if(i*64+63<mTmpTagList.count()) name += QString(" (%1-%2)").arg(i*64+1).arg(i*64+64);
            else name += QString(" (%1-%2)").arg(i*64+1).arg(i*64+mTmpTagList.count()%64);
            pTreeTmpItem->appendRow(CreateTreeItem(name));
        }
    }

    pTreeSysItem = CreateTreeItem(tr("系统变量"));
    pTreeItemRoot->appendRow(pTreeSysItem);

    if(mSysTagList.count() > 0)
    {
        int tableSysCnt = mSysTagList.count()/64;
        if(mSysTagList.count()%64) tableSysCnt++;
        for(int i=0; i<tableSysCnt; i++)
        {
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
    return nullptr;
}


void MainWindow::SetActiveSubWindow(MdiChildWindow *window)
{
    if(!window) return;
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
    foreach(QMdiSubWindow* window, ui->mdiArea->subWindowList())
    {
        MdiChildWindow * pChildWin = qobject_cast<MdiChildWindow *>(window->widget());
        if(pChildWin->windowTitle() == windowTitle)
            return pChildWin;
    }
    return nullptr;
}


QMdiSubWindow* MainWindow::FindMdiSubWindow(const QString &windowTitle)
{
    foreach(QMdiSubWindow* window, ui->mdiArea->subWindowList())
    {
        MdiChildWindow * pChildWin = qobject_cast<MdiChildWindow *>(window->widget());
        if(pChildWin->windowTitle() == windowTitle)
            return window;
    }
    return nullptr;
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


QJsonObject MainWindow::LoadJsonObjectFromFile(SaveFormat saveFormat, QString f)
{
    QFile loadFile(f);
    if (!loadFile.open(QIODevice::ReadOnly)) return QJsonObject();
    QByteArray loadData = loadFile.readAll();
    QJsonDocument loadDoc(saveFormat == Json ? QJsonDocument::fromJson(loadData) : QJsonDocument::fromBinaryData(loadData));
    loadFile.close();
    return loadDoc.object();
}


bool MainWindow::Load(SaveFormat saveFormat)
{
    if(mProjectPath == "") return false;

    QDir projDir(mProjectPath);
    if(!projDir.exists()) return false;

    //-----------------系统变量------------------//
    QJsonObject jsonSysTags = LoadJsonObjectFromFile(saveFormat, mProjectPath + "/SysVarList.odb");
    if(jsonSysTags != QJsonObject())
    {
        QJsonArray tagSysArray = jsonSysTags["SysVarArray"].toArray();
        for (int i = 0; i < tagSysArray.size(); ++i)
        {
            QJsonObject jsonObj = tagSysArray[i].toObject();
            TagItem *pItem = new TagItem();
            pItem->mId = jsonObj["iID"].toInt();
            pItem->mName = jsonObj["sName"].toString();
            pItem->mDescription = jsonObj["sDescription"].toString();
            mSysTagList.append(pItem);
        }
    }

    //-----------------中间变量------------------//
    QJsonObject jsonTmpTags = LoadJsonObjectFromFile(saveFormat, mProjectPath + "/TmpVarList.odb");
    if(jsonTmpTags != QJsonObject())
    {
        QJsonArray TmpVarArray = jsonTmpTags["TmpVarArray"].toArray();
        for (int i = 0; i < TmpVarArray.size(); ++i)
        {
            QJsonObject jsonObj = TmpVarArray[i].toObject();
            TagItem *pItem = new TagItem();
            pItem->mId = jsonObj["iID"].toInt();
            pItem->mName = jsonObj["sName"].toString();
            pItem->mDescription = jsonObj["sDescription"].toString();
            mTmpTagList.append(pItem);
        }
    }
    //-----------------设备变量------------------//
    QJsonObject jsonDBVarList = LoadJsonObjectFromFile(saveFormat, mProjectPath + "/DBVarList.odb");
    if(jsonDBVarList != QJsonObject())
    {
        QJsonArray DevVarTabArray = jsonDBVarList["DevVarTabArray"].toArray();
        for (int i = 0; i < DevVarTabArray.size(); ++i)
        {
            QJsonObject jsonObj = DevVarTabArray[i].toObject();
            QString sTableName = jsonObj["name"].toString();
            mMapIoTagPageGroupId[sTableName]=jsonObj["pageid"].toInt();
            mIoTagTableNameList.append(sTableName);
            QJsonObject jsonIoTags = LoadJsonObjectFromFile(saveFormat, mProjectPath + "/DevVarList-" + sTableName + ".odb");
            if(jsonIoTags == QJsonObject()) return false;
            QJsonArray IOVarArray = jsonIoTags["IOVarArray"].toArray();
            for (int i = 0; i < IOVarArray.size(); ++i)
            {
                QJsonObject jsonObj = IOVarArray[i].toObject();
                TagItem *pItem = new TagItem();
                pItem->mId = jsonObj["iID"].toInt();
                pItem->mName = jsonObj["sName"].toString();
                pItem->mDescription = jsonObj["sDescription"].toString();
                pItem->mDeviceInfo = jsonObj["sIOConnect"].toString();
                mMapIoTagList[sTableName].append(pItem);
            }
        }
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

    foreach (QMdiSubWindow* window, ui->mdiArea->subWindowList())
    {
        MdiChildWindow *pChildWin = qobject_cast<MdiChildWindow *>(window->widget());
        if(pChildWin->windowTitle() == winTittle)
        {
            SetActiveSubWindow(pChildWin);
            return;
        }
        else
        {
            if(bConnectStatus_ && timer_ != NULL)
                if(timer_->isActive()) timer_->stop();
            window->close();
        }
    }

    QList<TagItem *> tagList = QList<TagItem *>();
    if(winTittle.indexOf("IO设备") > -1)
    {
        QMap<QString, QList<TagItem *>>::const_iterator iter = mMapIoTagList.begin();
        while (iter != mMapIoTagList.end())
        {
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

    if(bConnectStatus_ && timer_ != nullptr && !timer_->isActive())
        timer_->start(PERIOD);
}


void MainWindow::ShowFirstPage()
{
    QMap<QString, QList<TagItem *>>::const_iterator iter = mMapIoTagList.begin();
    while (iter != mMapIoTagList.end())
    {
        QString sItemName = iter.key();
        QList<TagItem *> itemList = iter.value();
        if(itemList.count() > 0)
        {
            int tableCnt = itemList.count()/64;
            if(itemList.count()%64) tableCnt++;
            for(int i=0; i<tableCnt; i++)
            {
                QString name = "IO设备-" + sItemName;
                if(i*64+63<itemList.count()) name += QString(" (%1-%2)").arg(i*64+1).arg(i*64+64);
                else name += QString(" (%1-%2)").arg(i*64+1).arg(i*64+itemList.count()%64);
                if(i==0)
                {
                    QMap<QString, QList<TagItem *>>::const_iterator it = mMapIoTagList.begin();
                    while (it != mMapIoTagList.end())
                    {
                        if(name.indexOf(it.key()) > -1)
                        {
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

    if(dlg->exec() == QDialog::Accepted)
    {
        if(bConnectStatus_) return;

        port_ = 60000;
        ip_ = dlg->GetIPAddress();
        qDebug() << ip_ << port_;
        bConnectStatus_ = true;
        ui->statusBar->showMessage(tr("连接成功！"));

        timer_ = new QTimer();
        timer_->start(PERIOD);
        connect(timer_, SIGNAL(timeout()), this, SLOT(timeout()));
    }
    else
    {
        if(dlg->GetOption() == 0)
        {
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
    if(!bConnectStatus_)
    {
        if(timer_ != nullptr)
        {
            if(timer_->isActive()) timer_->stop();
            delete timer_;
            timer_ = nullptr;
        }
        return;
    }

    QList<QMdiSubWindow *> listSubWin = ui->mdiArea->subWindowList();
    if(listSubWin.size() < 1) return;
    MdiChildWindow* pMdiChildWindow =  qobject_cast<MdiChildWindow *>(listSubWin.at(0)->widget());
    QString windowTitle = "";
    if(pMdiChildWindow != nullptr) windowTitle = pMdiChildWindow->windowTitle();
    if(windowTitle == "") return;

    QList<TagItem *> tagList = QList<TagItem *>();
    if(windowTitle.indexOf("IO设备") > -1)
    {
        QMap<QString, QList<TagItem *>>::const_iterator iter = mMapIoTagList.begin();
        while (iter != mMapIoTagList.end())
        {
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
    idStart = tagList.at(idStart-1)->mId;
    idEnd = tagList.at(idEnd-1)->mId;
    //qDebug()<< idStart << idEnd;

    int id = idStart;
    if(windowTitle.contains("IO设备")) id += 0 + mMapIoTagPageId[windowTitle] * 1000;
    else if(windowTitle.contains("中间变量")) id += 1000000;
    else if(windowTitle.contains("系统变量")) id += 2000000;
    else ;

    url_ = QString("http://%1:%2/%3").arg(ip_).arg(port_).arg("READ");
    QUrl url(url_);
    QJsonObject json;
    json["StartID"] = id;
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
    if(!bConnectStatus_)
    {
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

    int id = idString.toInt();
    if(winTitle.contains("IO设备")) id += 0 + mMapIoTagPageId[winTitle] * 1000;
    if(winTitle.contains("中间变量")) id += 1000000;

    url_ = QString("http://%1:%2/%3").arg(ip_).arg(port_).arg("WRITE");
    QUrl url(url_);
    QJsonObject jsonWrite;
    jsonWrite["StartID"] = id;
    jsonWrite["Number"] = 1;
    QJsonArray tagArray;
    QJsonObject jsonObj;
    jsonObj[QString::number(id)] = valueString;
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

    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();
        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(bytes, &err);
        if (err.error == QJsonParseError::NoError && doc.isObject())
        {
            QMutexLocker locker(&mutex_);
            if(reply->url().path().toUpper() == "/READ")
            {
                const QJsonObject jsonDocObj = doc.object();
                int startID = jsonDocObj["StartID"].toInt();
                int start_id = startID;
                int id = 0;

                QList<QMdiSubWindow *> listSubWin = ui->mdiArea->subWindowList();
                if(listSubWin.size() < 1) return;

                MdiChildWindow* pMdiChildWindow = qobject_cast<MdiChildWindow *>(listSubWin.at(0)->widget());
                QString windowTitle = "";
                if(pMdiChildWindow != nullptr) windowTitle = pMdiChildWindow->windowTitle();
                if(windowTitle == "") return;

                QList<TagItem *> tagList = QList<TagItem *>();
                if(windowTitle.indexOf("IO设备") > -1)
                {
                    QMap<QString, QList<TagItem *>>::const_iterator iter = mMapIoTagList.begin();
                    while (iter != mMapIoTagList.end())
                    {
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
                idStart = tagList.at(idStart-1)->mId;
                idEnd = tagList.at(idEnd-1)->mId;
                int num = idEnd-idStart+1;
                int pageid = 0;

                if(windowTitle.contains("IO设备"))
                {
                    pageid = mMapIoTagPageId[windowTitle];
                    idStart += 0 + pageid * 1000;
                }
                else if(windowTitle.contains("中间变量")) idStart += 1000000;
                else if(windowTitle.contains("系统变量")) idStart += 2000000;
                else ;

                idEnd = idStart+num;

                QJsonArray tagArray = jsonDocObj["TagArray"].toArray();
                for (int i = 0; i < tagArray.size(); ++i) {
                    id = start_id;
                    QJsonObject jsonObj = tagArray[i].toObject();
                    QString datVal = jsonObj[QString::number(id)].toString();

                    if(id >= idStart && id <= idEnd)
                    {
                        if(windowTitle.contains("IO设备"))
                        {
                            if(pageid > 0) id = id / mMapIoTagPageId[windowTitle] / 1000;
                            pMdiChildWindow->SetTagLogicValueAndStatus(id, datVal, "");
                        }
                        else if(windowTitle.contains("中间变量"))
                        {
                            id -= 1000000;
                            pMdiChildWindow->SetTagLogicValueAndStatus(id, datVal, "");
                        }
                        else if(windowTitle.contains("系统变量"))
                        {
                            id -= 2000000;
                            pMdiChildWindow->SetTagLogicValueAndStatus(id, datVal, "");
                        }
                    }
                    start_id++;
                }
            }
            else if(reply->url().path().toUpper() == "/WRITE")
            {
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
    }
    else
    {
        // handle errors here
    }

    reply->deleteLater();
}
