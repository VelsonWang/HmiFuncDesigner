#include "qformwidgetview.h"

#include "qformlistwidget.h"
#include "qnewpagedialog.h"

#include "../../../libs/core/styledbar.h"
#include "../../../libs/core/qsoftcore.h"
#include "../../../libs/core/qactiontoolbar.h"

#include "../../../libs/shared/host/qformhost.h"
#include "../../../libs/shared/qhostfactory.h"
#include "../../../libs/shared/qprojectcore.h"
#include "../../../libs/shared/qpagemanager.h"
#include "../../../libs/shared/qlanguage.h"
#include "../../../libs/shared/qlanguagemanager.h"

#include <QVBoxLayout>
#include <QStringList>
#include <QPainter>

QFormWidgetView::QFormWidgetView(QWidget *parent) :
    QWidget(parent),
    m_formWidget(new QFormListWidget),
    m_styledBar(new StyledBar),
    m_languageComboBox(new QComboBox(this))
{
    this->setAutoFillBackground(false);
    QVBoxLayout *l=new QVBoxLayout;
    l->setMargin(0);
    l->setSpacing(0);

    l->addWidget(m_styledBar);
    l->addWidget(m_formWidget);

    this->setLayout(l);

    QSoftCore *core=QSoftCore::getCore();

    QActionToolBar *toolbar=new QActionToolBar;
    QList<QAction*> list;
    QAction *ac;

    ac=core->getAction("FormManager.Add");
    if(ac!=NULL)
    {
        list.append(ac);
        connect(ac,SIGNAL(triggered()),this,SLOT(add_page()));
        ac->setEnabled(false);
    }

    ac=new QAction(this);
    ac->setSeparator(true);
    list.append(ac);

    ac=core->getAction("FormManager.Same.Left");
    if(ac!=NULL)
    {
        list.append(ac);
        connect(ac,SIGNAL(triggered()),m_formWidget,SLOT(same_left()));
        ac->setEnabled(false);
    }

    ac=core->getAction("FormManager.Same.V-Center");
    if(ac!=NULL)
    {
        list.append(ac);
        connect(ac,SIGNAL(triggered()),m_formWidget,SLOT(same_v_centre()));
        ac->setEnabled(false);
    }

    ac=core->getAction("FormManager.Same.Right");
    if(ac!=NULL)
    {
        list.append(ac);
        connect(ac,SIGNAL(triggered()),m_formWidget,SLOT(same_right()));
        ac->setEnabled(false);
    }

    ac=core->getAction("FormManager.Same.Top");
    if(ac!=NULL)
    {
        list.append(ac);
        connect(ac,SIGNAL(triggered()),m_formWidget,SLOT(same_top()));
        ac->setEnabled(false);
    }

    ac=core->getAction("FormManager.Same.H-Center");
    if(ac!=NULL)
    {
        list.append(ac);
        connect(ac,SIGNAL(triggered()),m_formWidget,SLOT(same_h_centre()));
        ac->setEnabled(false);
    }


    ac=core->getAction("FormManager.Same.Bottom");
    if(ac!=NULL)
    {
        list.append(ac);
        connect(ac,SIGNAL(triggered()),m_formWidget,SLOT(same_bottom()));
        ac->setEnabled(false);
    }

    ac=new QAction(this);
    ac->setSeparator(true);
    list.append(ac);

    ac=core->getAction("FormManager.Same.Width");
    if(ac!=NULL)
    {
        list.append(ac);
        connect(ac,SIGNAL(triggered()),m_formWidget,SLOT(same_width()));
        ac->setEnabled(false);
    }

    ac=core->getAction("FormManager.Same.Height");
    if(ac!=NULL)
    {
        list.append(ac);
        connect(ac,SIGNAL(triggered()),m_formWidget,SLOT(same_height()));
        ac->setEnabled(false);
    }

    ac=core->getAction("FormManager.Same.Gemotry");
    if(ac!=NULL)
    {
        list.append(ac);
        connect(ac,SIGNAL(triggered()),m_formWidget,SLOT(same_geometry()));
        ac->setEnabled(false);
    }

    ac=new QAction(this);
    ac->setSeparator(true);
    list.append(ac);

    toolbar->addButtonActions(list);

    QHBoxLayout *h=new QHBoxLayout;

    m_languageComboBox->setFixedWidth(150);
    h->setMargin(0);
    h->setSpacing(0);
    h->addWidget(toolbar,1);
    h->addWidget(m_languageComboBox,0);
    m_styledBar->setLayout(h);

    connect(m_formWidget,SIGNAL(select(QAbstractHost*)),this,SIGNAL(select(QAbstractHost*)));
    connect(this,SIGNAL(property_edit_signal(QAbstractProperty*,QVariant)),m_formWidget,SLOT(property_edit_slot(QAbstractProperty*,QVariant)));
    connect(QSoftCore::getCore()->getProjectCore(),SIGNAL(opened_signals()),this,SLOT(project_opened()));
    connect(QSoftCore::getCore()->getProjectCore(),SIGNAL(closed_signals()),this,SLOT(project_closed()));
}

QFormWidgetView::~QFormWidgetView()
{
}

void QFormWidgetView::project_opened()
{
    QPageManager *manager=QSoftCore::getCore()->getProjectCore()->get_page_manager();
    QList<QAbstractHost*> pages=manager->getPages();

    foreach(QAbstractHost* h,pages)
    {
        h->getObject()->setProperty("no-ManhattanStyle",true);
        m_formWidget->insert_form(h);
    }
    if(pages.size()>0)
    {
        m_formWidget->show_form(pages.first());
    }

    disconnect(m_languageComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(language_combo_changed()));

    QLanguageManager *l_manager=QSoftCore::getCore()->getProjectCore()->getLanguageManager();

    QList<QLanguage*>  list=l_manager->get_all_languages();
    foreach(QLanguage*l ,list)
    {
        m_languageComboBox->addItem(l->get_language_name());
        m_language_uuid.append(l->getUuid());
    }
    QLanguage *c=l_manager->get_current_language();
    connect(m_languageComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(language_combo_changed()));
    if(c==NULL)
    {
        m_languageComboBox->setCurrentIndex(-1);
    }
    else
    {
        m_languageComboBox->setCurrentIndex(m_languageComboBox->findText(c->get_language_name()));
    }

    QSoftCore *core=QSoftCore::getCore();

    QAction *ac;

    ac=core->getAction("FormManager.Add");
    if(ac!=NULL)
    {
        ac->setEnabled(true);
    }

    ac=core->getAction("FormManager.Del");
    if(ac!=NULL)
    {
        ac->setEnabled(true);
    }

    ac=core->getAction("FormManager.Same.Left");
    if(ac!=NULL)
    {
        ac->setEnabled(true);
    }

    ac=core->getAction("FormManager.Same.V-Center");
    if(ac!=NULL)
    {
        ac->setEnabled(true);
    }

    ac=core->getAction("FormManager.Same.Right");
    if(ac!=NULL)
    {
        ac->setEnabled(true);
    }

    ac=core->getAction("FormManager.Same.Top");
    if(ac!=NULL)
    {
        ac->setEnabled(true);
    }

    ac=core->getAction("FormManager.Same.H-Center");
    if(ac!=NULL)
    {
        ac->setEnabled(true);
    }


    ac=core->getAction("FormManager.Same.Bottom");
    if(ac!=NULL)
    {
        ac->setEnabled(true);
    }

    ac=core->getAction("FormManager.Same.Width");
    if(ac!=NULL)
    {
        ac->setEnabled(true);
    }

    ac=core->getAction("FormManager.Same.Height");
    if(ac!=NULL)
    {
        ac->setEnabled(true);
    }

    ac=core->getAction("FormManager.Same.Gemotry");
    if(ac!=NULL)
    {
        ac->setEnabled(true);
    }
}

void QFormWidgetView::project_closed()
{
    //emit select(NULL);
    disconnect(m_languageComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(language_combo_changed()));
    m_languageComboBox->clear();
    m_language_uuid.clear();
    m_formWidget->clear();
    m_undo_stack->clear();

    QSoftCore *core=QSoftCore::getCore();

    QAction *ac;

    ac=core->getAction("FormManager.Add");
    if(ac!=NULL)
    {
        ac->setEnabled(false);
    }

    ac=core->getAction("FormManager.Del");
    if(ac!=NULL)
    {
        ac->setEnabled(false);
    }

    ac=core->getAction("FormManager.Same.Left");
    if(ac!=NULL)
    {
        ac->setEnabled(false);
    }

    ac=core->getAction("FormManager.Same.V-Center");
    if(ac!=NULL)
    {
        ac->setEnabled(false);
    }

    ac=core->getAction("FormManager.Same.Right");
    if(ac!=NULL)
    {
        ac->setEnabled(false);
    }

    ac=core->getAction("FormManager.Same.Top");
    if(ac!=NULL)
    {
        ac->setEnabled(false);
    }

    ac=core->getAction("FormManager.Same.H-Center");
    if(ac!=NULL)
    {
        ac->setEnabled(false);
    }


    ac=core->getAction("FormManager.Same.Bottom");
    if(ac!=NULL)
    {
        ac->setEnabled(false);
    }

    ac=core->getAction("FormManager.Same.Width");
    if(ac!=NULL)
    {
        ac->setEnabled(false);
    }

    ac=core->getAction("FormManager.Same.Height");
    if(ac!=NULL)
    {
        ac->setEnabled(false);
    }

    ac=core->getAction("FormManager.Same.Gemotry");
    if(ac!=NULL)
    {
        ac->setEnabled(false);
    }
}

void QFormWidgetView::add_page()
{
    QNewPageDialog dlg(m_undo_stack,this);
    dlg.exec();
}

void QFormWidgetView::set_undo_stack(QUndoStack *stack)
{
    m_undo_stack=stack;
    m_formWidget->set_undo_stack(stack);
}

void QFormWidgetView::language_combo_changed()
{
    int index=m_languageComboBox->currentIndex();
    QString uuid;
    if(index>=0 && index<m_language_uuid.size())
    {
        uuid=m_language_uuid.at(index);
    }
    else
    {
        uuid="";
    }
    QSoftCore::getCore()->getProjectCore()->getLanguageManager()->set_current_language(uuid);
}

void QFormWidgetView::set_select(QAbstractHost *host)
{
    m_formWidget->set_select(host);
}
