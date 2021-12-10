#include "qrunningmanager.h"
#include "qbasedialogwidget.h"
#include "qbasedialog.h"
#include "../shared/qprojectcore.h"
#include "../shared/host/qabstracthost.h"
#include "../shared/host/qprojecthost.h"
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

    m_pProjCoreObj->initScriptEngine();
    connect(m_pProjCoreObj->getProjectHost(), SIGNAL(notifyShowWidget(QWidget*)),
            this, SLOT(onShowWidget(QWidget*)));
    connect(m_pProjCoreObj->getProjectHost(), SIGNAL(notifyShowDialog(QAbstractHost*)),
            this, SLOT(onShowDialog(QAbstractHost*)));

    return true;
}

void QRunningManager::release()
{
    if(m_pProjCoreObj) {
        delete m_pProjCoreObj;
        m_pProjCoreObj = NULL;
    }
    m_pLastWidgetObj = NULL;
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
}

void QRunningManager::onShowDialog(QAbstractHost *host)
{
    m_pDlgBaseWidgetObj->setVisible(true);
    m_pDlgBaseWidgetObj->raise();
    QWidget* pWidgetObj = (QWidget*)host->getObject();
    QBaseDialog dlg(m_pMainWindowObj);
    dlg.set_widget(pWidgetObj);
    dlg.exec();
    pWidgetObj->setParent(NULL);
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
    QProjectHost* host = (QProjectHost*)m_pProjCoreObj->getProjectHost();
    QString szStartPage = m_pProjCoreObj->m_projInfoMgr.getStartPage();
    QPageManager *manager = m_pProjCoreObj->getPageManager();
    QList<QAbstractHost*> list = manager->getPages();
    foreach(QAbstractHost* page, list) {
        ((QWidget*)page->getObject())->setVisible(false);
    }
    int width = m_pProjCoreObj->m_projInfoMgr.getGraphPageWidth();
    int height = m_pProjCoreObj->m_projInfoMgr.getGraphPageHeight();
    m_pMainWindowObj->setFixedSize(QSize(width, height));
    m_pMainWindowObj->setWindowTitle(host->getPropertyValue("objectName").toString());
    QDesktopWidget *pDeskObj = qApp->desktop();
    m_pMainWindowObj->move((pDeskObj->width() - width) / 2, (pDeskObj->height() - height) / 2);
    if(szStartPage == "None") {
        host->showFirstForm();
    } else {
        host->show_form_by_uuid(szStartPage);
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
