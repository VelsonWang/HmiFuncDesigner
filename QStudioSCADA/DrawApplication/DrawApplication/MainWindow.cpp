
#include <QMdiSubWindow>
#include <QSettings>
#include <QStandardItem>
#include <QFileDialog>
#include <QMessageBox>
#include <QPluginLoader>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDir>

#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "ToolButton.h"
#include "configutils.h"
#include "DrawListUtils.h"
#include "qtpropertymanager.h"
#include "qtvariantproperty.h"
#include "qttreepropertybrowser.h"

#include <QDebug>



MainWindow::MainWindow(QString projpath, QString drawPageName, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_ProjectPath(projpath)
{
    ui->setupUi(this);
    readSettings(); // 初始窗口时读取窗口设置信息
    initWindow(); // 初始化窗口
    LoadPlugin(); // 加载工具插件
    LoadDrawNameList();
    TreeViewGraphUISetting(); // 图形列表UI设置
    ToolBoxUISetting(); // 工具箱UI设置

    ui->propertyEditor->setHeaderLabels(QStringList() << tr("属性名称") << tr("属性值"));
}

MainWindow::~MainWindow()
{
    ReleasePlugin();
    delete ui;
}

/*
* load pagegraph name list from DrawList.xml
*/
void MainWindow::LoadDrawNameList()
{
    DrawListUtils::LoadDrawList(m_ProjectPath, this->m_GraphPageNameList);
}


/*
* save pagegraph name list to DrawList.xml
*/
void MainWindow::SaveDrawNameList()
{
    DrawListUtils::SaveDrawList(m_ProjectPath, this->m_GraphPageNameList);
}


/*
* 打开画面
*/
void MainWindow::doOpenDraw(QString file)
{
    QString fileNamestr = file.mid(file.lastIndexOf("/") + 1, file.indexOf(".") - file.lastIndexOf("/") - 1);
    //qDebug()<< fileNamestr;

    QFile loadFile(file);
    if (!loadFile.open(QIODevice::ReadOnly))
    {
        return;
    }

    QByteArray saveData = loadFile.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
    //QJsonDocument loadDoc(QJsonDocument::fromBinaryData(saveData));

    ChildGraphWin *pChildGraphWin = createMdiChild(file, fileNamestr);

    const QJsonObject &json = loadDoc.object();
    pChildGraphWin->load(json);
    QJsonArray elementArray = json["elements"].toArray();

    for (int idx = 0; idx < elementArray.size(); idx++)
    {
        bool found = false;
        QJsonObject elementObj = elementArray[idx].toObject();
        QMapIterator<QString, QMap<QString, IDrawApplicationPlugin*> > it(m_toolPlugins);
        while(it.hasNext())
        {
            if(found)
                break;
            it.next();
            QMapIterator<QString, IDrawApplicationPlugin*>  t(it.value());
            while(t.hasNext())
            {
                t.next();
                IDrawApplicationPlugin* pPlugin = t.value();
                if((int)pPlugin->GetPluginID() == elementObj["GraphType"].toInt())
                {
                    StdElement *pStdObj = pPlugin->CreateElement();
                    IElement *pIElement = pStdObj->GetInterface();
                    if(pIElement)
                        pIElement->load(elementObj);
                    pChildGraphWin->AddGraphElement(pStdObj);
                    found = true;
                    break;
                }
            }
        }
    }
    loadFile.close();
}

void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("打开文件"), QDir::currentPath(), "*.drw");

    if(fileName.isEmpty())
        return;

    QString filePathName = fileName;
    if(filePathName.lastIndexOf(".drw") < 0)
        filePathName += ".drw";

    doOpenDraw(filePathName);
}

void MainWindow::on_actionSave_triggered()
{
    foreach (QMdiSubWindow* window, ui->mdiArea->subWindowList())
    {
        ChildGraphWin * pChildWin = qobject_cast<ChildGraphWin *>(window->widget());
        if(pChildWin->isModified)
        {
            if(pChildWin->m_FilePathName.isEmpty())
            {
                pChildWin->m_FilePathName = QFileDialog::getSaveFileName(this, tr("保存文件"), QDir::currentPath(), "*.drw");
                if(pChildWin->m_FilePathName.isEmpty())
                    return;
            }

            QString filePathName = pChildWin->m_FilePathName;
            if(filePathName.lastIndexOf(".drw") < 0)
                filePathName += ".drw";

            QFile saveFile(filePathName);
            if (!saveFile.open(QIODevice::WriteOnly))
            {
                return;
            }

            QJsonObject jsonObject;
            pChildWin->save(jsonObject);
            QJsonDocument saveDoc(jsonObject);
            saveFile.write(saveDoc.toJson());
            //saveFile.write(saveDoc.toBinaryData());
            saveFile.close();
        }
    }
}

void MainWindow::on_actionNew_triggered()
{
    int last = 0;
    QString strDrawPageName = "";

    last = DrawListUtils::GetMaxDrawPageNum("draw", this->m_GraphPageNameList);
    strDrawPageName = QString("draw%1").arg(last);

    this->m_GraphPageNameList.append(strDrawPageName);
    TreeViewGraphUpdate();

    QString file = m_ProjectPath + "/" + strDrawPageName + ".drw";
    ChildGraphWin * pChildWin = createMdiChild(file, strDrawPageName);
    pChildWin->isModified = true;
}


/*
* 创建子窗口,并返回子窗口
*/
ChildGraphWin* MainWindow::createMdiChild(QString filePathName, QString page)
{
    ChildGraphWin *pChildGraphWin = new ChildGraphWin(filePathName, page, this);
    ui->mdiArea->addSubWindow(pChildGraphWin);
    pChildGraphWin->GetGraphPageProperty(ui->propertyEditor);
    pChildGraphWin->SetPropertyBrowser(ui->propertyEditor);
    pChildGraphWin->showMaximized();
    return pChildGraphWin;
}


/*
* 活动窗口
*/
ChildGraphWin* MainWindow::activeMdiChild()
{
    // 如果有活动窗口，则将其内的中心部件转换为MdiChild类型
    if (QMdiSubWindow *activeSubWindow = ui->mdiArea->activeSubWindow())
    {
        return qobject_cast<ChildGraphWin *>(activeSubWindow->widget());
    }
    return NULL; // 没有活动窗口，直接返回0
}


ChildGraphWin* MainWindow::findMdiChild(const QString &windowTitle)
{
    foreach (QMdiSubWindow* window, ui->mdiArea->subWindowList()) {
        ChildGraphWin * pChildWin = qobject_cast<ChildGraphWin *>(window->widget());
        if(pChildWin->windowTitle() == windowTitle)
            return pChildWin;
    }
    return NULL;
}


/*
* 写入窗口设置
*/
void MainWindow::writeSettings()
{
    QSettings settings("Organization", "HMIFuncDesign");
    settings.setValue("pos", pos());   // 写入位置信息
    settings.setValue("size", size()); // 写入大小信息
}


/*
* 读取窗口设置
*/
void MainWindow::readSettings()
{
    QSettings settings("Organization", "HMIFuncDesign");
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(800, 600)).toSize();
    move(pos);
    resize(size);
}


/*
* 初始化窗口
*/
void MainWindow::initWindow()
{
    setCentralWidget(ui->mdiArea);
//    setWindowState(Qt::WindowMaximized);
    setWindowTitle(tr("绘制系统"));


    // 当多文档区域的内容超出可视区域后，出现滚动条
    ui->mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    ui->statusBar->showMessage(tr("欢迎使用绘制系统"));




}


void MainWindow::setActiveSubWindow(ChildGraphWin *window)
{
    if(!window)
        return;
    window->showMaximized();
    ui->mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow*>(window));
}


ChildGraphWin* MainWindow::getActiveSubWindow()
{
    return qobject_cast<ChildGraphWin*>(ui->mdiArea->activeSubWindow());
}

/*
* 关闭事件
*/
void MainWindow::closeEvent(QCloseEvent *event)
{
    on_actionSave_triggered();
    ui->mdiArea->closeAllSubWindows(); // 先执行多文档区域的关闭操作
    if (ui->mdiArea->currentSubWindow())
    {
        event->ignore(); // 如果还有窗口没有关闭，则忽略该事件
    }
    else
    {
        writeSettings(); // 在关闭前写入窗口设置
        event->accept();
    }

    SaveDrawNameList();
}


/*
* 图形列表UI设置
*/
void MainWindow::TreeViewGraphUISetting()
{
    ui->treeViewGraph->setHeaderHidden(true);

    pTreeViewGraphModel = new QStandardItemModel();
    QStandardItem *pGraphListItem = new QStandardItem(tr("图形列表"));
    pGraphListItem->setEditable(false);

    for(int i=0; i<this->m_GraphPageNameList.count(); i++)
    {
        QStandardItem *pGraphItem = new QStandardItem(QIcon(":/images/pjj_draw.png"), this->m_GraphPageNameList.at(i));
        pGraphItem->setEditable(false);
        pGraphListItem->appendRow(pGraphItem);

    }

    pTreeViewGraphModel->appendRow(pGraphListItem);
    ui->treeViewGraph->setModel(pTreeViewGraphModel);
    ui->treeViewGraph->expandAll();
}


/*
* 图形列表更新
*/
void MainWindow::TreeViewGraphUpdate()
{
    pTreeViewGraphModel->clear();
    ui->treeViewGraph->setHeaderHidden(true);

    QStandardItem *pGraphListItem = new QStandardItem(tr("图形列表"));
    pGraphListItem->setEditable(false);

    for(int i=0; i<this->m_GraphPageNameList.count(); i++)
    {
        QStandardItem *pGraphItem = new QStandardItem(QIcon(":/images/pjj_draw.png"), this->m_GraphPageNameList.at(i));
        pGraphItem->setEditable(false);
        pGraphListItem->appendRow(pGraphItem);

    }

    pTreeViewGraphModel->appendRow(pGraphListItem);
    ui->treeViewGraph->setModel(pTreeViewGraphModel);
    ui->treeViewGraph->expandAll();
}

/*
* 控件工具箱被单击
*/
void MainWindow::ToolButtonClick(QString s)
{
    if(s != "选择工具")
    {
        StdElement *pStdObj = GetPluginByElementName(s)->CreateElement();
        ChildGraphWin* pChildGraphWin = activeMdiChild();
        if(pChildGraphWin && pStdObj)
            pChildGraphWin->ToolButtonClick(pStdObj);
    }
}


/*
* 工具箱UI设置
*/
void MainWindow::ToolBoxUISetting()
{
    int i = 0;

    connect(ui->tbtnToolSelect, SIGNAL(Click(QString)), this, SLOT(ToolButtonClick(QString)));

    QMapIterator<QString, QMap<QString, IDrawApplicationPlugin*> > it(m_toolPlugins);

    while(it.hasNext())
    {
        it.next();
        //qDebug() << it.key();
        QWidget *pWidget = NULL;
        QVBoxLayout *pWidgetLayout = NULL;

        for(i=0; i<ui->toolBox->count(); i++)
        {
            if(ui->toolBox->itemText(i) == it.key())
            {
                pWidget = ui->toolBox->widget(i);
                pWidgetLayout = (QVBoxLayout*)pWidget->layout();
            }
        }

        if(!pWidget)
        {
            pWidget = new QWidget();
            pWidget->setObjectName(QStringLiteral("pWidget"));
            pWidget->setGeometry(QRect(0, 0, 160, 527));
            pWidgetLayout = new QVBoxLayout(pWidget);
            pWidgetLayout->setSpacing(6);
            pWidgetLayout->setContentsMargins(11, 11, 11, 11);
            pWidgetLayout->setObjectName(QStringLiteral("pWidgetLayout"));
        }

        QMapIterator<QString, IDrawApplicationPlugin*>  t(it.value());
        while(t.hasNext())
        {
            t.next();
            //qDebug() << t.key();
            IDrawApplicationPlugin* pPlugin = t.value();
            ToolButton *pTbtnToolSelect = new ToolButton(pWidget);
            pTbtnToolSelect->setObjectName(QStringLiteral("pTbtnToolSelect"));
            pTbtnToolSelect->setIcon(pPlugin->GetElementIcon());
            pTbtnToolSelect->setText(pPlugin->GetElementName());
            pTbtnToolSelect->setIconSize(QSize(32, 32));
            pTbtnToolSelect->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
            pTbtnToolSelect->setAutoRaise(true);
            connect(pTbtnToolSelect, SIGNAL(Click(QString)), this, SLOT(ToolButtonClick(QString)));
            pWidgetLayout->addWidget(pTbtnToolSelect);
        }

        pWidgetLayout->addStretch(); // 最后一个控件之后添加伸缩

        if(!pWidget)
            ui->toolBox->addItem(pWidget, it.key());
    }

}



/*
* 图形列表双击事件
*/
void MainWindow::on_treeViewGraph_doubleClicked(const QModelIndex &index)
{
    QStandardItem *item = pTreeViewGraphModel->itemFromIndex(index);
    QString winTittle = item->text();

    ChildGraphWin* pChildGraphWin = findMdiChild(winTittle);

    if(pChildGraphWin)
    {
        pChildGraphWin->showMaximized();
    }
    else
    {
        QString file = m_ProjectPath + "/" + winTittle + ".drw";
        doOpenDraw(file);
    }
}



void MainWindow::LoadPlugin()
{
    ReleasePlugin();

    QDir pluginsDir(ConfigUtils::AppDir());
    pluginsDir.cdUp();
    pluginsDir.cd(QLatin1String("DrawApplicationPlugins"));
    foreach (QString fileName, pluginsDir.entryList(QDir::Files))
    {
        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
        QObject *pluginObj = pluginLoader.instance();
        if (pluginObj)
        {
            IDrawApplicationPlugin *plugin= qobject_cast<IDrawApplicationPlugin *>(pluginObj);
            if (plugin)
            {
                QString type = plugin->GetPluginType();
                QString name = plugin->GetPluginName();

                if(type != QLatin1String("") && name != QLatin1String(""))
                {
                    QMap<QString, IDrawApplicationPlugin*> plugins = m_toolPlugins.value(type);
                    IDrawApplicationPlugin *p = plugins.value(name);

                    if(p != NULL)
                    {
                        delete p;
                    }
                    plugins.insert(name, plugin);
                    m_toolPlugins.insert(type, plugins);
                    plugin->Initialize();
                }
                else
                {
                    delete plugin;
                }
            }
        }
    }
}

QMap<QString, IDrawApplicationPlugin*> MainWindow::GetPluginByType(const QString &class_type)
{
    return m_toolPlugins.value(class_type);
}

IDrawApplicationPlugin* MainWindow::GetPluginByElementName(const QString &element_name)
{
    QMapIterator<QString, QMap<QString, IDrawApplicationPlugin*> > it(m_toolPlugins);

    while(it.hasNext())
    {
        it.next();
        QMapIterator<QString, IDrawApplicationPlugin*>  t(it.value());
        while(t.hasNext())
        {
            t.next();
            if(t.key() == element_name)
                return t.value();

        }
    }
    return NULL;
}


IDrawApplicationPlugin* MainWindow::GetPlugin(const QString &class_type, const QString &element_name)
{
    return m_toolPlugins.value(class_type).value(element_name);
}


void MainWindow::ReleasePlugin()
{
    QMapIterator<QString, QMap<QString, IDrawApplicationPlugin*> > it(m_toolPlugins);

    while(it.hasNext())
    {
        it.next();
        QMapIterator<QString, IDrawApplicationPlugin*>  t(it.value());
        while(t.hasNext())
        {
            t.next();
            delete t.value();
        }
    }
    m_toolPlugins.clear();
}

/*
 * 设置状态栏信息
 */
void MainWindow::setStatusBarMessage(const QString &msg)
{
    ui->statusBar->showMessage(msg);
}


