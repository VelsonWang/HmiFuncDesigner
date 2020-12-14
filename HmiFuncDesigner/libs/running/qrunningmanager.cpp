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

QRunningManager::QRunningManager(QObject *parent) :
    QObject(parent),
    m_project(NULL),
    m_main_window(new QMainWindow((QWidget*)parent)),
    m_dialog_base_widget(new QBaseDialogWidget(m_main_window)),
    m_last_widget(NULL)
{
    m_dialog_base_widget->setVisible(false);
    m_main_window->installEventFilter(this);
}

QRunningManager::~QRunningManager()
{
    release();
    m_main_window->removeEventFilter(this);
    delete m_main_window;
}

bool QRunningManager::load(const QString &path)
{
    release();

    m_project=new QProjectCore;

    if(!m_project->open(path+"/config.sfb"))
    {
        return false;
    }
    m_project->init_script_engine();
    connect(m_project->get_project_host(),SIGNAL(show_widget(QWidget*)),this,SLOT(show_widget(QWidget*)));
    connect(m_project->get_project_host(),SIGNAL(show_dialog(QAbstractHost*)),this,SLOT(show_dialog(QAbstractHost*)));
    return true;
}

void QRunningManager::release()
{
    if(m_project!=NULL)
    {
        delete m_project;
        m_project=NULL;
    }
    m_last_widget=NULL;
}

void QRunningManager::show_widget(QWidget *widget)
{
    if(widget==NULL)
    {
        return;
    }
    if(widget->parent()!=m_main_window)
    {
        widget->setParent(m_main_window);
    }

    if(m_last_widget!=NULL)
    {
        m_last_widget->setVisible(false);
    }
    m_last_widget=widget;
    widget->show();
    widget->raise();
}

void QRunningManager::show_dialog(QAbstractHost *host)
{
    m_dialog_base_widget->setVisible(true);
    m_dialog_base_widget->raise();

    QWidget* wid=(QWidget*)host->get_object();

    QBaseDialog dlg(m_main_window);

    dlg.set_widget(wid);

    dlg.exec();

    wid->setParent(NULL);
}

bool QRunningManager::eventFilter(QObject *o, QEvent *e)
{
    if(o==m_main_window && e->type()==QEvent::Close)
    {
        stop();
        return true;
    }
    return false;
}

void QRunningManager::start()
{
    QProjectHost* host=(QProjectHost*)m_project->get_project_host();
    QString start_page=host->get_property_value("start_page").toString();

    QPageManager *manager=m_project->get_page_manager();
    QList<QAbstractHost*> list=manager->getPages();
    foreach(QAbstractHost* page,list)
    {
        ((QWidget*)page->get_object())->setVisible(false);
    }

    QSize sz=host->get_property_value("running_size").toSize();
    m_main_window->setFixedSize(sz);

    m_main_window->setWindowTitle(host->get_property_value("objectName").toString());

    QDesktopWidget *desk=qApp->desktop();

    m_main_window->move((desk->width()-sz.width())/2,(desk->height()-sz.height())/2);

    host->show_form_by_uuid(start_page);


    m_main_window->setVisible(true);

    emit start_signal();
}

void QRunningManager::stop()
{
    emit stop_signal();
    release();
    m_main_window->setVisible(false);
}
