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
    currentGraphPage_(nullptr),
    projpath_(projpath),
    graphPageName_(graphPageName) {

    // 注册元素对象创建函数
    registerCreateObjectFunc();

    // 方便调试移动窗口暂时移除
    //setWindowFlags(Qt::FramelessWindowHint);

    DrawListUtils::loadDrawList(projpath_);
    foreach (QString pageName, DrawListUtils::drawList_) {
        QString fileName = projpath_ + "/" + pageName + ".drw";
        if (fileName.toLower().endsWith(".drw")) {
            GraphPage *graphPage = new GraphPage(QRectF(), this);
            if (!createDocument(graphPage, fileName)) {
                return;
            }
            graphPage->setProjectPath(projpath_);
            graphPage->loadAsXML(fileName);
            graphPage->resize(graphPage->getGraphPageWidth(), graphPage->getGraphPageHeight());
            graphPage->setFileName(pageName + ".drw");
            graphPage->setGraphPageId(pageName);
        }
    }

    moveCenter();
}


MainWindow::~MainWindow() {
    GraphPageManager::releaseInstance();
    currentGraphPage_ = nullptr;
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
        QString pageName_ = pageName;
        QString pageId = pageName_.replace(".drw", "");
        GraphPage *graphPage = GraphPageManager::getInstance()->getGraphPageById(pageId);
        if(graphPage != nullptr) {
            currentGraphPage_ = graphPage;
            this->resize(graphPage->getGraphPageWidth(), graphPage->getGraphPageHeight());
        }
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

bool MainWindow::createDocument(GraphPage *graphPage,
                                const QString &filename) {
    if (isGraphPageOpen(filename)) {
        QMessageBox::information(this,
                                 trUtf8("打开文件错误"),
                                 trUtf8("文件已打开"),
                                 QMessageBox::Ok);
        delete graphPage;
        return false;
    }

    currentGraphPage_ = graphPage;
    GraphPageManager::getInstance()->addGraphPage(graphPage);

    return true;
}

void MainWindow::updateGraphPageViewInfo(const QString &fileName) {
    QFileInfo file(fileName);
    currentGraphPage_->setGraphPageId(file.baseName());
}


void MainWindow::moveCenter() {
    Helper::WidgetMoveCenter(this);
}








