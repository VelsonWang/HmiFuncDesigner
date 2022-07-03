#include "qrunningmanager.h"
#include "qbasedialogwidget.h"
#include "qbasedialog.h"
#include "../shared/qprojectcore.h"
#include "../shared/qpagemanager.h"
#include <QEvent>
#include <QDesktopWidget>
#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>

QRunningManager::QRunningManager(QObject *parent) :
    QObject(parent),
    m_pProjCoreObj(new QProjectCore),
    m_pMainWindowObj(new QMainWindow((QWidget*)parent)),
    m_pDlgBaseWidgetObj(new QBaseDialogWidget(m_pMainWindowObj)),
    m_pLastWidgetObj(NULL)
{
    m_pDlgBaseWidgetObj->setVisible(false);
    m_pMainWindowObj->installEventFilter(this);
    connect(&m_timer, SIGNAL(timeout()), m_pMainWindowObj, SLOT(update()));
}

QRunningManager::~QRunningManager()
{
    release();
    if(m_pMainWindowObj) {
        m_pMainWindowObj->removeEventFilter(this);
        delete m_pMainWindowObj;
        m_pMainWindowObj = NULL;
    }
}


bool QRunningManager::load(QString proj)
{
    QFile fileProj(proj);
    if(!fileProj.exists()) {
        QMessageBox::information(NULL, tr("系统提示"), tr("工程：") + proj + tr("不存在！"));
        qDebug() << "project file not exists!";
        return false;
    }

    ////////////////////////////////////////////////////////////////////////////

    QFileInfo fileInfoSrc(proj);
    if(fileInfoSrc.size() <= 512) {
        QMessageBox::information(NULL, tr("系统提示"), tr("读取工程数据出错，文件头数据有误！"));
        return false;
    }

    // 读取文件头信息
    if(!fileProj.open(QIODevice::ReadOnly)) {
        QMessageBox::information(NULL, tr("系统提示"), tr("打开工程：") + proj + tr("失败！"));
        return false;
    }

    quint8 buf[1024] = {0};
    quint16 wSize = (sizeof(TFileHeader) < 512) ? 512 : sizeof(TFileHeader);

    if(fileProj.read((char *)buf, wSize) != wSize) {
        fileProj.close();
        QMessageBox::information(NULL, tr("系统提示"), tr("读取文件头数据出错！"));
        return false;
    }
    fileProj.close();

    TFileHeader headerObj;
    memcpy((void *)&headerObj, (void *)buf, sizeof(TFileHeader));

    if(headerObj.byOpenVerifyPassword != 0) {
        QByteArray baPwd;
        baPwd.append((const char *)headerObj.szPassword, headerObj.byOpenVerifyPassword);
        //        VerifyPasswordDialog dlg;
        //        dlg.setTargetPassword(baPwd.toHex());
        //        if(dlg.exec() == QDialog::Rejected) {
        //            return;
        //        }
    }

    if(!m_pProjCoreObj->openFromXml(proj)) {
        return false;
    }

    //    connect(m_pProjCoreObj->getProjectHost(), SIGNAL(notifyShowWidget(QWidget*)),
    //            this, SLOT(onShowWidget(QWidget*)));
    //    connect(m_pProjCoreObj->getProjectHost(), SIGNAL(notifyShowDialog(QAbstractHost*)),
    //            this, SLOT(onShowDialog(QAbstractHost*)));

    return true;
}

void QRunningManager::release()
{
    if(m_pProjCoreObj) {
        delete m_pProjCoreObj;
        m_pProjCoreObj = NULL;
    }
    m_pLastWidgetObj = NULL;
    m_stackShowWidget.clear();
}

void QRunningManager::onShowWidget(QWidget *widget)
{
    if(widget == NULL) {
        return;
    }
    if(widget->parent() != m_pMainWindowObj) {
        int width = qMax(widget->width(), m_pMainWindowObj->width());
        int height = qMax(widget->height(), m_pMainWindowObj->height());
        m_pMainWindowObj->setFixedSize(QSize(width, height));
        widget->setParent(m_pMainWindowObj);
        QDesktopWidget *pDeskObj = qApp->desktop();
        m_pMainWindowObj->move((pDeskObj->width() - width) / 2, (pDeskObj->height() - height) / 2);
    }
    if(m_pLastWidgetObj != NULL) {
        m_pLastWidgetObj->setVisible(false);
    }
    m_pLastWidgetObj = widget;
    widget->show();
    widget->raise();
    if(m_stackShowWidget.size() > 256) {
        m_stackShowWidget.pop_back();
    }
}

void QRunningManager::onShowDialog(QWidget* widget)
{
    m_pDlgBaseWidgetObj->setVisible(true);
    m_pDlgBaseWidgetObj->raise();
    QBaseDialog dlg(m_pMainWindowObj);
    dlg.set_widget(widget);
    dlg.exec();
    widget->setParent(NULL);
}

bool QRunningManager::eventFilter(QObject *o, QEvent *e)
{
    if(o == m_pMainWindowObj && e->type() == QEvent::Close) {
        stop();
        return true;
    }
    return false;
}

void QRunningManager::start()
{
    QString szStartPage = m_pProjCoreObj->m_projInfoMgr.getStartPage();
    QPageManager *pageMgr = m_pProjCoreObj->getPageManager();
    QList<QWidget*> list = pageMgr->getPages();
    foreach(QWidget* pObj, list) {
        pObj->setVisible(false);
    }
    int width = m_pProjCoreObj->m_projInfoMgr.getGraphPageWidth();
    int height = m_pProjCoreObj->m_projInfoMgr.getGraphPageHeight();
    m_pMainWindowObj->setFixedSize(QSize(width, height));
    QDesktopWidget *pDeskObj = qApp->desktop();
    m_pMainWindowObj->move((pDeskObj->width() - width) / 2, (pDeskObj->height() - height) / 2);
    if(szStartPage == "None") {
        if(pageMgr->pageCount() > 0) {
            m_stackShowWidget.push(pageMgr->getPage(0));
            onShowWidget(pageMgr->getPage(0));
        }
    } else {
        QWidget* pWidgetObj = pageMgr->getPage(szStartPage);
        if(pWidgetObj) {
            m_stackShowWidget.push(pWidgetObj);
            onShowWidget(pWidgetObj);
        }
    }

    m_pMainWindowObj->setVisible(true);

    // 开启定时画面刷新
    int pageScanPeriod = m_pProjCoreObj->m_projInfoMgr.getDataScanPeriod();
    if(pageScanPeriod < 100) {
        pageScanPeriod = 100;
    }
    m_timer.start(pageScanPeriod);

    emit notifyStart();
}

void QRunningManager::stop()
{
    emit notifyStop();
    release();
    m_pMainWindowObj->setVisible(false);
}

void QRunningManager::showGraphPage(const QString &pageId)
{
    QPageManager *pageMgr = m_pProjCoreObj->getPageManager();
    QWidget* pWidgetObj = pageMgr->getPage(pageId);
    if(pWidgetObj) {
        m_stackShowWidget.push(pWidgetObj);
        onShowWidget(pWidgetObj);
    } else {
        qDebug() << "can't find page with id: " << pageId;
    }
}

void QRunningManager::showLastGraphPage()
{
    if(m_stackShowWidget.size() > 0) {
        QWidget *pWidgetObj = m_stackShowWidget.pop();
        onShowWidget(pWidgetObj);
    } else {
        qDebug() << "m_stackShowWidget is empty!";
    }

}

void QRunningManager::showControlElement(const QString &eleId)
{
    QPageManager *pageMgr = m_pProjCoreObj->getPageManager();
    QWidget* pWidgetObj = pageMgr->getWidget(eleId);
    if(pWidgetObj) {
        pWidgetObj->show();
    } else {
        qDebug() << "can't find control element with id: " << eleId;
    }
}

void QRunningManager::hideControlElement(const QString &eleId)
{
    QPageManager *pageMgr = m_pProjCoreObj->getPageManager();
    QWidget* pWidgetObj = pageMgr->getWidget(eleId);
    if(pWidgetObj) {
        pWidgetObj->hide();
    } else {
        qDebug() << "can't find control element with id: " << eleId;
    }
}

void QRunningManager::enableControlElement(const QString &eleId)
{
    QPageManager *pageMgr = m_pProjCoreObj->getPageManager();
    QWidget* pWidgetObj = pageMgr->getWidget(eleId);
    if(pWidgetObj) {
        pWidgetObj->setEnabled(true);
    } else {
        qDebug() << "can't find control element with id: " << eleId;
    }
}

void QRunningManager::disableControlElement(const QString &eleId)
{
    QPageManager *pageMgr = m_pProjCoreObj->getPageManager();
    QWidget* pWidgetObj = pageMgr->getWidget(eleId);
    if(pWidgetObj) {
        pWidgetObj->setEnabled(false);
    } else {
        qDebug() << "can't find control element with id: " << eleId;
    }
}

void QRunningManager::moveControlElement(const QString &eleId, int x, int y)
{
    QPageManager *pageMgr = m_pProjCoreObj->getPageManager();
    QWidget* pWidgetObj = pageMgr->getWidget(eleId);
    if(pWidgetObj) {
        QPoint pos = pWidgetObj->pos();
        pWidgetObj->move(pos.x() + x, pos.y() + y);
    } else {
        qDebug() << "can't find control element with id: " << eleId;
    }
}

void QRunningManager::blinkControlElement(const QString &eleId)
{
    QPageManager *pageMgr = m_pProjCoreObj->getPageManager();
    QWidget* pWidgetObj = pageMgr->getWidget(eleId);
    if(pWidgetObj) {
        m_blinkTimer.addWidget(pWidgetObj);
    } else {
        qDebug() << "can't find control element with id: " << eleId;
    }
}
void QRunningManager::stopBlinkControlElement(const QString &eleId)
{
    QPageManager *pageMgr = m_pProjCoreObj->getPageManager();
    QWidget* pWidgetObj = pageMgr->getWidget(eleId);
    if(pWidgetObj) {
        m_blinkTimer.removeWidget(pWidgetObj);
    } else {
        qDebug() << "can't find control element with id: " << eleId;
    }
}


extern "C" {

void rtSwitchGraphPage(char *pageId)
{
    QRunningManager::instance()->showGraphPage(pageId);
}

void rtReturnGraphPage()
{
    QRunningManager::instance()->showLastGraphPage();
}

void rtShowControlElement(char *eleId)
{
    QRunningManager::instance()->showControlElement(eleId);
}

void rtHideControlElement(char *eleId)
{
    QRunningManager::instance()->hideControlElement(eleId);
}

void rtEnableControlElement(char *eleId)
{
    QRunningManager::instance()->enableControlElement(eleId);
}

void rtDisableControlElement(char *eleId)
{
    QRunningManager::instance()->disableControlElement(eleId);
}

void rtMoveControlElement(char *eleId, int x, int y)
{
    QRunningManager::instance()->moveControlElement(eleId, x, y);
}

void rtBlinkControlElement(char *eleId)
{
    QRunningManager::instance()->blinkControlElement(eleId);
}

void rtStopBlinkControlElement(char *eleId)
{
    QRunningManager::instance()->stopBlinkControlElement(eleId);
}

}
