#include "MainWindow.h"
#include "Helper.h"
#include "DrawListUtils.h"
#include "Log/Log.h"
#include "HmiRunTime.h"
#include <QDesktopWidget>
#include <QApplication>
#include <QFileInfo>
#include <QRect>
#include <QGraphicsView>
#include <QFileDialog>
#include <QDebug>

QMutex MainWindow::mutex_;
MainWindow *MainWindow::instance_ = Q_NULLPTR;
QString MainWindow::projpath_ = "";
QString MainWindow::graphPageName_ = "";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    currentGraphPage_(nullptr)
{
    // 注册元素对象创建函数
    registerCreateObjectFunc();

    // 方便调试移动窗口暂时移除
    //setWindowFlags(Qt::FramelessWindowHint);

    showedGraphPageStack_.clear();

    moveCenter();
}


MainWindow::~MainWindow()
{
    unLoadGraphPages();
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event)
    LogInfo("exit application!");
    g_pHmiRunTime->Stop();
    qApp->exit(0);
}


/**
 * @brief MainWindow::loadGraphPages
 * @details 加载工程画面
 * @param pagePath
 */
void MainWindow::loadGraphPages(const QString &pagePath)
{
    projpath_ = pagePath;
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
            graphPage->hide();
        }
    }
}


/**
 * @brief MainWindow::unLoadGraphPages
 * @details 卸载工程画面
 */
void MainWindow::unLoadGraphPages()
{
    GraphPageManager::releaseInstance();
    currentGraphPage_ = nullptr;
}


/**
 * @brief openGraphPage
 * @details 打开画面
 * @param pagePath 画面路径
 * @param pagePath 画面名称
 */
void MainWindow::openGraphPage(const QString &pagePath, const QString &pageName)
{
    QString fileName = pagePath + "/" + pageName;
    if (fileName.toLower().endsWith(".drw")) {
        QString pageName_ = pageName;
        QString pageId = pageName_.replace(".drw", "");
        GraphPage *graphPage = GraphPageManager::getInstance()->getGraphPageById(pageId);
        if(graphPage != nullptr) {
            currentGraphPage_ = graphPage;
            graphPage->openGraphPage();
            graphPage->show();
            this->resize(graphPage->getGraphPageWidth(), graphPage->getGraphPageHeight());
        }
        moveCenter();
    }
}


/**
 * @brief MainWindow::switchGraphPage
 * @details 切换至画面
 * @param pageName 画面名称
 */
void MainWindow::switchGraphPage(const QString &pageName)
{
    QString pageId = pageName;
    GraphPage *graphPage = GraphPageManager::getInstance()->getGraphPageById(pageId);
    if(graphPage != nullptr) {
        showedGraphPageStack_.push(currentGraphPage_);
        currentGraphPage_->closeGraphPage();
        currentGraphPage_ = graphPage;
        graphPage->openGraphPage();
        graphPage->show();
        this->resize(graphPage->getGraphPageWidth(), graphPage->getGraphPageHeight());
    }
    moveCenter();
}

/**
 * @brief MainWindow::returnGraphPage
 * @details 返回功能切换画面
 */
void MainWindow::returnGraphPage()
{
    if(currentGraphPage_ != nullptr) {
        if(showedGraphPageStack_.size() > 0) {
            currentGraphPage_->hide();
            currentGraphPage_->closeGraphPage();
            GraphPage *graphPage = showedGraphPageStack_.pop();
            if(graphPage != nullptr) {
                graphPage->openGraphPage();
                graphPage->show();
                currentGraphPage_ = graphPage;
                graphPage->move(0, 0);
                this->resize(graphPage->getGraphPageWidth(), graphPage->getGraphPageHeight());
            }
        }
    }
}

bool MainWindow::isGraphPageOpen(const QString &filename)
{
    QListIterator <GraphPage*> it(GraphPageManager::getInstance()->getGraphPageList());
    while (it.hasNext()) {
        if (filename == it.next()->getFileName()) {
            return true;
        }
    }
    return false;
}

bool MainWindow::createDocument(GraphPage *graphPage,
                                const QString &filename)
{
    if (isGraphPageOpen(filename)) {
        QMessageBox::information(this,
                                 tr("打开文件错误"),
                                 tr("文件已打开"),
                                 QMessageBox::Ok);
        delete graphPage;
        return false;
    }

    currentGraphPage_ = graphPage;
    GraphPageManager::getInstance()->addGraphPage(graphPage);

    return true;
}

void MainWindow::updateGraphPageViewInfo(const QString &fileName)
{
    QFileInfo file(fileName);
    currentGraphPage_->setGraphPageId(file.baseName());
}


void MainWindow::moveCenter()
{
    Helper::WidgetMoveCenter(this);
}


/**
 * @brief MainWindow::hideControlElement
 * @details 隐藏控件
 * @param eleId 控件ID
 */
void MainWindow::hideControlElement(const QString &eleId)
{
    QStringList listPageIdEleId = eleId.split('.');
    if(listPageIdEleId.size() == 2) {
        QString szPageId = listPageIdEleId.at(0);
        QString szEleId = listPageIdEleId.at(1);
        if(currentGraphPage_ != nullptr) {
            if(currentGraphPage_->getGraphPageId() == szPageId) {
                Element *pEle = currentGraphPage_->getElement(szEleId);
                if(pEle != nullptr) {
                    pEle->hideElement();
                }
            }
        }
    }
}


/**
 * @brief MainWindow::showControlElement
 * @details 显示控件
 * @param eleId 控件ID
 */
void MainWindow::showControlElement(const QString &eleId)
{
    QStringList listPageIdEleId = eleId.split('.');
    if(listPageIdEleId.size() == 2) {
        QString szPageId = listPageIdEleId.at(0);
        QString szEleId = listPageIdEleId.at(1);
        if(currentGraphPage_ != nullptr) {
            if(currentGraphPage_->getGraphPageId() == szPageId) {
                Element *pEle = currentGraphPage_->getElement(szEleId);
                if(pEle != nullptr) {
                    pEle->showElement();
                }
            }
        }
    }
}


/**
 * @brief MainWindow::enableElement
 * @details 生效控件
 * @param eleId 控件ID
 */
void MainWindow::enableElement(const QString &eleId)
{
    QStringList listPageIdEleId = eleId.split('.');
    if(listPageIdEleId.size() == 2) {
        QString szPageId = listPageIdEleId.at(0);
        QString szEleId = listPageIdEleId.at(1);
        if(currentGraphPage_ != nullptr) {
            if(currentGraphPage_->getGraphPageId() == szPageId) {
                Element *pEle = currentGraphPage_->getElement(szEleId);
                if(pEle != nullptr) {
                    pEle->enableElement();
                }
            }
        }
    }
}


/**
 * @brief MainWindow::disableElement
 * @details 失效控件
 * @param eleId 控件ID
 */
void MainWindow::disableElement(const QString &eleId)
{
    QStringList listPageIdEleId = eleId.split('.');
    if(listPageIdEleId.size() == 2) {
        QString szPageId = listPageIdEleId.at(0);
        QString szEleId = listPageIdEleId.at(1);
        if(currentGraphPage_ != nullptr) {
            if(currentGraphPage_->getGraphPageId() == szPageId) {
                Element *pEle = currentGraphPage_->getElement(szEleId);
                if(pEle != nullptr) {
                    pEle->disableElement();
                }
            }
        }
    }
}


/**
 * @brief MainWindow::moveControlElement
 * @details 偏移控件
 * @param eleId 控件ID
 * @param x x偏移坐标
 * @param y y偏移坐标
 */
void MainWindow::moveControlElement(const QString &eleId, int x, int y)
{
    QStringList listPageIdEleId = eleId.split('.');
    if(listPageIdEleId.size() == 2) {
        QString szPageId = listPageIdEleId.at(0);
        QString szEleId = listPageIdEleId.at(1);
        if(currentGraphPage_ != nullptr) {
            if(currentGraphPage_->getGraphPageId() == szPageId) {
                Element *pEle = currentGraphPage_->getElement(szEleId);
                if(pEle != nullptr) {
                    pEle->moveTo(x, y);
                }
            }
        }
    }
}


/**
 * @brief MainWindow::startBlinkElement
 * @details 闪烁控件
 * @param eleId 控件ID
 */
void MainWindow::startBlinkElement(const QString &eleId)
{
    QStringList listPageIdEleId = eleId.split('.');
    if(listPageIdEleId.size() == 2) {
        QString szPageId = listPageIdEleId.at(0);
        QString szEleId = listPageIdEleId.at(1);
        if(currentGraphPage_ != nullptr) {
            if(currentGraphPage_->getGraphPageId() == szPageId) {
                Element *pEle = currentGraphPage_->getElement(szEleId);
                if(pEle != nullptr) {
                    pEle->startBlinkElement();
                }
            }
        }
    }
}


/**
 * @brief MainWindow::stopBlinkElement
 * @details 停止闪烁控件
 * @param eleId 控件ID
 */
void MainWindow::stopBlinkElement(const QString &eleId)
{
    QStringList listPageIdEleId = eleId.split('.');
    if(listPageIdEleId.size() == 2) {
        QString szPageId = listPageIdEleId.at(0);
        QString szEleId = listPageIdEleId.at(1);
        if(currentGraphPage_ != nullptr) {
            if(currentGraphPage_->getGraphPageId() == szPageId) {
                Element *pEle = currentGraphPage_->getElement(szEleId);
                if(pEle != nullptr) {
                    pEle->stopBlinkElement();
                }
            }
        }
    }
}







