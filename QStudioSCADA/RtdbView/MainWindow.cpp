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
#include <QDebug>

#define PERIOD    (500)


MainWindow::MainWindow(QString projPath, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mProjectPath(projPath),
    bConnectStatus(false),
    tcpSocket(NULL),
    serverIP(NULL),
    pTimer(NULL)
{
    ui->setupUi(this);

    ReadSettings(); // 初始窗口时读取窗口设置信息
    Load(DATA_SAVE_FORMAT);
    InitWindow(); // 初始化窗口
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

    if(pTimer != NULL)
    {
        if(pTimer->isActive())
            pTimer->stop();
        delete pTimer;
        pTimer = NULL;
    }

    if(tcpSocket != NULL)
    {
        QByteArray outMsgBytes;
        outMsgBytes.clear();
        QDataStream out(&outMsgBytes, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_7);
        // send complete message
        out << QString("completed");
        tcpSocket->write(outMsgBytes, outMsgBytes.length());

        tcpSocket->disconnectFromHost();
        tcpSocket->waitForDisconnected();
        delete tcpSocket;
        tcpSocket = NULL;
    }

    if(serverIP != NULL)
    {
        delete serverIP;
    }
}


QStandardItem *MainWindow::CreateTreeItem(const QString name, bool bconnect)
{
    QStandardItem *pItem = new QStandardItem(name);
    pItem->setEditable(false);
    if(bconnect)
    {
        // 连接单击响应插槽

    }
    return pItem;
}


void MainWindow::InitTreeViewUi()
{
    ui->TagTreeView->setHeaderHidden(true);
    pTagTreeViewModel = new QStandardItemModel();

    pTreeItemRoot = new QStandardItem(tr("实时数据库"));
    pTreeItemRoot->setEditable(false);
    pTreeIoItem = CreateTreeItem(tr("IO变量"), false);
    pTreeItemRoot->appendRow(pTreeIoItem);

    QMap<QString, QList<TagItem *>>::const_iterator iter = mMapIoTagList.begin();
    while (iter != mMapIoTagList.end())
    {
        QString sItemName = iter.key();
        QList<TagItem *> itemList = iter.value();
        if(itemList.count() > 0)
        {
            int tableCnt = itemList.count()/64;
            if(itemList.count()%64)
                tableCnt++;
            for(int i=0; i<tableCnt; i++)
            {
                QString name = "IO设备-" + sItemName;
                if(i*64+63<itemList.count())
                {
                    name += QString(" (%1-%2)").arg(i*64+1).arg(i*64+64);
                }
                else
                {
                    name += QString(" (%1-%2)").arg(i*64+1).arg(i*64+itemList.count()%64);
                }
                pTreeIoItem->appendRow(CreateTreeItem(name, true));
                mMapIoTagPageId[name] = mMapIoTagPageGroupId[sItemName];
                mMapIoTagPageGroupId.clear();
            }
        }
        ++iter;
    }

    pTreeTmpItem = CreateTreeItem(tr("中间变量"), false);
    pTreeItemRoot->appendRow(pTreeTmpItem);

    if(mTmpTagList.count() > 0)
    {
        int tableTmpCnt = mTmpTagList.count()/64;
        if(mTmpTagList.count()%64)
            tableTmpCnt++;
        for(int i=0; i<tableTmpCnt; i++)
        {
            QString name = "中间变量";
            if(i*64+63<mTmpTagList.count())
            {
                name += QString(" (%1-%2)").arg(i*64+1).arg(i*64+64);
            }
            else
            {
                name += QString(" (%1-%2)").arg(i*64+1).arg(i*64+mTmpTagList.count()%64);
            }
            pTreeTmpItem->appendRow(CreateTreeItem(name, true));
        }
    }

    pTreeSysItem = CreateTreeItem(tr("系统变量"), false);
    pTreeItemRoot->appendRow(pTreeSysItem);

    if(mSysTagList.count() > 0)
    {
        int tableSysCnt = mSysTagList.count()/64;
        if(mSysTagList.count()%64)
            tableSysCnt++;
        for(int i=0; i<tableSysCnt; i++)
        {
            QString name = "系统变量";
            if(i*64+63<mSysTagList.count())
            {
                name += QString(" (%1-%2)").arg(i*64+1).arg(i*64+64);
            }
            else
            {
                name += QString(" (%1-%2)").arg(i*64+1).arg(i*64+mSysTagList.count()%64);
            }
            pTreeSysItem->appendRow(CreateTreeItem(name, true));
        }
    }

    pTagTreeViewModel->appendRow(pTreeItemRoot);
    ui->TagTreeView->setModel(pTagTreeViewModel);
    ui->TagTreeView->expandAll();

}


MdiChildWindow* MainWindow::ActiveMdiChild()
{
    // 如果有活动窗口，则将其内的中心部件转换为MdiChild类型
    if (QMdiSubWindow *activeSubWindow = ui->mdiArea->activeSubWindow())
    {
        return qobject_cast<MdiChildWindow *>(activeSubWindow->widget());
    }
    return NULL; // 没有活动窗口，直接返回0
}


void MainWindow::SetActiveSubWindow(MdiChildWindow *window)
{
    if(!window)
        return;
    window->showMaximized();
    //m_CurItem = window->windowTitle();
    ui->mdiArea->setActiveSubWindow(0);//Activates the subwindow window. If window is 0, any current active window is deactivated.
    ui->mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow*>(window));
}


MdiChildWindow* MainWindow::GetActiveSubWindow()
{
    return qobject_cast<MdiChildWindow*>(ui->mdiArea->activeSubWindow()->widget());
}



MdiChildWindow* MainWindow::FindMdiChild(const QString &windowTitle)
{
    foreach (QMdiSubWindow* window, ui->mdiArea->subWindowList())
    {
        MdiChildWindow * pChildWin = qobject_cast<MdiChildWindow *>(window->widget());
        if(pChildWin->windowTitle() == windowTitle)
            return pChildWin;
    }
    return NULL;
}


QMdiSubWindow* MainWindow::FindMdiSubWindow(const QString &windowTitle)
{
    foreach (QMdiSubWindow* window, ui->mdiArea->subWindowList())
    {
        MdiChildWindow * pChildWin = qobject_cast<MdiChildWindow *>(window->widget());
        if(pChildWin->windowTitle() == windowTitle)
            return window;
    }
    return NULL;
}



void MainWindow::closeEvent(QCloseEvent *event) // 关闭事件
{
    QString strFile = QCoreApplication::applicationDirPath() + "/lastpath.ini";
    //ConfigUtils::SetCfgStr(strFile, "RtdbConnectIp", "IP", "");
    ui->mdiArea->closeAllSubWindows(); // 先执行多文档区域的关闭操作
    if (ui->mdiArea->currentSubWindow()) {
        event->ignore(); // 如果还有窗口没有关闭，则忽略该事件
    } else {
        WriteSettings(); // 在关闭前写入窗口设置
        event->accept();
    }
}

void MainWindow::WriteSettings() // 写入窗口设置
{
    QSettings settings("Organization", "RtdbView");
    settings.setValue("pos", pos());   // 写入位置信息
    settings.setValue("size", size()); // 写入大小信息
}

void MainWindow::ReadSettings() // 读取窗口设置
{
    QSettings settings("Organization", "RtdbView");
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(800, 600)).toSize();
    move(pos);
    resize(size);
}

void MainWindow::InitWindow() // 初始化窗口
{
    setCentralWidget(ui->mdiArea);
//    setWindowState(Qt::WindowMaximized);
    setWindowTitle(tr("实时数据库"));


    // 当多文档区域的内容超出可视区域后，出现滚动条
    ui->mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    ui->statusBar->showMessage(tr("未连接..."));



    InitTreeViewUi();

}


QJsonObject MainWindow::LoadJsonObjectFromFile(SaveFormat saveFormat, QString f)
{
    QFile loadFile(f);
    if (!loadFile.open(QIODevice::ReadOnly))
        return QJsonObject();
    QByteArray loadData = loadFile.readAll();
    QJsonDocument loadDoc(saveFormat == Json ? QJsonDocument::fromJson(loadData) : QJsonDocument::fromBinaryData(loadData));
    loadFile.close();
    return loadDoc.object();
}


bool MainWindow::Load(SaveFormat saveFormat)
{
    if(mProjectPath == "")
        return false;

    QDir projDir(mProjectPath);
    if(!projDir.exists())
        return false;

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
            if(jsonIoTags == QJsonObject())
                return false;
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


/*
* 根据单击树选项显示子窗口
*/
void MainWindow::on_TagTreeView_clicked(const QModelIndex &index)
{
    QStandardItem *item = pTagTreeViewModel->itemFromIndex(index);
    QString winTittle = item->text();
    if(!winTittle.endsWith(")"))
        return;

    foreach (QMdiSubWindow* window, ui->mdiArea->subWindowList())
    {
        MdiChildWindow * pChildWin = qobject_cast<MdiChildWindow *>(window->widget());
        if(pChildWin->windowTitle() == winTittle)
        {
            SetActiveSubWindow(pChildWin);
            return;
        }
        else
        {
            if(bConnectStatus)
            {
                if(pTimer != NULL)
                {
                    if(pTimer->isActive())
                        pTimer->stop();
                }
            }
            window->close();
        }
    }

    QList<TagItem *> tagList = QList<TagItem *>();
    if(winTittle.indexOf("IO设备") > -1)
    {
        QMap<QString, QList<TagItem *>>::const_iterator iter = mMapIoTagList.begin();
        while (iter != mMapIoTagList.end())
        {
            if(winTittle.indexOf(iter.key()) > -1)
            {
                tagList = iter.value();
            }
            ++iter;
        }
    }
    else if(winTittle.indexOf("中间变量") > -1)
    {
        tagList = mTmpTagList;
    }
    else if(winTittle.indexOf("系统变量") > -1)
    {
        tagList = mSysTagList;
    }
    else
    {
    }

    MdiChildWindow* pMdiChildWindow = new MdiChildWindow(tagList, winTittle, this);
    connect(pMdiChildWindow,SIGNAL(writeRtdbTag(QString)),this,SLOT(writeRtdbTag(QString)));
    ui->mdiArea->addSubWindow(pMdiChildWindow);
    SetActiveSubWindow(pMdiChildWindow);

    if(bConnectStatus)
    {
        if(pTimer != NULL)
        {
            if(!pTimer->isActive())
                pTimer->start(PERIOD);
        }
    }
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
            if(itemList.count()%64)
                tableCnt++;
            for(int i=0; i<tableCnt; i++)
            {
                QString name = "IO设备-" + sItemName;
                if(i*64+63<itemList.count())
                {
                    name += QString(" (%1-%2)").arg(i*64+1).arg(i*64+64);
                }
                else
                {
                    name += QString(" (%1-%2)").arg(i*64+1).arg(i*64+itemList.count()%64);
                }
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


/*
* 连接实时数据库
*/
void MainWindow::on_actionConnect_triggered()
{
    QScopedPointer<RtdbConnectDialog> dlg(new RtdbConnectDialog(this));
    if(bConnectStatus)
    {
        dlg->SetConnectStatus(tr("连接状态：已连接"));
    }
    else
    {
        dlg->SetConnectStatus(tr("连接状态：连接关闭"));
    }
    if(dlg->exec() == QDialog::Accepted)
    {
        if(bConnectStatus)
            return;
        tcpSocket = new QTcpSocket(this);
        connect(tcpSocket,SIGNAL(connected()),this,SLOT(slotConnected()));
        connect(tcpSocket,SIGNAL(disconnected()),this,SLOT(slotDisconnected()));
        connect(tcpSocket, SIGNAL(readyRead()),this, SLOT(dataReceived()));

        port = 6000;
        serverIP = new QHostAddress();
        bConnectStatus = false;

        if(!serverIP->setAddress(dlg->GetIPAddress()))
        {
            //qDebug() << "server ip address error!";
            QMessageBox::critical(this, tr("ip错误"), tr("ip设置错误！请重新设置！"));
            if(tcpSocket != NULL)
            {
                delete tcpSocket;
                tcpSocket = NULL;
            }
            return;
        }
        //qDebug() << *serverIP << port;
        tcpSocket->connectToHost (*serverIP, port);
        if (tcpSocket->waitForConnected(1000))
        {
            //qDebug("Connected!");
            bConnectStatus = true;
            ui->statusBar->showMessage(tr("连接成功！"));
        }
        else
        {
            QMessageBox::critical(this, tr("连接失败"), tr("连接至Runtime失败！"));
            if(tcpSocket != NULL)
            {
                delete tcpSocket;
                tcpSocket = NULL;
            }
            return;
        }
    }
    else
    {
        if(dlg->GetOption() == 0)
        {
            if(tcpSocket != NULL)
            {
                QByteArray outMsgBytes;
                outMsgBytes.clear();
                QDataStream out(&outMsgBytes, QIODevice::WriteOnly);
                out.setVersion(QDataStream::Qt_5_7);
                // send complete message
                out << QString("completed");
                tcpSocket->write(outMsgBytes, outMsgBytes.length());

                tcpSocket->disconnectFromHost();
                tcpSocket->waitForDisconnected();
                delete tcpSocket;
                bConnectStatus = false;
                ui->statusBar->showMessage(tr("未连接..."));
                tcpSocket = NULL;
            }
        }
    }
}


void MainWindow::slotConnected()
{
    QByteArray msgBytes;
    QDataStream out(&msgBytes, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_7);

    QString msg = QString("connect to server");
    out << msg;

    tcpSocket->write(msgBytes, msgBytes.length());

    pTimer = new QTimer();
    pTimer->start(PERIOD);
    connect(pTimer,SIGNAL(timeout()), this, SLOT(timeout()));

}


void MainWindow::slotDisconnected()
{
    //qDebug() << "slotDisconnected...";

}


void MainWindow::timeout()
{   
    if(!bConnectStatus)
    {
        if(pTimer != NULL)
        {
            if(pTimer->isActive())
                pTimer->stop();
            delete pTimer;
            pTimer = NULL;
        }
        return;
    }

    QList<QMdiSubWindow *> listSubWin = ui->mdiArea->subWindowList();
    if(listSubWin.size() < 1)
        return;
    MdiChildWindow* pMdiChildWindow =  qobject_cast<MdiChildWindow *>(listSubWin.at(0)->widget()); ;

    QString windowTitle = pMdiChildWindow->windowTitle();
    //qDebug()<< windowTitle << mMapIoTagPageId[windowTitle];

    QList<TagItem *> tagList = QList<TagItem *>();
    if(windowTitle.indexOf("IO设备") > -1)
    {
        QMap<QString, QList<TagItem *>>::const_iterator iter = mMapIoTagList.begin();
        while (iter != mMapIoTagList.end())
        {
            if(windowTitle.indexOf(iter.key()) > -1)
            {
                tagList = iter.value();
            }
            ++iter;
        }
    }
    else if(windowTitle.indexOf("中间变量") > -1)
    {
        tagList = mTmpTagList;
    }
    else if(windowTitle.indexOf("系统变量") > -1)
    {
        tagList = mSysTagList;
    }
    else
    {
    }

    int start = windowTitle.indexOf("(");
    int end = windowTitle.indexOf(")");
    QString tmp =  windowTitle.mid(start + 1, end -start-1);
    QStringList list = tmp.split('-');
    if(list.count() != 2)
        return;
    int idStart = QString(list[0]).toInt();
    int idEnd = QString(list[1]).toInt();
    idStart = tagList.at(idStart-1)->mId;
    idEnd = tagList.at(idEnd-1)->mId;
    //qDebug()<< idStart << idEnd;

    QByteArray outMsgBytes;
    outMsgBytes.clear();
    QDataStream out(&outMsgBytes, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_7);

    QString outMsg = "";
    outMsgBytes.clear();
    QString cmdline = "read ";
    int id = idStart;
    if(windowTitle.contains("IO设备"))
    {
        id += 0 + mMapIoTagPageId[windowTitle] * 1000;
    }
    else if(windowTitle.contains("中间变量"))
    {
        id += 1000000;
    }
    else if(windowTitle.contains("系统变量"))
    {
        id += 2000000;
    }
    else
    {}
    cmdline += QString("%1").arg(id);
    cmdline += ",";
    cmdline += QString("%1").arg(idEnd - idStart + 1);
    cmdline += ";";
    out << cmdline;
    tcpSocket->write(outMsgBytes, outMsgBytes.length());

}

/*

command: read id,number;
ret: read number,id=value,...,id=value;
     read error,error_reason_string;

command: write number,id=value,...,id=value;
ret: write ok;
     write error,error_reason_string;

*/

/*
outMsgBytes.clear();
outMsg = QString("write 2,1=%1,2=%2;").arg(val1++).arg(val2++);
qDebug() << "timerEvent    "<< outMsg;
out << outMsg;
tcpSocket->write(outMsgBytes, outMsgBytes.length());
*/
void MainWindow::dataReceived()
{
    while(tcpSocket->bytesAvailable()>0)
    {
        QByteArray datagram;
        datagram.resize(tcpSocket->bytesAvailable());
        tcpSocket->read(datagram.data(),datagram.size());
        //QString msg = datagram.data();
        //qDebug() << msg.left(datagram.size());
        QDataStream in(&datagram, QIODevice::ReadOnly);
        in.setVersion(QDataStream::Qt_5_7);
        QString msg = onDataTransfer(in);
        //if(msg != "")
        //    qDebug() << msg;
    }
}


QString MainWindow::onDataTransfer(QDataStream& in)
{
    QByteArray outMsgBytes;
    outMsgBytes.clear();
    QDataStream out(&outMsgBytes, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_7);

    QString inMsg = "";
    QString outMsg = "";

    in >> inMsg;
    //qDebug() << "server return: " << inMsg;

    if(inMsg == "connect to server")
    {
        outMsgBytes.clear();
        outMsg = QString("read write rtdb");
        out << outMsg;
        tcpSocket->write(outMsgBytes, outMsgBytes.length());
    }
    if(inMsg == "read write rtdb")
    {
        //outMsgBytes.clear();
        //outMsg = QString("read write rtdb");
        //out << outMsg;
        //tcpSocket->write(outMsgBytes, outMsgBytes.length());
    }
    else if(inMsg == "completed")
    {
        outMsgBytes.clear();
        // send complete message
        out << QString("completed");
        tcpSocket->write(outMsgBytes, outMsgBytes.length());
        this->bConnectStatus = false;
        ui->statusBar->showMessage(tr("未连接..."));
    }
    else
    {
        if(inMsg.startsWith("read") && inMsg.endsWith(";"))
            ReadParser(inMsg);
        if(inMsg.startsWith("write") && inMsg.endsWith(";"))
            WriteParser(inMsg);
    }
    return "";
}

/*
* 读命令返回解析
* command: read id,number;
* ret: read number,id=value,...,id=value;
*      read error,error_reason_string;
*/
void MainWindow::ReadParser(QString cmdline)
{
    //qDebug()<< "ReadParser" << cmdline;
    QByteArray outMsgBytes;
    outMsgBytes.clear();
    QDataStream out(&outMsgBytes, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_7);

    CommandLineParser parser(cmdline);
    if(parser.mCmd == "read")
    {
        if(parser.mArgs.count() < 2)
            return;
        int num = QString(parser.mArgs.at(0)).toInt();
        for(int i=0; i<num; i++)
        {
            QStringList listArgs = parser.mArgs.at(1+i).split('=');
            int id = QString(listArgs.at(0)).toInt();
            QString datVal = listArgs.at(1);
            //qDebug()<< "read " << id << datVal;

            QList<QMdiSubWindow *> listSubWin = ui->mdiArea->subWindowList();
            if(listSubWin.size() < 1)
                return;
            MdiChildWindow* pMdiChildWindow =  qobject_cast<MdiChildWindow *>(listSubWin.at(0)->widget()); ;

            QString windowTitle = pMdiChildWindow->windowTitle();

            QList<TagItem *> tagList = QList<TagItem *>();
            if(windowTitle.indexOf("IO设备") > -1)
            {
                QMap<QString, QList<TagItem *>>::const_iterator iter = mMapIoTagList.begin();
                while (iter != mMapIoTagList.end())
                {
                    if(windowTitle.indexOf(iter.key()) > -1)
                    {
                        tagList = iter.value();
                    }
                    ++iter;
                }
            }
            else if(windowTitle.indexOf("中间变量") > -1)
            {
                tagList = mTmpTagList;
            }
            else if(windowTitle.indexOf("系统变量") > -1)
            {
                tagList = mSysTagList;
            }
            else
            {
            }

            int start = windowTitle.indexOf("(");
            int end = windowTitle.indexOf(")");
            QString tmp =  windowTitle.mid(start + 1, end -start-1);
            QStringList list = tmp.split('-');
            if(list.count() != 2)
                return;
            int idStart = QString(list[0]).toInt();
            int idEnd = QString(list[1]).toInt();
            idStart = tagList.at(idStart-1)->mId;
            idEnd = tagList.at(idEnd-1)->mId;
            int num = idEnd - idStart + 1;
            int pageid = 0;

            if(windowTitle.contains("IO设备"))
            {
                pageid = mMapIoTagPageId[windowTitle];
                idStart += 0 + pageid * 1000;
            }
            else if(windowTitle.contains("中间变量"))
            {
                idStart += 1000000;
            }
            else if(windowTitle.contains("系统变量"))
            {
                idStart += 2000000;
            }
            else
            {}
            idEnd = idStart + num;
            if(id >= idStart && id <= idEnd)
            {
                if(windowTitle.contains("IO设备"))
                {
                    if(pageid > 0)
                    {
                        id = id / mMapIoTagPageId[windowTitle] / 1000;
                    }
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
        }
    }
}

/*
* 写命令返回解析
* command: write number,id=value,...,id=value;
* ret: write ok;
*      write error,error_reason_string;
*/
void MainWindow::WriteParser(QString cmdline)
{
    //qDebug()<< "WriteParser" << cmdline;
}


void MainWindow::on_actionClose_triggered()
{
    WriteSettings();
    qApp->exit();
}

/*
* 插槽：写实时数据库
*/
void MainWindow::writeRtdbTag(QString cmdline)
{
    //qDebug()<<cmdline;
    if(!bConnectStatus)
    {
        QMessageBox::information(this, tr("提示"), tr("未连接实时数据库！"));
        return;
    }

    QStringList cmdLineList = cmdline.split('=');
    if(cmdLineList.count() != 2)
        return;
    QString idString = cmdLineList.at(0);
    QString valueString = cmdLineList.at(1);

    QList<QMdiSubWindow *> listSubWin = ui->mdiArea->subWindowList();
    if(listSubWin.size() < 1)
        return;
    MdiChildWindow* pMdiChildWindow =  qobject_cast<MdiChildWindow *>(listSubWin.at(0)->widget()); ;
    QString winTitle = pMdiChildWindow->windowTitle();

    QString writeCmdline = "write 1,";
    int id = idString.toInt();
    if(winTitle.contains("IO设备"))
    {
        id += 0 + mMapIoTagPageId[winTitle] * 1000;
    }
    if(winTitle.contains("中间变量"))
    {
        id += 1000000;
    }

    writeCmdline += QString("%1").arg(id);
    writeCmdline += "=";
    writeCmdline += valueString;
    writeCmdline += ";";
    //qDebug()<< writeCmdline;

    QByteArray outMsgBytes;
    QDataStream out(&outMsgBytes, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_7);
    outMsgBytes.clear();
    out << writeCmdline;
    tcpSocket->write(outMsgBytes, outMsgBytes.length());
}
