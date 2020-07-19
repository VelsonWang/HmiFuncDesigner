#include "DrawMainWindow.h"
#include "Helper.h"
#include "ProjectData.h"
#include "DrawListUtils.h"
#include "ProjectInfoManager.h"
#include "ProjectData.h"
#include "qtvariantproperty.h"
#include "qttreepropertybrowser.h"
#include "variantmanager.h"
#include "variantfactory.h"
#include <QDesktopWidget>
#include <QApplication>
#include <QFileInfo>
#include <QRect>
#include <QGraphicsView>
#include <QFileDialog>
#include <QScrollArea>
#include <QToolBar>
#include <QInputDialog>
#include <QDebug>


DrawMainWindow::DrawMainWindow(const QString &szProjPath,
                       const QString &szProjName,
                       const QString &szGraphPageName,
                       QWidget *parent) :
    QMainWindow(parent),
    currentGraphPage_(Q_NULLPTR),
    currentView_(Q_NULLPTR),
    gridVisible_(true),
    currentGraphPageIndex_(0),
    szProjPath_(szProjPath),
    szProjName_(szProjName),
    graphPageName_(szGraphPageName)
{
    setupUi(this);

    undoGroup_ = new QUndoGroup(this);
    createActions();
    createMenus();
    createToolbars();
    initView();
    slotUpdateActions();

    setWindowState(Qt::WindowMaximized);
    setWindowTitle(tr("画面编辑器"));
    setWindowIcon(QIcon(":/DrawAppImages/application.png"));

    connect(graphPageTabWidget_, SIGNAL(currentChanged(int)), SLOT(slotChangeGraphPage(int)));

    QDesktopWidget * pDesktopWidget = QApplication::desktop();
    QRect rect = pDesktopWidget->screenGeometry();
    int screenWidth = rect.width();
    int screenHeight = rect.height();
    this->resize(screenWidth*3/4, screenHeight*3/4);

    Helper::WidgetMoveCenter(this);

    DrawListUtils::setProjectPath(szProjPath_);

    listWidgetGraphPages->setContextMenuPolicy(Qt::DefaultContextMenu);
}


DrawMainWindow::~DrawMainWindow()
{

}


void DrawMainWindow::initView()
{
    graphPageTabWidget_ = new QTabWidget(this);
    graphPageTabWidget_->installEventFilter(this);
    this->scrollArea->setWidget(graphPageTabWidget_);

    elementWidget_ = new ElementLibraryWidget();
    this->ElemetsLayout->addWidget(elementWidget_);

    variantEditorFactory_ = new VariantFactory(this);

    //propertyEditor_ = new QtTreePropertyBrowser(dockProperty);
    propertyEditor_ = new QtTreePropertyBrowser(this);
    propertyEditor_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    propertyEditor_->setHeaderLabels(QStringList() << tr("属性") << tr("值"));
    //propertyEditor_->setColumnWidth(0, 60);
    //propertyEditor_->setColumnWidth(1, 200);


    this->PropertyLayout->addWidget(propertyEditor_);

    VariantManager *pVariantManager  = new VariantManager(this);
    variantPropertyManager_ = pVariantManager;
    pVariantManager->setPropertyEditor(propertyEditor_);
    propertyEditor_->setFactoryForManager(variantPropertyManager_, variantEditorFactory_);

    connect(variantPropertyManager_, SIGNAL(valueChanged(QtProperty *, const QVariant &)),
            this, SLOT(propertyValueChanged(QtProperty *, const QVariant &)));


    objTree_ = new ObjectsTreeView();
    this->ObjectTreeLayout->addWidget(objTree_);
    slotShowTreeObj(false);
}

void DrawMainWindow::createActions()
{
    actionShowGraphObj_ = new QAction(tr("图形元素"), this);
    actionShowGraphObj_->setCheckable(true);
    actionShowGraphObj_->setChecked(true);
    connect(actionShowGraphObj_, SIGNAL(triggered(bool)), SLOT(slotShowGraphObj(bool)));

    actionShowTreeObj_ = new QAction(tr("对象树"), this);
    actionShowTreeObj_->setCheckable(true);
    actionShowTreeObj_->setChecked(false);
    connect(actionShowTreeObj_, SIGNAL(triggered(bool)), SLOT(slotShowTreeObj(bool)));

    actionShowPropEditor_ = new QAction(tr("属性编辑器"), this);
    actionShowPropEditor_->setCheckable(true);
    actionShowPropEditor_->setChecked(true);
    connect(actionShowPropEditor_, SIGNAL(triggered(bool)), SLOT(slotShowPropEditor(bool)));

    actionNew = new QAction(QIcon(":/DrawAppImages/filenew.png"), tr("新建"), this);
    actionNew->setShortcut(QString("Ctrl+N"));
    connect(actionNew, SIGNAL(triggered()), SLOT(slotEditNew()));

    actionOpen = new QAction(QIcon(":/DrawAppImages/fileopen.png"), tr("打开"), this);
    actionOpen->setShortcut(QString("Ctrl+O"));
    connect(actionOpen, SIGNAL(triggered()), SLOT(slotEditOpen()));

    actionSaveGraphPage_ = new QAction(QIcon(":/DrawAppImages/saveproject.png"), tr("保存"), this);
    actionSaveGraphPage_->setShortcut(QKeySequence::Save);
    connect(actionSaveGraphPage_, SIGNAL(triggered()), SLOT(slotSaveGraphPage()));

    actionCloseGraphPage_ = new QAction(tr("关闭"), this);
    connect(actionCloseGraphPage_, SIGNAL(triggered()), SLOT(slotCloseGraphPage()));

    actionCloseAll_ = new QAction(tr("关闭所有"), this);
    connect(actionCloseAll_, SIGNAL(triggered()), SLOT(slotCloseAll()));

    actionExit_ = new QAction(tr("退出"),this);
    actionExit_->setShortcut(QKeySequence::Quit);
    connect(actionExit_,SIGNAL(triggered()),SLOT(slotExit()));

    actionShowGrid = new QAction(QIcon(":/DrawAppImages/showgrid.png"), tr("显示栅格"), this);
    actionShowGrid->setCheckable(true);
    actionShowGrid->setChecked(gridVisible_);
    connect(actionShowGrid, SIGNAL(triggered(bool)), SLOT(slotShowGrid(bool)));

    actionShowLinear_ = new QAction(QIcon(":/DrawAppImages/ruler.png"), tr("显示线条"), this);
    actionShowLinear_->setCheckable(true);
    actionShowLinear_->setChecked(false);
    connect(actionShowLinear_, SIGNAL(triggered(bool)), SLOT(slotShowLinear(bool)));

    actionZoomIn_ = new QAction(QIcon(":/DrawAppImages/zoom-in.png"), tr("放大"), this);
    connect(actionZoomIn_, SIGNAL(triggered()), SLOT(slotZoomIn()));

    actionZoomOut_ = new QAction(QIcon(":/DrawAppImages/zoom-out.png"), tr("缩小"), this);
    connect(actionZoomOut_, SIGNAL(triggered()), SLOT(slotZoomOut()));

    actionUndo_ = undoGroup_->createUndoAction(this);
    actionUndo_->setIcon(QIcon(":/DrawAppImages/undo.png"));
    actionUndo_->setText(tr("撤销"));
    actionUndo_->setShortcut(QKeySequence::Undo);

    actionRedo_ = undoGroup_->createRedoAction(this);
    actionRedo_->setText(tr("重做"));
    actionRedo_->setIcon(QIcon(":/DrawAppImages/redo.png"));
    actionRedo_->setShortcut(QKeySequence::Redo);

    actionDelete_ = new QAction(QIcon(":/DrawAppImages/delete.png"), tr("删除"));
    actionDelete_->setShortcut(QKeySequence::Delete);
    connect(actionDelete_, SIGNAL(triggered()), SLOT(slotEditDelete()));

    actionCopy_ = new QAction(QIcon(":/DrawAppImages/editcopy.png"),tr("拷贝"));
    actionCopy_->setShortcut(QKeySequence::Copy);
    connect(actionCopy_, SIGNAL(triggered()), SLOT(slotEditCopy()));

    actionPaste_ = new QAction(QIcon(":/DrawAppImages/editpaste.png"),tr("粘贴"));
    actionPaste_->setShortcut(QKeySequence::Paste);
    connect(actionPaste_, SIGNAL(triggered()), SLOT(slotEditPaste()));

    // 顶部对齐
    alignTopAction_ = new QAction(QIcon(":/DrawAppImages/align-top.png"), tr("顶部对齐"));
    alignTopAction_->setData(Qt::AlignTop);
    connect(alignTopAction_, SIGNAL(triggered()), SLOT(slotAlignElements()));

    // 底部对齐
    alignDownAction_ = new QAction(QIcon(":/DrawAppImages/align-bottom.png"), tr("底部对齐"));
    alignDownAction_->setData(Qt::AlignBottom);
    connect(alignDownAction_, SIGNAL(triggered()), SLOT(slotAlignElements()));

    // 右对齐
    alignRightAction_ = new QAction(QIcon(":/DrawAppImages/align-right.png"), tr("右对齐"));
    alignRightAction_->setData(Qt::AlignRight);
    connect(alignRightAction_, SIGNAL(triggered()), SLOT(slotAlignElements()));

    // 左对齐
    alignLeftAction_ = new QAction(QIcon(":/DrawAppImages/align-left.png"), tr("左对齐"));
    alignLeftAction_->setData(Qt::AlignLeft);
    connect(alignLeftAction_, SIGNAL(triggered()), SLOT(slotAlignElements()));

    // 水平均匀分布
    hUniformDistributeAction_ = new QAction(QIcon(":/DrawAppImages/align_hsame.png"), tr("水平均匀分布"));
    connect(hUniformDistributeAction_, SIGNAL(triggered()), SLOT(slotHUniformDistributeElements()));

    // 垂直均匀分布
    vUniformDistributeAction_ = new QAction(QIcon(":/DrawAppImages/align_vsame.png"), tr("垂直均匀分布"));
    connect(vUniformDistributeAction_, SIGNAL(triggered()), SLOT(slotVUniformDistributeElements()));

    // 设置选中控件大小一致
    setTheSameSizeAction_ = new QAction(QIcon(":/DrawAppImages/the-same-size.png"), tr("大小一致"));
    connect(setTheSameSizeAction_, SIGNAL(triggered()), SLOT(slotSetTheSameSizeElements()));

    // 上移一层
    upLayerAction_ = new QAction(QIcon(":/DrawAppImages/posfront.png"), tr("上移一层"));
    connect(upLayerAction_, SIGNAL(triggered()), SLOT(slotUpLayerElements()));

    // 下移一层
    downLayerAction_ = new QAction(QIcon(":/DrawAppImages/posback.png"), tr("下移一层"));
    connect(downLayerAction_, SIGNAL(triggered()), SLOT(slotDownLayerElements()));

}

void DrawMainWindow::createMenus()
{
    QMenu *filemenu = new QMenu(tr("文件"), this);
#if 0  // for test we need this
    filemenu->addAction(actionNew);
    filemenu->addAction(actionOpen);
#endif
    filemenu->addAction(actionSaveGraphPage_);
    filemenu->addSeparator();
    filemenu->addAction(actionCloseGraphPage_);
    filemenu->addAction(actionCloseAll_);
    filemenu->addSeparator();
    filemenu->addAction(actionExit_);

    QMenu *windowMenu = new QMenu(tr("窗口"), this);
    windowMenu->addAction(actionShowGraphObj_);
    windowMenu->addAction(actionShowTreeObj_);
    windowMenu->addAction(actionShowPropEditor_);

    QMainWindow::menuBar()->addMenu(filemenu);
    QMainWindow::menuBar()->addMenu(windowMenu);
}

void DrawMainWindow::createToolbars()
{
    toolBar->addAction(actionSaveGraphPage_);
    toolBar->addSeparator();
    toolBar->addAction(actionShowGrid);
    //toolBar->addAction(actionShowLinear);
    toolBar->addAction(actionZoomOut_);
    toolBar->addAction(actionZoomIn_);
    toolBar->addSeparator();
    toolBar->addAction(actionUndo_);
    toolBar->addAction(actionRedo_);
    toolBar->addSeparator();
    toolBar->addAction(actionCopy_); // 拷贝
    toolBar->addAction(actionPaste_); // 粘贴
    toolBar->addAction(actionDelete_); // 删除
    toolBar->addSeparator();
    toolBar->addAction(alignTopAction_); // 顶部对齐
    toolBar->addAction(alignDownAction_); // 底部对齐
    toolBar->addAction(alignLeftAction_); // 左对齐
    toolBar->addAction(alignRightAction_); // 右对齐
    toolBar->addAction(hUniformDistributeAction_); // 水平均匀分布
    toolBar->addAction(vUniformDistributeAction_); // 垂直均匀分布
    toolBar->addAction(setTheSameSizeAction_); // 设置选中控件大小一致
    toolBar->addAction(upLayerAction_); // 上移一层
    toolBar->addAction(downLayerAction_); // 下移一层
    toolBar->addSeparator();
}

void DrawMainWindow::closeEvent(QCloseEvent *event)
{
    bool unsaved = false;

    QListIterator<GraphPage*> it(GraphPageManager::getInstance()->getGraphPageList());

    while (it.hasNext()) {
        GraphPage *graphPage = it.next();
        if (!graphPage->undoStack()->isClean() || graphPage->getUnsavedFlag()) {
            unsaved = true;
        }
    }

    if (unsaved) {
        int ret = exitResponse();

        if (ret == QMessageBox::Yes) {
            slotSaveGraphPage();
            event->accept();
        } else if (ret == QMessageBox::No) {
            event->accept();
        }
    } else {
        event->accept();
    }
}


/**
 * @brief openGraphPage
 * @details 打开画面
 * @param pagePath 画面路径
 * @param pagePath 画面名称
 */
void DrawMainWindow::openGraphPage(const QString &szProjPath,
                               const QString &szProjName,
                               const QString &szPageName)
{
    DrawListUtils::loadDrawList(szProjPath);
    foreach(QString szPageId, DrawListUtils::drawList_) {
        listWidgetGraphPages->addItem(szPageId);
        QString fileName = szProjPath + "/" + szPageId + ".drw";

        if (fileName.toLower().endsWith(".drw")) {
            QGraphicsView *view = createTabView();

            if (graphPageTabWidget_->indexOf(view) != -1) {
                delete view;
                return;
            }

            GraphPage *graphPage = new GraphPage(QRectF(), variantPropertyManager_, propertyEditor_);
            if (!createDocument(graphPage, view, fileName)) {
                return;
            }

            if(szPageId == szPageName) {
                currentGraphPage_ = graphPage;
                currentView_ = dynamic_cast<QGraphicsView *>(view);
            }

            graphPage->setProjectPath(szProjPath);
            graphPage->setProjectName(szProjName);
            graphPage->setGridVisible(gridVisible_);
            graphPage->loadAsXML(fileName);
            view->setFixedSize(graphPage->getGraphPageWidth(), graphPage->getGraphPageHeight());
            graphPage->setFileName(szPageId + ".drw");
            graphPage->setGraphPageId(szPageId);
            graphPage->fillGraphPagePropertyModel();
        }
    }

    QList<QListWidgetItem*> listWidgetItem = listWidgetGraphPages->findItems(szPageName, Qt::MatchCaseSensitive);
    if ( listWidgetItem.size() > 0 ) {
        listWidgetGraphPages->setCurrentItem(listWidgetItem.at(0));
    }

}


void DrawMainWindow::slotEditNew()
{
    addNewGraphPage();
}

QString DrawMainWindow::fixedWindowTitle(const QGraphicsView *viewGraphPage) const
{
    QString title = currentGraphPage_->getGraphPageId();

    if (title.isEmpty()) {
        title = "Untitled";
    } else {
        title = QFileInfo(title).fileName();
    }

    QString result;

    for (int i = 0; ;++i) {
        result = title;

        if (i > 0) {
            result += QString::number(i);
        }

        bool unique = true;

        for (int j = 0; j < graphPageTabWidget_->count(); ++j) {
            const QWidget *widget = graphPageTabWidget_->widget(j);

            if (widget == viewGraphPage) {
                continue;
            }

            if (result == graphPageTabWidget_->tabText(j)) {
                unique = false;
                break;
            }
        }

        if (unique) {
            break;
        }
    }

    return result;
}

bool DrawMainWindow::isGraphPageOpen(const QString &filename)
{
    QListIterator <GraphPage*> it(GraphPageManager::getInstance()->getGraphPageList());

    while (it.hasNext()) {
        if (filename == it.next()->getFileName()) {
            return true;
        }
    }

    return false;
}

void DrawMainWindow::addNewGraphPage()
{
    QGraphicsView *view = createTabView();

    if (graphPageTabWidget_->indexOf(view) != -1) {
        delete view;
        return;
    }

    GraphPage *graphPage = new GraphPage(QRectF(), variantPropertyManager_, propertyEditor_);
    graphPage->setProjectPath(szProjPath_);
    graphPage->setProjectName(szProjName_);
    graphPage->setGridVisible(gridVisible_);
    currentGraphPage_ = graphPage;
    view->setScene(graphPage);
    view->setFixedSize(graphPage->getGraphPageWidth(), graphPage->getGraphPageHeight());
    currentView_ = dynamic_cast<QGraphicsView *>(view);
    QString title = fixedWindowTitle(view);
    graphPage->setFileName(title + ".drw");
    graphPage->setGraphPageId(title);
    graphPageTabWidget_->addTab(currentView_, title);
    graphPageTabWidget_->setCurrentWidget(currentView_);
    GraphPageManager::getInstance()->addGraphPage(graphPage);

    undoGroup_->addStack(graphPage->undoStack());
    undoGroup_->setActiveStack(graphPage->undoStack());

    connectGraphPage(graphPage);
}

void DrawMainWindow::connectGraphPage(GraphPage *graphPage)
{
    connect(graphPage->undoStack(), SIGNAL(indexChanged(int)), SLOT(slotUpdateActions()));
    connect(graphPage->undoStack(), SIGNAL(cleanChanged(bool)), SLOT(slotUpdateActions()));
    connect(graphPage, SIGNAL(newElementAdded()), SLOT(slotNewElementAdded()));
    connect(graphPage, SIGNAL(elementsDeleted()), SLOT(slotElementsDeleted()));
    connect(graphPage, SIGNAL(elementIdChanged()), SLOT(slotElementIdChanged()));
    connect(graphPage, SIGNAL(changeGraphPageName()), SLOT(slotChangeGraphPageName()));
    connect(graphPage, SIGNAL(selectionChanged()), SLOT(slotUpdateActions()));
    connect(graphPage, SIGNAL(elementPropertyChanged()), SLOT(slotUpdateActions()));
    connect(graphPage, SIGNAL(GraphPagePropertyChanged()), SLOT(slotUpdateActions()));
    connect(graphPage, SIGNAL(GraphPageSaved()), SLOT(slotUpdateActions()));
}

void DrawMainWindow::disconnectGraphPage(GraphPage *graphPage)
{
    disconnect(graphPage->undoStack(), SIGNAL(indexChanged(int)), this, SLOT(slotUpdateActions()));
    disconnect(graphPage->undoStack(), SIGNAL(cleanChanged(bool)), this, SLOT(slotUpdateActions()));
    disconnect(graphPage, SIGNAL(newElementAdded()), this, SLOT(slotNewElementAdded()));
    disconnect(graphPage, SIGNAL(elementsDeleted()), this, SLOT(slotElementsDeleted()));
    disconnect(graphPage, SIGNAL(elementIdChanged()), this, SLOT(slotElementIdChanged()));
    disconnect(graphPage, SIGNAL(changeGraphPageName()), this, SLOT(slotChangeGraphPageName()));
    disconnect(graphPage, SIGNAL(selectionChanged()), this, SLOT(slotUpdateActions()));
    disconnect(graphPage, SIGNAL(elementPropertyChanged()), this, SLOT(slotUpdateActions()));
    disconnect(graphPage, SIGNAL(GraphPagePropertyChanged()), this, SLOT(slotUpdateActions()));
    disconnect(graphPage, SIGNAL(GraphPageSaved()), this, SLOT(slotUpdateActions()));
}

QGraphicsView *DrawMainWindow::createTabView()
{
    QGraphicsView *view = new QGraphicsView();
    view->setDragMode(QGraphicsView::RubberBandDrag);
    view->setCacheMode(QGraphicsView::CacheBackground);
    view->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    return view;
}

void DrawMainWindow::slotUpdateActions()
{
    static const QIcon unsaved(":/DrawAppImages/filesave.png");

    for (int i = 0; i < graphPageTabWidget_->count(); i++) {
        QGraphicsView *view = dynamic_cast<QGraphicsView*>(graphPageTabWidget_->widget(i));
        if (!dynamic_cast<GraphPage *>(view->scene())->undoStack()->isClean() ||
            dynamic_cast<GraphPage *>(view->scene())->getUnsavedFlag()) {
            graphPageTabWidget_->setTabIcon(graphPageTabWidget_->indexOf(view), unsaved);
        } else {
            graphPageTabWidget_->setTabIcon(graphPageTabWidget_->indexOf(view), QIcon());
        }
    }

    actionZoomIn_->setEnabled(graphPageTabWidget_->count() ? true : false);
    actionZoomOut_->setEnabled(graphPageTabWidget_->count() ? true : false);
    actionShowGrid->setEnabled(graphPageTabWidget_->count() ? true : false);

    if (!currentGraphPage_) {
        return;
    }

    undoGroup_->setActiveStack(currentGraphPage_->undoStack());

    if (!currentGraphPage_->undoStack()->isClean() || currentGraphPage_->getUnsavedFlag()) {
        actionSaveGraphPage_->setEnabled(true);
    } else {
        actionSaveGraphPage_->setEnabled(false);
    }
}

void DrawMainWindow::slotChangeGraphPage(int GraphPageNum)
{
    if (GraphPageNum == -1) {
        objTree_->clearModel();
        return;
    }

    listWidgetGraphPages->setCurrentRow(GraphPageNum);

    for (int i = 0; i < graphPageTabWidget_->count(); i++) {
        QGraphicsView *view = dynamic_cast<QGraphicsView *>(graphPageTabWidget_->widget(i));
        dynamic_cast<GraphPage *>(view->scene())->setActive(false);
    }

    currentView_ = dynamic_cast<QGraphicsView *>(graphPageTabWidget_->widget(GraphPageNum));
    currentGraphPage_ = dynamic_cast<GraphPage *>(currentView_->scene());
    currentGraphPage_->setActive(true);
    //currentGraphPage_->fillGraphPagePropertyModel();
    objTree_->setContentList(currentGraphPage_->items());
    objTree_->updateContent();
    currentGraphPageIndex_ = GraphPageNum;
    slotUpdateActions();
}

void DrawMainWindow::slotChangeGraphPageName()
{
    graphPageTabWidget_->setTabText(currentGraphPageIndex_, currentGraphPage_->getGraphPageId());
    //int index = GraphPageManager::getInstance()->getIndexByGraphPage(currentGraphPage_);
}

void DrawMainWindow::updateObjectTree()
{
    objTree_->setContentList(currentGraphPage_->items());
    objTree_->updateContent();
}

void DrawMainWindow::slotElementIdChanged()
{
    updateObjectTree();
}

void DrawMainWindow::slotElementPropertyChanged()
{

}

void DrawMainWindow::slotGraphPagePropertyChanged()
{

}

void DrawMainWindow::propertyValueChanged(QtProperty *property, const QVariant &value)
{
    Q_UNUSED(property)
    Q_UNUSED(value)
}

void DrawMainWindow::slotNewElementAdded()
{
    updateObjectTree();
}

void DrawMainWindow::slotElementsDeleted()
{
    updateObjectTree();
}

void DrawMainWindow::slotShowGrid(bool on)
{
    QListIterator <GraphPage*> iter(GraphPageManager::getInstance()->getGraphPageList());

    while (iter.hasNext()) {
        iter.next()->setGridVisible(on);
    }

    gridVisible_ = on;
}

void DrawMainWindow::slotShowGraphObj(bool on)
{
    on ? this->dockElemets->show() : this->dockElemets->hide();
}

void DrawMainWindow::slotShowTreeObj(bool on)
{
    on ? this->dockObjectTree->show() : this->dockObjectTree->hide();
}

void DrawMainWindow::slotShowPropEditor(bool on)
{
    on ? this->dockProperty->show() : this->dockProperty->hide();
}


void DrawMainWindow::slotCloseAll()
{
    while (graphPageTabWidget_->count()) {
        QGraphicsView *view = static_cast<QGraphicsView*>(graphPageTabWidget_->widget(graphPageTabWidget_->currentIndex()));
        removeGraphPage(view);
        delete view;
    }

    currentView_ = Q_NULLPTR;
    currentGraphPage_ = Q_NULLPTR;
    slotUpdateActions();
}

void DrawMainWindow::removeGraphPage(QGraphicsView *view)
{
    int index = graphPageTabWidget_->indexOf(view);
    GraphPage *graphPage = static_cast<GraphPage*>(view->scene());

    if (index == -1)
        return;

    if (!graphPage->undoStack()->isClean()) {
        int ret = exitResponse();

        if (ret == QMessageBox::Yes) {
            slotSaveGraphPage();
        }
    }

    graphPageTabWidget_->removeTab(index);
    undoGroup_->removeStack(graphPage->undoStack());
    GraphPageManager::getInstance()->removeGraphPage(graphPage);
    disconnectGraphPage(graphPage);
    delete graphPage;
}

void DrawMainWindow::slotCloseGraphPage()
{
    QGraphicsView *view = currentView_;
    removeGraphPage(view);
    delete view;

    if (graphPageTabWidget_->count() == 0) {
        currentGraphPage_ = Q_NULLPTR;
        currentView_ = Q_NULLPTR;
    }

    slotUpdateActions();
}

void DrawMainWindow::slotEditOpen()
{
    const QString &filename = QFileDialog::getOpenFileName(this,
                                                           tr("Open"),
                                                           ".",
                                                           tr("GraphPage (*.drw)"));
    if (filename.isEmpty())
        return;
#if 0
    if (filename.toLower().endsWith(".drwb")) {

        QGraphicsView *view = createTabView();

        if (graphPageTabWidget_->indexOf(view) != -1) {
            delete view;
            return;
        }

        GraphPage *graphPage = new GraphPage(QRectF());
        if (!createDocument(graphPage,view,filename)) {
            return;
        }
        graphPage->loadAsBinary(filename);
    }
#endif
    if (filename.toLower().endsWith(".drw")) {

        QGraphicsView *view = createTabView();

        if (graphPageTabWidget_->indexOf(view) != -1) {
            delete view;
            return;
        }

        GraphPage *graphPage = new GraphPage(QRectF(), variantPropertyManager_, propertyEditor_);
        if (!createDocument(graphPage, view, filename)) {
            return;
        } 

        currentGraphPage_ = graphPage;
        currentView_ = dynamic_cast<QGraphicsView *>(view);
        graphPage->setProjectPath(szProjPath_);
        graphPage->setProjectName(szProjName_);
        graphPage->loadAsXML(filename);
        int pos = filename.lastIndexOf("/");
        QString pageFileName = "";
        if (pos != -1) {
            pageFileName = filename.right(filename.length() - pos - 1);
        }
        graphPage->setFileName(pageFileName);
        graphPage->setGraphPageId(pageFileName.left(pageFileName.length() - 4));
    }
}

bool DrawMainWindow::createDocument(GraphPage *graphPage,
                                QGraphicsView *view,
                                const QString &filename)
{
    if (isGraphPageOpen(filename)) {
        QMessageBox::information(this,
                                 tr("打开文件错误"),
                                 tr("文件已打开"),
                                 QMessageBox::Ok);
        delete graphPage;
        delete view;
        return false;
    }

    graphPage->setGridVisible(gridVisible_);
    view->setScene(graphPage);
    view->setFixedSize(graphPage->getGraphPageWidth(), graphPage->getGraphPageHeight());
    graphPageTabWidget_->addTab(view, graphPage->getGraphPageId());
    graphPageTabWidget_->setCurrentWidget(view);
    GraphPageManager::getInstance()->addGraphPage(graphPage);

    undoGroup_->addStack(graphPage->undoStack());
    undoGroup_->setActiveStack(graphPage->undoStack());

    connectGraphPage(graphPage);

    graphPage->undoStack()->setClean();

    return true;
}


QString DrawMainWindow::getFileName()
{
    QString filename = QFileDialog::getSaveFileName(this,
                                                    tr("Save as"),
                                                    QString("./%1").arg(currentGraphPage_->getGraphPageId()),
                                                    tr("GraphPage(*.drw)"));
    return filename;
}

void DrawMainWindow::updateGraphPageViewInfo(const QString &fileName)
{
    int index = graphPageTabWidget_->indexOf(currentView_);
    QFileInfo file(fileName);
    currentGraphPage_->setGraphPageId(file.baseName());
    graphPageTabWidget_->setTabText(index,file.baseName());
    slotChangeGraphPageName();
}

void DrawMainWindow::slotSaveGraphPage()
{
    if (!currentGraphPage_) {
        return;
    }

    for (;;) {
        QString fileName = currentGraphPage_->getFileName();

        if (fileName.isEmpty())
            fileName = getFileName();

        if (fileName.isEmpty())
            break;

        currentGraphPage_->setFileName(fileName);
        updateGraphPageViewInfo(fileName);
        currentGraphPage_->saveAsXML(szProjPath_ + "/" + fileName);
#if 0
        if (fileName.toLower().endsWith(".drw")) {
            QString binaryFileName = fileName.toLower()+ "b"; // ".drw"==>".drwb"
            currentGraphPage->saveAsBinary(szProjPath_ + "/" + binaryFileName);
        }
#endif
        break;

    }
}

void DrawMainWindow::slotExit()
{
    if (graphPageTabWidget_->count() == 0) {
        QApplication::quit();
        return;
    }

    bool unsaved = false;

    QListIterator<GraphPage*> it(GraphPageManager::getInstance()->getGraphPageList());

    while (it.hasNext()) {
        if (!it.next()->undoStack()->isClean()) {
            unsaved = true;
        }
    }

    if (unsaved) {
        int ret = exitResponse();
        if (ret == QMessageBox::Yes) {
            slotSaveGraphPage();
            QApplication::quit();
        } else if (ret == QMessageBox::No) {
            QApplication::quit();
        }
    }

    QApplication::quit();
}

int DrawMainWindow::exitResponse()
{
    int ret = QMessageBox::information(this,
                                       tr("退出程序"),
                                       tr("文件已修改。是否保存?"),
                                       QMessageBox::Yes | QMessageBox::No);
    return ret;
}

void DrawMainWindow::slotShowLinear(bool on) {
    Q_UNUSED(on)
}

void DrawMainWindow::slotZoomIn()
{
    if(currentGraphPage_ != nullptr) {
        int width = currentGraphPage_->getGraphPageWidth();
        int height = currentGraphPage_->getGraphPageHeight();
        currentGraphPage_->setGraphPageWidth(static_cast<int>(width * 1.25));
        currentGraphPage_->setGraphPageHeight(static_cast<int>(height * 1.25));
        currentGraphPage_->setGridVisible(currentGraphPage_->isGridVisible());
    }
    if (currentView_ != nullptr) {
        currentView_->scale(1.25, 1.25);
        currentView_->setFixedSize(currentGraphPage_->getGraphPageWidth(), currentGraphPage_->getGraphPageHeight());
    }
}

void DrawMainWindow::slotZoomOut()
{
    if(currentGraphPage_ != nullptr) {
        int width = currentGraphPage_->getGraphPageWidth();
        int height = currentGraphPage_->getGraphPageHeight();
        currentGraphPage_->setGraphPageWidth(static_cast<int>(width * 1/1.25));
        currentGraphPage_->setGraphPageHeight(static_cast<int>(height * 1/1.25));
        currentGraphPage_->setGridVisible(currentGraphPage_->isGridVisible());
    }
    if (currentView_ != nullptr) {
        currentView_->scale(1/1.25, 1/1.25);
        currentView_->setFixedSize(currentGraphPage_->getGraphPageWidth(), currentGraphPage_->getGraphPageHeight());
    }
}


/**
 * @brief DrawMainWindow::slotAlignElements
 * @details 顶部对齐, 底部对齐, 右对齐, 左对齐
 */
void DrawMainWindow::slotAlignElements()
{
    QAction *action = qobject_cast<QAction*>(sender());
    if (!action)
        return;

    Qt::Alignment alignment = static_cast<Qt::Alignment>(action->data().toInt());
    if(currentGraphPage_ != nullptr) {
        QList<QGraphicsItem*> items = currentGraphPage_->selectedItems();
        currentGraphPage_->onAlignElements(alignment, items);
    }
}


/**
 * @brief DrawMainWindow::slotHUniformDistributeElements
 * @details 水平均匀分布
 */
void DrawMainWindow::slotHUniformDistributeElements()
{
    if(currentGraphPage_ != nullptr) {
        QList<QGraphicsItem*> items = currentGraphPage_->selectedItems();
        currentGraphPage_->onHUniformDistributeElements(items);
    }
}


/**
 * @brief DrawMainWindow::slotVUniformDistributeElements
 * @details 垂直均匀分布
 */
void DrawMainWindow::slotVUniformDistributeElements()
{
    if(currentGraphPage_ != nullptr) {
        QList<QGraphicsItem*> items = currentGraphPage_->selectedItems();
        currentGraphPage_->onVUniformDistributeElements(items);
    }
}


/**
 * @brief DrawMainWindow::slotSetTheSameSizeElements
 * @details 设置选中控件大小一致
 */
void DrawMainWindow::slotSetTheSameSizeElements()
{
    if(currentGraphPage_ != nullptr) {
        QList<QGraphicsItem*> items = currentGraphPage_->selectedItems();
        currentGraphPage_->onSetTheSameSizeElements(items);
    }
}


/**
 * @brief DrawMainWindow::slotUpLayerElements
 * @details 上移一层
 */
void DrawMainWindow::slotUpLayerElements()
{
    if(currentGraphPage_ != nullptr) {
        QList<QGraphicsItem*> items = currentGraphPage_->selectedItems();
        currentGraphPage_->onUpLayerElements(items);
    }
}


/**
 * @brief DrawMainWindow::slotDownLayerElements
 * @details 下移一层
 */
void DrawMainWindow::slotDownLayerElements()
{
    if(currentGraphPage_ != nullptr) {
        QList<QGraphicsItem*> items = currentGraphPage_->selectedItems();
        currentGraphPage_->onDownLayerElements(items);
    }
}


/**
 * @brief DrawMainWindow::slotEditDelete
 * @details 删除
 */
void DrawMainWindow::slotEditDelete()
{
    if(currentGraphPage_ != nullptr) {
        QList<QGraphicsItem*> items = currentGraphPage_->selectedItems();
        currentGraphPage_->onEditDelete(items);
    }
}


/**
 * @brief DrawMainWindow::slotEditCopy
 * @details 拷贝
 */
void DrawMainWindow::slotEditCopy()
{
    if(currentGraphPage_ != nullptr) {
        QList<QGraphicsItem*> items = currentGraphPage_->selectedItems();
        currentGraphPage_->onEditCopy(items);
    }
}


/**
 * @brief DrawMainWindow::slotEditPaste
 * @details 粘贴
 */
void DrawMainWindow::slotEditPaste()
{
    if(currentGraphPage_ != nullptr) {
        QList<QGraphicsItem*> items = currentGraphPage_->selectedItems();
        currentGraphPage_->onEditPaste();
    }
}


/**
 * @brief DrawMainWindow::on_listWidgetGraphPages_itemClicked
 * @details 画面名称被单击
 * @param item
 */
void DrawMainWindow::on_listWidgetGraphPages_currentTextChanged(const QString &currentText)
{
    Q_UNUSED(currentText)
    graphPageTabWidget_->setCurrentIndex(listWidgetGraphPages->currentRow());
}


/**
 * @brief DrawMainWindow::contextMenuEvent
 * @details 右键菜单
 * @param event
 */
void DrawMainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event)

    QMenu *pMenu = new QMenu(this);

    QAction *pNewAct = new QAction(tr("新建"), this);
    pNewAct->setStatusTip(tr("新建画面"));
    connect(pNewAct, SIGNAL(triggered()), this, SLOT(onNewGraphPage()));
    pMenu->addAction(pNewAct);

    QAction *pRenameAct = new QAction(tr("重命名"), this);
    pRenameAct->setStatusTip(tr("重命名画面"));
    connect(pRenameAct, SIGNAL(triggered()), this, SLOT(onRenameGraphPage()));
    pMenu->addAction(pRenameAct);

    QAction *pDeleteAct = new QAction(tr("删除"), this);
    pDeleteAct->setStatusTip(tr("删除画面"));
    connect(pDeleteAct, SIGNAL(triggered()), this, SLOT(onDeleteGraphPage()));
    pMenu->addAction(pDeleteAct);

    QAction *pCopyAct = new QAction(tr("复制"), this);
    pCopyAct->setStatusTip(tr("复制画面"));
    connect(pCopyAct, SIGNAL(triggered()), this, SLOT(onCopyGraphPage()));
    pMenu->addAction(pCopyAct);

    QAction *pPasteAct = new QAction(tr("粘贴"), this);
    pPasteAct->setStatusTip(tr("粘贴画面"));
    connect(pPasteAct, SIGNAL(triggered()), this, SLOT(onPasteGraphPage()));
    pMenu->addAction(pPasteAct);

    pMenu->move(cursor().pos());
    pMenu->show();
}


/**
 * @brief DrawMainWindow::createEmptyGraphpage
 * @details 创建空的画面页
 * @param projPath 工程路径
 * @param graphPageName 画面名称
 * @param width 画面宽度
 * @param height 画面高度
 */
void DrawMainWindow::createEmptyGraphpage(const QString &projPath,
                                      const QString &graphPageName,
                                      int width,
                                      int height)
{
    QString fileName = projPath + "/" + graphPageName + ".drw";
    QString szContent = QString(
                "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                "<graphPage fileName=\"%1.drw\" graphPageId=\"%1\" "
                "width=\"%2\" height=\"%3\" background=\"#ffffff\">\n"
                "</graphPage>")
            .arg(graphPageName)
            .arg(QString::number(width))
            .arg(QString::number(height));

    Helper::writeString(fileName, szContent);
}


/**
 * @brief DrawMainWindow::onNewGraphPage
 * @details 新建画面
 */
void DrawMainWindow::onNewGraphPage()
{
    int last = DrawListUtils::getMaxDrawPageNum("draw");
    QString szGraphPageName = QString("draw%1").arg(last);

    QInputDialog dlg(this);
    dlg.setWindowTitle(tr("画面名称"));
    dlg.setLabelText(tr("请输入画面名称"));
    dlg.setOkButtonText(tr("确定"));
    dlg.setCancelButtonText(tr("取消"));
    dlg.setTextValue(szGraphPageName);

reInput:
    if ( dlg.exec() == QDialog::Accepted ) {
        szGraphPageName = dlg.textValue();
        if ( szGraphPageName == "" ) {
            goto reInput;
        }

        QList<GraphPage*> listGraphPage = GraphPageManager::getInstance()->getGraphPageList();

        int width = 480;
        int height = 272;
        if ( listGraphPage.size() > 0 ) {
            GraphPage* pGraphPage = listGraphPage.at(0);
            width = pGraphPage->getGraphPageWidth();
            height = pGraphPage->getGraphPageHeight();
        }

        createEmptyGraphpage(szProjPath_, szGraphPageName, width, height);
        DrawListUtils::drawList_.append(szGraphPageName);
        DrawListUtils::saveDrawList(szProjPath_);

        listWidgetGraphPages->addItem(szGraphPageName);
        QString fileName = szProjPath_ + "/" + szGraphPageName + ".drw";

        if (fileName.toLower().endsWith(".drw")) {
            QGraphicsView *view = createTabView();

            if (graphPageTabWidget_->indexOf(view) != -1) {
                delete view;
                return;
            }

            GraphPage *graphPage = new GraphPage(QRectF(), variantPropertyManager_, propertyEditor_);
            if (!createDocument(graphPage, view, fileName)) {
                return;
            }

            currentGraphPage_ = graphPage;
            currentView_ = dynamic_cast<QGraphicsView *>(view);
            graphPage->setProjectPath(szProjPath_);
            graphPage->setProjectName(szProjName_);
            graphPage->loadAsXML(fileName);
            view->setFixedSize(graphPage->getGraphPageWidth(), graphPage->getGraphPageHeight());
            graphPage->setFileName(szGraphPageName + ".drw");
            graphPage->setGraphPageId(szGraphPageName);
        }

        QList<QListWidgetItem*> listWidgetItem = listWidgetGraphPages->findItems(szGraphPageName, Qt::MatchCaseSensitive);
        if ( listWidgetItem.size() > 0 ) {
            listWidgetGraphPages->setCurrentItem(listWidgetItem.at(0));
            graphPageTabWidget_->setCurrentIndex(listWidgetGraphPages->currentRow());
        }
    }
}


/**
 * @brief DrawMainWindow::onRenameGraphPage
 * @details 修改画面名称
 */
void DrawMainWindow::onRenameGraphPage()
{
    QString szOldGraphPageName = listWidgetGraphPages->currentItem()->text();

    QInputDialog dlg(this);
    dlg.setWindowTitle(tr("修改画面名称"));
    dlg.setLabelText(tr("请输入画面名称"));
    dlg.setOkButtonText(tr("确定"));
    dlg.setCancelButtonText(tr("取消"));
    dlg.setTextValue(szOldGraphPageName);

reInput:
    if (dlg.exec() == QDialog::Accepted) {
        QString szNewGraphPageName = dlg.textValue();

        if (szNewGraphPageName == "") {
            goto reInput;
        }

        for (int i = 0; i < DrawListUtils::drawList_.count(); i++) {
            if ( szOldGraphPageName == DrawListUtils::drawList_.at(i) ) {
                DrawListUtils::drawList_.replace(i, szNewGraphPageName);
                QString szOldName = szProjPath_ + "/" + szOldGraphPageName + ".drw";
                QString szNewName = szProjPath_ + "/" + szNewGraphPageName + ".drw";
                QFile::rename(szOldName, szNewName);
                DrawListUtils::saveDrawList(szProjPath_);
                listWidgetGraphPages->currentItem()->setText(szNewGraphPageName);
                GraphPage *pGraphPage = GraphPageManager::getInstance()->getGraphPageById(szOldGraphPageName);
                pGraphPage->setFileName(szNewGraphPageName + ".drw");
                pGraphPage->setGraphPageId(szNewGraphPageName);
                graphPageTabWidget_->setTabText(graphPageTabWidget_->currentIndex(), szNewGraphPageName);
                currentGraphPage_->setUnsavedFlag(true);
                slotUpdateActions();
                break;
            }
        }
    }
}


/**
 * @brief DrawMainWindow::onDeleteGraphPage
 * @details 删除画面
 */
void DrawMainWindow::onDeleteGraphPage()
{
    QString szGraphPageName = listWidgetGraphPages->currentItem()->text();

    for (int i = 0; i < DrawListUtils::drawList_.count(); i++) {
        if ( szGraphPageName == DrawListUtils::drawList_.at(i) ) {
            DrawListUtils::drawList_.removeAt(i);

            QString fileName = szProjPath_ + "/" + szGraphPageName + ".drw";
            QFile file(fileName);
            if (file.exists()) {
                file.remove();
            }

            graphPageTabWidget_->removeTab(listWidgetGraphPages->currentRow());

            GraphPage *pGraphPageObj = GraphPageManager::getInstance()->getGraphPageById(szGraphPageName);
            if ( pGraphPageObj != Q_NULLPTR ) {
                GraphPageManager::getInstance()->removeGraphPage(pGraphPageObj);
                delete pGraphPageObj;
                pGraphPageObj = Q_NULLPTR;
            }

            DrawListUtils::saveDrawList(szProjPath_);

            listWidgetGraphPages->clear();
            foreach(QString szPageId, DrawListUtils::drawList_) {
                listWidgetGraphPages->addItem(szPageId);
            }

            if ( listWidgetGraphPages->count() > 0 ) {
                listWidgetGraphPages->setCurrentRow(0);
                graphPageTabWidget_->setCurrentIndex(0);
            }

            break;
        }
    }
}


/**
 * @brief DrawMainWindow::onCopyGraphPage
 * @details 复制画面
 */
void DrawMainWindow::onCopyGraphPage()
{
    szCopyGraphPageFileName_ = listWidgetGraphPages->currentItem()->text();
}


/**
 * @brief DrawMainWindow::onPasteGraphPage
 * @details 粘贴画面
 */
void DrawMainWindow::onPasteGraphPage()
{
    int iLast = 0;

reGetNum:
    iLast = DrawListUtils::getMaxDrawPageNum(szCopyGraphPageFileName_);
    QString strDrawPageName = szCopyGraphPageFileName_ + QString("-%1").arg(iLast);
    if ( DrawListUtils::drawList_.contains(strDrawPageName )) {
        szCopyGraphPageFileName_ = strDrawPageName;
        goto reGetNum;
    }

    listWidgetGraphPages->addItem(strDrawPageName);
    DrawListUtils::drawList_.append(strDrawPageName);
    DrawListUtils::saveDrawList(szProjPath_);
    QString szFileName = szProjPath_ + "/" + szCopyGraphPageFileName_ + ".drw";
    QFile file(szFileName);
    QString szPasteFileName = szProjPath_ + "/" + strDrawPageName + ".drw";
    file.copy(szPasteFileName);

    if (szPasteFileName.toLower().endsWith(".drw")) {
        QGraphicsView *view = createTabView();

        if (graphPageTabWidget_->indexOf(view) != -1) {
            delete view;
            return;
        }

        GraphPage *graphPage = new GraphPage(QRectF(), variantPropertyManager_, propertyEditor_);
        if (!createDocument(graphPage, view, szPasteFileName)) {
            return;
        }

        currentGraphPage_ = graphPage;
        currentView_ = dynamic_cast<QGraphicsView *>(view);
        graphPage->setProjectPath(szProjPath_);
        graphPage->setProjectName(szProjName_);
        graphPage->loadAsXML(szPasteFileName);
        view->setFixedSize(graphPage->getGraphPageWidth(), graphPage->getGraphPageHeight());
        graphPage->setFileName(strDrawPageName + ".drw");
        graphPage->setGraphPageId(strDrawPageName);
    }

    QList<QListWidgetItem*> listWidgetItem = listWidgetGraphPages->findItems(strDrawPageName, Qt::MatchCaseSensitive);
    if ( listWidgetItem.size() > 0 ) {
        listWidgetGraphPages->setCurrentItem(listWidgetItem.at(0));
        graphPageTabWidget_->setCurrentIndex(listWidgetGraphPages->currentRow());
    }

    currentGraphPage_->setUnsavedFlag(true);
    slotUpdateActions();
}
