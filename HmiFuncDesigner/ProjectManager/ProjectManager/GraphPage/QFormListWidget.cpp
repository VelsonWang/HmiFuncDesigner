#include "QFormListWidget.h"
#include "qdesignerformhost.h"
#include "QAbstractHost.h"
#include <QVBoxLayout>
#include <QVariant>
#include <QApplication>
#include <QClipboard>

QFormListWidget::QFormListWidget(QWidget *parent):
    QWidget(parent),
    m_current_form(Q_NULLPTR)
{
    QVBoxLayout *l = new QVBoxLayout(parent);
    l->setMargin(0);
    l->setSpacing(0);
    this->setLayout(l);

    this->setFocusPolicy(Qt::StrongFocus);


//    QSoftCore *core=QSoftCore::get_core();
//    QAction *ac;
//    ac=core->get_action("FormManager.Copy");
//    if(ac!=Q_NULLPTR)
//    {
//        ac->setShortcut(QKeySequence::Copy);
//        m_form_action_list.append(ac);
//    }

//    ac=core->get_action("FormManager.Cut");
//    if(ac!=Q_NULLPTR)
//    {
//        ac->setShortcut(QKeySequence::Cut);
//        m_form_action_list.append(ac);
//    }

//    ac=core->get_action("FormManager.Paste");
//    if(ac!=Q_NULLPTR)
//    {
//        ac->setShortcut(QKeySequence::Paste);
//        m_form_action_list.append(ac);
//    }

//    ac=core->get_action("FormManager.Delete");
//    if(ac!=Q_NULLPTR)
//    {
//        m_form_action_list.append(ac);
//        ac->setShortcut(QKeySequence::Delete);
//    }

//    ac=core->get_action("FormManager.Select.All");
//    if(ac!=Q_NULLPTR)
//    {
//        m_form_action_list.append(ac);
//        ac->setShortcut(QKeySequence::SelectAll);
//    }

//    QPageManager *manager=QSoftCore::get_core()->get_project_core()->get_page_manager();
//    connect(manager,SIGNAL(insert_page_signal(QAbstractHost*)),this,SLOT(insertPageSlot(QAbstractHost*)));
//    connect(manager,SIGNAL(remove_page_signal(QAbstractHost*)),this,SLOT(removePageSlot(QAbstractHost*)));
}

QFormListWidget::~QFormListWidget()
{
    clear();
}

void QFormListWidget::insertForm(QAbstractHost *host, int index)
{
    if(host==Q_NULLPTR || m_host_to_form.keys().contains(host) || !host->getObject()->isWidgetType())
    {
        return;
    }

    if(index<0 || index>m_forms.size())
    {
        index=m_forms.size();
    }
    QDesignerFormHost *form = new QDesignerFormHost(host,this);
    connect(form, SIGNAL(selectObj(QAbstractHost*)), this, SIGNAL(selectObj(QAbstractHost*)));
    form->setSelectWidgetObj(host);
    m_forms.insert(index,form);
    m_host_to_form.insert(host,form);

    setSelectObj(host);
    selectObj(host);
}

void QFormListWidget::removeForm(QAbstractHost *host)
{
    if(host==Q_NULLPTR || !m_host_to_form.keys().contains(host))
    {
        return;
    }
    if(m_current_form!=Q_NULLPTR && m_current_form->getRootHost()==host){
        if(m_forms.size()<2)
        {
            m_current_form=Q_NULLPTR;
            selectObj(Q_NULLPTR);
        }
        else
        {
            if(m_forms.first()==m_current_form)
            {
                setSelectObj(m_forms.at(1)->getRootHost());
                selectObj(m_forms.at(1)->getRootHost());
            }
            else
            {
                setSelectObj(m_forms.at(0)->getRootHost());
                selectObj(m_forms.at(0)->getRootHost());
            }
        }
    }
    QDesignerFormHost *form=m_host_to_form.value(host);
    m_host_to_form.remove(host);
    if(form!=NULL)
    {
        m_forms.removeAll(form);
        delete form;
    }
}

void QFormListWidget::showForm(QAbstractHost *host)
{
    QDesignerFormHost *form=m_host_to_form.value(host);
    if(m_current_form==form)
    {
        return;
    }
    if(m_current_form!=Q_NULLPTR)
    {
        m_current_form->hide();
    }
    m_current_form=form;
    if(m_current_form!=Q_NULLPTR)
    {
        m_current_form->show();
    }
}

void QFormListWidget::updateGeometry(QAbstractHost* host)
{
    QDesignerFormHost *pObj = m_host_to_form.value(host);
    if(pObj != Q_NULLPTR) {
        pObj->updateGeometry();
    }

}

void QFormListWidget::clear()
{
    qDeleteAll(m_forms);
    m_forms.clear();
    m_host_to_form.clear();
    m_current_form=Q_NULLPTR;
}


void QFormListWidget::sameLeft()
{
    if(m_current_form!=Q_NULLPTR)
    {
        m_current_form->sameLeft();
    }
}

void QFormListWidget::sameTop()
{
    if(m_current_form!=Q_NULLPTR)
    {
        m_current_form->sameTop();
    }
}


void QFormListWidget::sameRight()
{
    if(m_current_form!=Q_NULLPTR)
    {
        m_current_form->sameRight();
    }
}


void QFormListWidget::sameBottom()
{
    if(m_current_form!=Q_NULLPTR)
    {
        m_current_form->sameBottom();
    }
}


void QFormListWidget::sameWidth()
{
    if(m_current_form!=Q_NULLPTR)
    {
        m_current_form->sameWidth();
    }
}


void QFormListWidget::sameHeight()
{
    if(m_current_form!=Q_NULLPTR)
    {
        m_current_form->sameHeight();
    }
}


void QFormListWidget::sameGeometry()
{
    if(m_current_form!=Q_NULLPTR)
    {
        m_current_form->sameGeometry();
    }
}


void QFormListWidget::sameVCenter()
{
    if(m_current_form!=Q_NULLPTR)
    {
        m_current_form->sameVCenter();
    }
}


void QFormListWidget::sameHCenter()
{
    if(m_current_form!=Q_NULLPTR)
    {
        m_current_form->sameHCenter();
    }
}

void QFormListWidget::setSelectObj(QAbstractHost *host)
{
    QAbstractHost *par=host;
    while(par->getParent()!=Q_NULLPTR)
    {
        par=par->getParent();
    }

    if(m_current_form!=Q_NULLPTR)
    {
        if(m_current_form->getRootHost()==par)
        {
            m_current_form->setSelectWidgetObj(host);
        }
        else
        {
            showForm(par);
            if(m_current_form!=Q_NULLPTR)
            {
                m_current_form->setSelectWidgetObj(host);
            }
        }
    }
    else
    {
        showForm(par);
        if(m_current_form!=Q_NULLPTR)
        {
            m_current_form->setSelectWidgetObj(host);
        }
    }
}

void QFormListWidget::insertPageSlot(QAbstractHost *host)
{
//    QPageManager *manager=QSoftCore::get_core()->get_project_core()->get_page_manager();

//    int index=manager->get_pages().indexOf(host);
//    insertForm(host,index);
}

void QFormListWidget::removePageSlot(QAbstractHost *host)
{
    removeForm(host);
}
