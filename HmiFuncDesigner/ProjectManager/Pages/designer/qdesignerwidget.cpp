#include "qdesignerwidget.h"

#include "widgetbox/qwidgetboxview.h"

#include "property/qpropertyview.h"
#include "form/qformwidgetview.h"
#include "objectlist/qobjectlistwidget.h"

#include "../../libs/core/minisplitter.h"

#include <QVBoxLayout>

QDesignerWidget::QDesignerWidget(QWidget *parent) :
    QAbstractPageWidget(parent),
    m_widgetBox(new QWidgetBoxView),
    m_propertyView(new QPropertyView),
    m_formWidgetView(new QFormWidgetView),
    m_objectListWidget(new QObjectListWidget)
{
    QVBoxLayout *l=new QVBoxLayout;

    l->setMargin(0);
    l->setSpacing(0);



    MiniSplitter *splitter=new MiniSplitter(Qt::Horizontal);

    l->addWidget(splitter);


    splitter->addWidget(m_widgetBox);
    splitter->addWidget(m_formWidgetView);

    MiniSplitter *s=new MiniSplitter(Qt::Vertical);
    s->addWidget(m_propertyView);
    s->addWidget(m_objectListWidget);
    splitter->addWidget(s);

    splitter->setStretchFactor(0,0);
    splitter->setStretchFactor(1,1);
    splitter->setStretchFactor(2,0);


    connect(m_formWidgetView,SIGNAL(select(QAbstractHost*)),m_propertyView,SLOT(selectWidget(QAbstractHost*)));
    connect(m_formWidgetView,SIGNAL(select(QAbstractHost*)),m_objectListWidget,SLOT(set_select(QAbstractHost*)));
    connect(m_propertyView,SIGNAL(property_edit_signal(QAbstractProperty*,QVariant)),m_formWidgetView,SIGNAL(property_edit_signal(QAbstractProperty*,QVariant)));
    connect(m_objectListWidget,SIGNAL(select(QAbstractHost*)),m_formWidgetView,SLOT(set_select(QAbstractHost*)));
    this->setLayout(l);
}

void QDesignerWidget::set_undo_stack(QUndoStack *stack)
{
    m_undo_stack=stack;
    m_formWidgetView->set_undo_stack(m_undo_stack);
    m_propertyView->set_undo_stack(m_undo_stack);
    m_objectListWidget->set_undo_stack(m_undo_stack);
}
