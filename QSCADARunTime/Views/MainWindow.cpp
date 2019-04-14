#include "MainWindow.h"
#include "Helper.h"
#include "DrawListUtils.h"
#include <QDesktopWidget>
#include <QApplication>
#include <QFileInfo>
#include <QRect>
#include <QGraphicsView>
#include <QFileDialog>
#include <QDebug>


MainWindow::MainWindow(const QString &projpath,
                       const QString &graphPageName,
                       QWidget *parent) :
    QMainWindow(parent),
    currentGraphPage_(0),
    currentView_(0),
    currentGraphPageIndex_(0),
    projpath_(projpath),
    graphPageName_(graphPageName) {

    // 注册元素对象创建函数
    registerCreateObjectFunc();

    setWindowFlags(Qt::FramelessWindowHint);

    moveCenter();

}


MainWindow::~MainWindow() {

}


/**
 * @brief openGraphPage
 * @details 打开画面
 * @param pagePath 画面路径
 * @param pagePath 画面名称
 */
void MainWindow::openGraphPage(const QString &pagePath, const QString &pageName) {
    QString fileName = pagePath + "/" + pageName;

    if (fileName.toLower().endsWith(".drw")) {
        QGraphicsView *view = createTabView();
        GraphPage *graphPage = new GraphPage(QRectF());
        if (!createDocument(graphPage, view, fileName)) {
            return;
        }
        graphPage->setProjectPath(pagePath);
        graphPage->loadAsXML(fileName);
        view->setFixedSize(graphPage->getGraphPageWidth(), graphPage->getGraphPageHeight());
        graphPage->setFileName(pageName + ".drw");
        graphPage->setGraphPageId(pageName);

        this->resize(graphPage->getGraphPageWidth(), graphPage->getGraphPageHeight());
        moveCenter();
    }
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

QGraphicsView *MainWindow::createTabView() {

    QGraphicsView *view = new QGraphicsView(this);
    view->setDragMode(QGraphicsView::RubberBandDrag);
    view->setCacheMode(QGraphicsView::CacheBackground);
    view->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    return view;
}


void MainWindow::slotCloseAll() {
    currentView_ = 0;
    currentGraphPage_ = 0;
}

void MainWindow::slotCloseGraphPage() {

    QGraphicsView *view = currentView_;
    delete view;

    currentGraphPage_ = 0;
    currentView_ = 0;
}

bool MainWindow::createDocument(GraphPage *graphPage,
                                QGraphicsView *view,
                                const QString &filename) {

    if (isGraphPageOpen(filename)) {
        QMessageBox::information(this,
                                 trUtf8("打开文件错误"),
                                 trUtf8("文件已打开"),
                                 QMessageBox::Ok);
        delete graphPage;
        delete view;
        return false;
    }

    currentGraphPage_ = graphPage;
    view->setScene(graphPage);
    view->setFixedSize(graphPage->getGraphPageWidth(), graphPage->getGraphPageHeight());
    currentView_ = view;
    GraphPageManager::getInstance()->addGraphPage(graphPage);
    graphPage->undoStack()->setClean();

    return true;
}

void MainWindow::updateGraphPageViewInfo(const QString &fileName) {
    QFileInfo file(fileName);
    currentGraphPage_->setGraphPageId(file.baseName());
}


void MainWindow::moveCenter() {
    Helper::WidgetMoveCenter(this);
}








