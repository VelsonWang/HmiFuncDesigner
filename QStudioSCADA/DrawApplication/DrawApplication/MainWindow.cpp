#include "MainWindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    currentGraphPage(0),
    currentView(0),
    gridVisible(true),
    currentGraphPageIndex(0)
{
    setupUi(this);

    undoGroup = new QUndoGroup(this);
    createActions();
    createMenus();
    createToolbars();
    initView();
    slotUpdateActions();

    setWindowState(Qt::WindowMaximized);
    setWindowTitle(trUtf8("编辑器"));
    setWindowIcon(QIcon(":/images/application.png"));

    connect(graphPageTabWidget_,SIGNAL(currentChanged(int)),SLOT(slotChangeGraphPage(int)));
}

void MainWindow::initView() {

    graphPageTabWidget_ = new QTabWidget(this);
    this->setCentralWidget(graphPageTabWidget_);

    grapePageTree_ = new GraphPageTreeView();
    this->GraphPagesLayout->addWidget(grapePageTree_);

    elementWidget = new ElementLibraryWidget();
    this->ElemetsLayout->addWidget(elementWidget);

    propertyModel = new PropertyModel();
    propertyView = new PropertyTableView(propertyModel);
    this->PropertyLayout->addWidget(propertyView);

    preview = new PreviewWindow;

    objTree = new ObjectsTreeView();
    this->ObjectTreeLayout->addWidget(objTree);
}

void MainWindow::createActions() {

    actionShowGraphObj = new QAction(trUtf8("图形元素"),this);
    actionShowGraphObj->setCheckable(true);
    actionShowGraphObj->setChecked(true);
    connect(actionShowGraphObj,SIGNAL(triggered(bool)),SLOT(slotShowGraphObj(bool)));

    actionShowTreeObj = new QAction(trUtf8("对象树"),this);
    actionShowTreeObj->setCheckable(true);
    actionShowTreeObj->setChecked(true);
    connect(actionShowTreeObj,SIGNAL(triggered(bool)),SLOT(slotShowTreeObj(bool)));

    actionShowPropEditor = new QAction(trUtf8("属性编辑器"),this);
    actionShowPropEditor->setCheckable(true);
    actionShowPropEditor->setChecked(true);
    connect(actionShowPropEditor,SIGNAL(triggered(bool)),SLOT(slotShowPropEditor(bool)));

    actionShowProjTree = new QAction(trUtf8("项目树"),this);
    actionShowProjTree->setCheckable(true);
    actionShowProjTree->setChecked(true);
    connect(actionShowProjTree,SIGNAL(triggered(bool)),SLOT(slotShowProjTree(bool)));

    actionNew = new QAction(QIcon(":/images/filenew.png"),trUtf8("新建"),this);
    actionNew->setShortcut(QString("Ctrl+N"));
    connect(actionNew,SIGNAL(triggered()),SLOT(slotEditNew()));

    actionOpen = new QAction(QIcon(":/images/fileopen.png"),trUtf8("打开一个项目"),this);
    actionOpen->setShortcut(QString("Ctrl+O"));
    connect(actionOpen,SIGNAL(triggered()),SLOT(slotEditOpen()));

    actionSaveProject = new QAction(QIcon(":/images/filesave.png"),trUtf8("保存项目"),this);
    connect(actionSaveProject,SIGNAL(triggered()),SLOT(slotSaveProject()));

    actionSaveGraphPageAs = new QAction(trUtf8("另存为"),this);
    actionSaveGraphPageAs->setShortcut(QString("Ctrl+Shift+S"));
    connect(actionSaveGraphPageAs,SIGNAL(triggered()),SLOT(slotSaveGraphPageAs()));

    actionSaveGraphPage = new QAction(trUtf8("保存"),this);
    actionSaveGraphPage->setShortcut(QKeySequence::Save);
    connect(actionSaveGraphPage,SIGNAL(triggered()),SLOT(slotSaveGraphPage()));

    actionCloseGraphPage = new QAction(trUtf8("关闭"),this);
    connect(actionCloseGraphPage,SIGNAL(triggered()),SLOT(slotCloseGraphPage()));

    actionCloseAll = new QAction(trUtf8("关闭所有"),this);
    connect(actionCloseAll,SIGNAL(triggered()),SLOT(slotCloseAll()));

    actionExit = new QAction(trUtf8("退出"),this);
    actionExit->setShortcut(QKeySequence::Quit);
    connect(actionExit,SIGNAL(triggered()),SLOT(slotExit()));

    actionShowGrid = new QAction(QIcon(":/images/showgrid.png"),trUtf8("显示栅格"),this);
    actionShowGrid->setCheckable(true);
    actionShowGrid->setChecked(false);
    connect(actionShowGrid,SIGNAL(triggered(bool)),SLOT(slotShowGrid(bool)));

    actionShowLinear = new QAction(QIcon(":/images/ruler.png"),trUtf8("显示线条"),this);
    actionShowLinear->setCheckable(true);
    actionShowLinear->setChecked(false);
    connect(actionShowLinear,SIGNAL(triggered(bool)),SLOT(slotShowLinear(bool)));

    actionZoomIn = new QAction(QIcon(":/images/zoom-in.png"),trUtf8("放大"),this);
    connect(actionZoomIn,SIGNAL(triggered()),SLOT(slotZoomIn()));

    actionZoomOut = new QAction(QIcon(":/images/zoom-out.png"),trUtf8("缩小"),this);
    connect(actionZoomOut,SIGNAL(triggered()),SLOT(slotZoomOut()));

    actionPreview = new QAction(QIcon(":/images/preview.png"),trUtf8("预览"),this);
    connect(actionPreview,SIGNAL(triggered()),SLOT(slotPreview()));

    actionUndo = undoGroup->createUndoAction(this);
    actionUndo->setIcon(QIcon(":/images/undo.png"));
    actionUndo->setText(trUtf8("撤销"));
    actionUndo->setShortcut(QKeySequence::Undo);

    actionRedo = undoGroup->createRedoAction(this);
    actionRedo->setText(trUtf8("重做"));
    actionRedo->setIcon(QIcon(":/images/redo.png"));
    actionRedo->setShortcut(QKeySequence::Redo);

}

void MainWindow::createMenus() {

    QMenu *filemenu = new QMenu(trUtf8("文件"),this);
    filemenu->addAction(actionNew);
    filemenu->addAction(actionOpen);
    filemenu->addAction(actionSaveProject);
    filemenu->addAction(actionSaveGraphPageAs);
    filemenu->addAction(actionSaveGraphPage);
    filemenu->addSeparator();
    filemenu->addAction(actionCloseGraphPage);
    filemenu->addAction(actionCloseAll);
    filemenu->addSeparator();
    filemenu->addAction(actionExit);

    QMenu *windowMenu = new QMenu(trUtf8("窗口"),this);
    windowMenu->addAction(actionShowGraphObj);
    windowMenu->addAction(actionShowTreeObj);
    windowMenu->addAction(actionShowPropEditor);
    windowMenu->addAction(actionShowProjTree);

    QMainWindow::menuBar()->addMenu(filemenu);
    QMainWindow::menuBar()->addMenu(windowMenu);
}

void MainWindow::createToolbars() {

    toolBar->addAction(actionShowGrid);
    //toolBar->addAction(actionShowLinear);
    toolBar->addAction(actionZoomOut);
    toolBar->addAction(actionZoomIn);
    toolBar->addAction(actionPreview);
    toolBar->addSeparator();
    toolBar->addAction(actionUndo);
    toolBar->addAction(actionRedo);
    toolBar->addSeparator();

}

void MainWindow::closeEvent(QCloseEvent *event) {

    bool unsaved = false;

    QListIterator <GraphPage*> it(GraphPageManager::getInstance()->getGraphPageList());

    while (it.hasNext()) {
        GraphPage *graphPage = it.next();
        if (!graphPage->undoStack()->isClean() || graphPage->getUnsavedFlag()) {
            unsaved = true;
        }
    }

    if (unsaved) {
        int r = exitResponse();

        if (r == QMessageBox::Yes) {
            slotSaveProject();
            event->accept();
        }
        else if (r == QMessageBox::No) {
            event->accept();
        }
    }

    event->accept();
}

void MainWindow::slotEditNew() {

    addNewGraphPage();
}

QString MainWindow::fixedWindowTitle(const QGraphicsView *viewGraphPage) const {

    QString title = currentGraphPage->getGraphPageId();

    if (title.isEmpty()) {
        title = "Untitled";
    }
    else {
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

bool MainWindow::isGraphPageOpen(const QString &filename) {

    QListIterator <GraphPage*> it(GraphPageManager::getInstance()->getGraphPageList());

    while (it.hasNext()) {
        if (filename == it.next()->getFileName()) {
            return true;
        }
    }

    return false;
}

void MainWindow::addNewGraphPage() {

    QGraphicsView *view = createTabView();
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    if (graphPageTabWidget_->indexOf(view) != -1) {
        delete view;
        return;
    }

    GraphPage *graphPage = new GraphPage(QRectF());
    graphPage->setGridVisible(gridVisible);
    currentGraphPage = graphPage;
    view->setScene(graphPage);

    view->setFixedSize(graphPage->getGraphPageWidth(), graphPage->getGraphPageHeight());

    currentView = view;
    QString title = fixedWindowTitle(view);
    graphPage->setGraphPageId(title);
    graphPage->setPropertyModel(propertyModel);
    graphPageTabWidget_->addTab(currentView, title);
    graphPageTabWidget_->setCurrentWidget(view);
    grapePageTree_->addNewElement(graphPage->getGraphPageId());
    GraphPageManager::getInstance()->addGraphPage(graphPage);

    undoGroup->addStack(graphPage->undoStack());
    undoGroup->setActiveStack(graphPage->undoStack());

    connectGraphPage(graphPage);
}

void MainWindow::connectGraphPage(GraphPage *graphPage) {

    connect(graphPage->undoStack(),SIGNAL(indexChanged(int)),SLOT(slotUpdateActions()));
    connect(graphPage->undoStack(),SIGNAL(cleanChanged(bool)),SLOT(slotUpdateActions()));
    connect(graphPage,SIGNAL(newElementAdded()),SLOT(slotNewElementAdded()));
    connect(graphPage,SIGNAL(elementsDeleted()),SLOT(slotElementsDeleted()));
    connect(graphPage,SIGNAL(elementIdChanged()),SLOT(slotElementIdChanged()));
    connect(graphPage,SIGNAL(changeGraphPageName()),SLOT(slotChangeGraphPageName()));
    connect(graphPage,SIGNAL(selectionChanged()),SLOT(slotUpdateActions()));
    connect(graphPage,SIGNAL(elementPropertyChanged()),SLOT(slotUpdateActions()));
    connect(graphPage,SIGNAL(GraphPagePropertyChanged()),SLOT(slotUpdateActions()));
    connect(graphPage,SIGNAL(GraphPageSaved()),SLOT(slotUpdateActions()));
}

void MainWindow::disconnectGraphPage(GraphPage *graphPage) {

    disconnect(graphPage->undoStack(),SIGNAL(indexChanged(int)),this,SLOT(slotUpdateActions()));
    disconnect(graphPage->undoStack(),SIGNAL(cleanChanged(bool)),this,SLOT(slotUpdateActions()));
    disconnect(graphPage,SIGNAL(newElementAdded()),this,SLOT(slotNewElementAdded()));
    disconnect(graphPage,SIGNAL(elementsDeleted()),this,SLOT(slotElementsDeleted()));
    disconnect(graphPage,SIGNAL(elementIdChanged()),this,SLOT(slotElementIdChanged()));
    disconnect(graphPage,SIGNAL(changeGraphPageName()),this,SLOT(slotChangeGraphPageName()));
    disconnect(graphPage,SIGNAL(selectionChanged()),this,SLOT(slotUpdateActions()));
    disconnect(graphPage,SIGNAL(elementPropertyChanged()),this,SLOT(slotUpdateActions()));
    disconnect(graphPage,SIGNAL(GraphPagePropertyChanged()),this,SLOT(slotUpdateActions()));
    disconnect(graphPage,SIGNAL(GraphPageSaved()),this,SLOT(slotUpdateActions()));
}

QGraphicsView *MainWindow::createTabView() {

    QGraphicsView *view = new QGraphicsView;
    view->setDragMode(QGraphicsView::RubberBandDrag);
    view->setCacheMode(QGraphicsView::CacheBackground);
    view->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    return view;
}

void MainWindow::slotUpdateActions() {

    static const QIcon unsaved(":/images/filesave.png");

    for (int i = 0; i < graphPageTabWidget_->count(); i++) {
        QGraphicsView *view = (QGraphicsView*)graphPageTabWidget_->widget(i);

        if (!((GraphPage*)view->scene())->undoStack()->isClean() ||
            ((GraphPage*)view->scene())->getUnsavedFlag())
        {
            graphPageTabWidget_->setTabIcon(graphPageTabWidget_->indexOf(view),unsaved);
        }
        else {
            graphPageTabWidget_->setTabIcon(graphPageTabWidget_->indexOf(view),QIcon());
        }
    }

    actionZoomIn->setEnabled(graphPageTabWidget_->count() ? true : false);
    actionZoomOut->setEnabled(graphPageTabWidget_->count() ? true : false);
    actionPreview->setEnabled(graphPageTabWidget_->count() ? true : false);
    actionShowGrid->setEnabled(graphPageTabWidget_->count() ? true : false);

    if (!currentGraphPage) {
        return;
    }

    undoGroup->setActiveStack(currentGraphPage->undoStack());

    if (!currentGraphPage->undoStack()->isClean() || currentGraphPage->getUnsavedFlag()) {
        actionSaveGraphPage->setEnabled(true);
    }
    else {
        actionSaveGraphPage->setEnabled(false);
    }
}

void MainWindow::slotChangeGraphPage(int GraphPageNum) {

    if (GraphPageNum == -1) {
        objTree->clearModel();
        propertyModel->resetModel();
        return;
    }

    for (int i = 0; i < graphPageTabWidget_->count(); i++) {
        QGraphicsView *view = (QGraphicsView*)graphPageTabWidget_->widget(i);
        ((GraphPage*)view->scene())->setActive(false);
    }

    currentView = (QGraphicsView*)graphPageTabWidget_->widget(GraphPageNum);
    currentGraphPage = (GraphPage*)currentView->scene();
    currentGraphPage->setActive(true);
    currentGraphPage->fillGraphPagePropertyModel();
    objTree->setContentList(currentGraphPage->items());
    objTree->updateContent();
    currentGraphPageIndex = GraphPageNum;
    slotUpdateActions();
}

void MainWindow::slotChangeGraphPageName() {

    graphPageTabWidget_->setTabText(currentGraphPageIndex,currentGraphPage->getGraphPageId());

    int index = GraphPageManager::getInstance()->getIndexByGraphPage(currentGraphPage);

    if (index >= 0) {
        grapePageTree_->graphPageChangeName(currentGraphPageIndex,currentGraphPage->getGraphPageId());
    }
}

void MainWindow::updateObjectTree() {

    objTree->setContentList(currentGraphPage->items());
    objTree->updateContent();
}

void MainWindow::slotElementIdChanged() {

    updateObjectTree();
}

void MainWindow::slotElementPropertyChanged() {

}

void MainWindow::slotGraphPagePropertyChanged() {

}

void MainWindow::slotNewElementAdded() {

    updateObjectTree();
}

void MainWindow::slotElementsDeleted() {

    updateObjectTree();
}

void MainWindow::slotShowGrid(bool on) {

    QListIterator <GraphPage*> i(GraphPageManager::getInstance()->getGraphPageList());

    while (i.hasNext()) {
        i.next()->setGridVisible(on);
    }

    gridVisible = on;
}

void MainWindow::slotShowGraphObj(bool on) {

    on ? this->dockElemets->show() : this->dockElemets->hide();
}

void MainWindow::slotShowTreeObj(bool on) {

    on ? this->dockObjectTree->show() : this->dockObjectTree->hide();
}

void MainWindow::slotShowPropEditor(bool on) {

    on ? this->dockProperty->show() : this->dockProperty->hide();
}

void MainWindow::slotShowProjTree(bool on) {

    on ? this->dockGraphPages->show() : this->dockGraphPages->hide();
}

void MainWindow::slotCloseAll() {

    while (graphPageTabWidget_->count()) {
        QGraphicsView *view = static_cast<QGraphicsView*>(graphPageTabWidget_->widget(graphPageTabWidget_->currentIndex()));
        removeGraphPage(view);
        delete view;
    }

    currentView = 0;
    currentGraphPage = 0;
    slotUpdateActions();
}

void MainWindow::removeGraphPage(QGraphicsView *view) {

    int index = graphPageTabWidget_->indexOf(view);
    GraphPage *graphPage = static_cast<GraphPage*>(view->scene());

    if (index == -1)
        return;

    if (!graphPage->undoStack()->isClean()) {
        int r = exitResponse();

        if (r == QMessageBox::Yes) {
            slotSaveGraphPage();
        }
    }

    graphPageTabWidget_->removeTab(index);
    undoGroup->removeStack(graphPage->undoStack());
    GraphPageManager::getInstance()->removeGraphPage(graphPage);
    grapePageTree_->updateView();
    disconnectGraphPage(graphPage);
    delete graphPage;
}

void MainWindow::slotCloseGraphPage() {

    QGraphicsView *view = currentView;
    removeGraphPage(view);
    delete view;

    if (graphPageTabWidget_->count() == 0) {
        currentGraphPage = 0;
        currentView = 0;
    }

    slotUpdateActions();
}

void MainWindow::slotEditOpen() {

    const QString &filename = QFileDialog::getOpenFileName(this,
                                                           trUtf8("Open"),
                                                           ".", trUtf8("Mnemo designer project (*.mdproj);;"
                                                                       "Mnemo designer GraphPage Bin (*.mdGraphPagebin);;"
                                                                       "Mnemo designer GraphPage XML (*.mdGraphPagexml)"));
    if (filename.isEmpty()) {
        return;
    }

    if (filename.toLower().endsWith(".mdproj")) {
        loadProject(filename);
    }

    if (filename.toLower().endsWith(".mdGraphPagebin")) {
        //refactor it!!!

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

    if (filename.toLower().endsWith(".mdGraphPagexml")) {

        QGraphicsView *view = createTabView();

        if (graphPageTabWidget_->indexOf(view) != -1) {
            delete view;
            return;
        }

        GraphPage *graphPage = new GraphPage(QRectF());
        if (!createDocument(graphPage,view,filename)) {
            return;
        }
        graphPage->loadAsXML(filename);
    }
}

bool MainWindow::createDocument(GraphPage *graphPage,QGraphicsView *view,const QString &filename) {

    if (isGraphPageOpen(filename)) {
        QMessageBox::information(this,trUtf8("打开文件错误"),trUtf8("文件已打开"),QMessageBox::Ok);
        delete graphPage;
        delete view;
        return false;
    }

    graphPage->setGridVisible(gridVisible);
    currentGraphPage = graphPage;
    view->setScene(graphPage);
    currentView = view;
    graphPage->setPropertyModel(propertyModel);
    graphPageTabWidget_->addTab(currentView,graphPage->getGraphPageId());
    graphPageTabWidget_->setCurrentWidget(view);
    grapePageTree_->addNewElement(graphPage->getGraphPageId());
    GraphPageManager::getInstance()->addGraphPage(graphPage);

    undoGroup->addStack(graphPage->undoStack());
    undoGroup->setActiveStack(graphPage->undoStack());

    connectGraphPage(graphPage);

    graphPage->undoStack()->setClean();

    return true;
}

void MainWindow::loadProject(const QString &filename) {

    QFile file(filename);

    QDataStream in(&file);

    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(this,trUtf8("错误"),trUtf8("无法打开文件。"),
                                 QMessageBox::Ok);
        return;
    }

    while (!in.atEnd()) {

        QGraphicsView *view = createTabView();

        if (graphPageTabWidget_->indexOf(view) != -1) {
            delete view;
            return;
        }

        GraphPage *graphPage = new GraphPage(QRectF());
        in >> *graphPage;
        graphPage->readItems(in,0,false);
        graphPage->setGridVisible(gridVisible);
        currentGraphPage = graphPage;
        view->setScene(graphPage);
        currentView = view;
        graphPage->setPropertyModel(propertyModel);
        graphPageTabWidget_->addTab(currentView,graphPage->getGraphPageId());
        graphPageTabWidget_->setCurrentWidget(view);
        grapePageTree_->addNewElement(graphPage->getGraphPageId());
        GraphPageManager::getInstance()->addGraphPage(graphPage);

        undoGroup->addStack(graphPage->undoStack());
        undoGroup->setActiveStack(graphPage->undoStack());

        connectGraphPage(graphPage);

        graphPage->undoStack()->setClean();
    }

    file.close();
}

void MainWindow::slotSaveProject() {

    int r = QMessageBox::information(this,trUtf8("保存"),trUtf8("保存这个项目?"),
                                     QMessageBox::Ok|QMessageBox::Cancel);

    if (r == QMessageBox::Ok) {
        QString fileName = QFileDialog::getSaveFileName(this,trUtf8("Save as"),
                                                        ".",trUtf8("Mnemo designer (*.mdproj)"));
        if (fileName.isEmpty()) {
            return;
        }

        if (!fileName.toLower().endsWith(".mdproj")) {
            fileName += ".mdproj";
        }

        saveProject(fileName);
    }
    else if (r == QMessageBox::Cancel) {
        return;
    }
}

void MainWindow::saveProject(const QString &filename) {

    QFile file(filename);

    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::information(this,trUtf8("错误"),trUtf8("无法保存文件"),
                                 QMessageBox::Ok);
    }

    QDataStream out(&file);

    QListIterator <GraphPage*> it(GraphPageManager::getInstance()->getGraphPageList());

    while (it.hasNext()) {

        GraphPage *graphPage = it.next();
        out << *graphPage;
        graphPage->writeItems(out,graphPage->items());
        graphPage->undoStack()->setClean();
    }

    file.close();
}

void MainWindow::slotSaveGraphPageAs() {

    SaveStrategyDialog saveAsDialog(this);
    saveAsDialog.exec();

    if (!currentGraphPage) {
        return;
    }

    if (saveAsDialog.result() == QDialog::Accepted) {

        switch (saveAsDialog.getLastChoose()) {

            case SAVE_XML: {
                    QString filename = QFileDialog::getSaveFileName(this,trUtf8("Save as"),
                                                                QString("./%1").arg(currentGraphPage->getGraphPageId()),
                                                                trUtf8("XML(*.mdGraphPagexml)"));
                    currentGraphPage->setFileName(filename);
                    currentGraphPage->saveAsXML(filename);
                    break;
                }
            case SAVE_BINARY: {
                    QString filename = QFileDialog::getSaveFileName(this,trUtf8("Save as"),
                                                                QString("./%1").arg(currentGraphPage->getGraphPageId()),
                                                                trUtf8("Binary(*.mdGraphPagebin)"));
                    currentGraphPage->setFileName(filename);
                    currentGraphPage->saveAsBinary(filename);
                    break;
                }
            case SAVE_IMAGE:
                saveImage();
                break;
        }
    }
}

QString MainWindow::getFileName() {

    QString filename = QFileDialog::getSaveFileName(this,trUtf8("Save as"),
                                                    QString("./%1").arg(currentGraphPage->getGraphPageId()),
                                                    trUtf8("Binary(*.mdGraphPagebin);;XML(*.mdGraphPagexml)"));
    return filename;
}

void MainWindow::updateGraphPageViewInfo(const QString &fileName) {

    int index = graphPageTabWidget_->indexOf(currentView);
    QFileInfo fi(fileName);
    currentGraphPage->setGraphPageId(fi.baseName());
    graphPageTabWidget_->setTabText(index,fi.baseName());
    slotChangeGraphPageName();
}

void MainWindow::slotSaveGraphPage() {

    if (!currentGraphPage) {
        return;
    }

    for (;;) {
        QString fileName = currentGraphPage->getFileName();

        if (fileName.isEmpty())
            fileName = getFileName();

        if (fileName.isEmpty())
            break;

        QFileInfo fi(fileName);

        if (fileName.toLower().endsWith(".mdGraphPagexml")) {
            currentGraphPage->setFileName(fileName);
            updateGraphPageViewInfo(fileName);
            currentGraphPage->saveAsXML(fileName);
        }

        if (fileName.toLower().endsWith(".mdGraphPagebin")) {
            currentGraphPage->setFileName(fileName);
            updateGraphPageViewInfo(fileName);
            currentGraphPage->saveAsBinary(fileName);
        }

        break;

    }
}

void MainWindow::slotExit() {

    if (graphPageTabWidget_->count() == 0) {
        QApplication::quit();
        return;
    }

    bool unsaved = false;

    QListIterator <GraphPage*> it(GraphPageManager::getInstance()->getGraphPageList());

    while (it.hasNext()) {
        if (!it.next()->undoStack()->isClean()) {
            unsaved = true;
        }
    }

    if (unsaved) {
        int r = exitResponse();

        if (r == QMessageBox::Yes) {
            slotSaveProject();
            QApplication::quit();
        }
        else if (r == QMessageBox::No) {
            QApplication::quit();
        }
    }

    QApplication::quit();
}

int MainWindow::exitResponse() {

    int r = QMessageBox::information(this,trUtf8("退出程序"),
                             trUtf8("文件已修改。是否保存?"),
                             QMessageBox::Yes | QMessageBox::No);
    return r;
}

void MainWindow::slotShowLinear(bool on) {
    Q_UNUSED(on)
}

void MainWindow::slotZoomIn() {

    if (currentView) {
        currentView->scale(1.25,1.25);
    }
}

void MainWindow::slotZoomOut() {

    if (currentView) {
        currentView->scale(1/1.25,1/1.25);
    }
}

void MainWindow::slotPreview() {

    if (!currentGraphPage) {
        return;
    }

    QPixmap pixmap(currentGraphPage->width(),currentGraphPage->height());

    {
        QPainter painter(&pixmap);
        painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
        currentGraphPage->render(&painter);
    }

    preview->updatePreview(pixmap);
}

void MainWindow::saveImage() {

    const QString filename = QFileDialog::getSaveFileName(this,trUtf8("Save As"),
                                                          ".",trUtf8("*.jpg"));
    if (filename.isEmpty()) {
        return;
    }

    QImage image(currentGraphPage->width(),currentGraphPage->height(),QImage::Format_ARGB32);

    {
        QPainter painter(&image);
        painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

        bool showGrid = currentGraphPage->isGridVisible();

        if (showGrid) {
            currentGraphPage->setGridVisible(false);
        }

        QList <QGraphicsItem*> items = currentGraphPage->selectedItems();
        currentGraphPage->clearSelection();

        currentGraphPage->render(&painter);

        if (showGrid) {
            currentGraphPage->setGridVisible(true);
        }

        foreach (QGraphicsItem *item, items) {
            item->setSelected(true);
        }

        currentGraphPage->slotSelectionChanged();
    }

    if (!image.save(filename)) {
        QMessageBox::information(this,trUtf8("保存错误"),
                                 trUtf8("文件无法保存"), QMessageBox::Ok);
    }
}
