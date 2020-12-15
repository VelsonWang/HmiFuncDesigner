#include "qpropertyview.h"

#include "../../../libs/core/qpropertylist/qpropertylistview.h"
#include "../../../libs/shared/host/qabstracthost.h"
#include "../../../libs/core/styledbar.h"


#include <QVBoxLayout>
#include <QLabel>

QPropertyView::QPropertyView(QWidget *parent) :
    QWidget(parent),
    m_propertyView(new QPropertyListView(this)),
    m_styledBar(new StyledBar(this)),
    m_select(NULL)
{
    this->setMinimumSize(200,200);
    QVBoxLayout *l=new QVBoxLayout();
    l->setMargin(0);
    l->setSpacing(0);
    l->addWidget(m_styledBar);
    l->addWidget(m_propertyView);

    this->setLayout(l);

    QLabel* label=new QLabel(tr("Object Property"));
    label->setStyleSheet("color:rgb(255,255,255);");
    QVBoxLayout *layout=new QVBoxLayout;
    layout->setMargin(5);
    layout->addWidget(label);
    m_styledBar->setLayout(layout);

    this->resize(200,200);
    connect(m_propertyView,SIGNAL(property_edit_signal(QAbstractProperty*,QVariant)),this,SIGNAL(property_edit_signal(QAbstractProperty*,QVariant)));
}

void QPropertyView::selectWidget(QAbstractHost *host)
{
    if(m_select==host)
    {
        return;
    }
    m_select=host;
    if(host==NULL)
    {
        m_propertyView->setPropertys(QList<QAbstractProperty*>());
    }
    else
    {
        m_propertyView->setPropertys(host->getPropertys());
    }
}

void QPropertyView::set_undo_stack(QUndoStack *stack)
{
    m_propertyView->set_undo_stack(stack);
}
