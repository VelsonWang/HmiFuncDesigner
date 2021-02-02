#include "qdesignerwidget.h"
#include "widgetbox/qwidgetboxview.h"
#include "property/qpropertyview.h"
#include "form/qformwidgetview.h"
#include "form/formnamelistwidget.h"
#include "objectlist/qobjectlistwidget.h"
#include "../../libs/core/minisplitter.h"
#include "../../Public/userevent.h"
#include <QVBoxLayout>

QDesignerWidget::QDesignerWidget(QWidget *parent)
    : QAbstractPageWidget(parent),
      m_widgetBox(new QWidgetBoxView),
      m_propertyView(new QPropertyView),
      m_formWidgetView(new QFormWidgetView),
      m_objectListWidget(new QObjectListWidget),
      m_formNameListWidget(new FormNameListWidget)
{
    QWidget *pTopWidget = new QWidget();

    MiniSplitter *pSplitterObj = new MiniSplitter(Qt::Vertical);
    pSplitterObj->addWidget(m_formNameListWidget);
    pSplitterObj->addWidget(m_widgetBox);

    QVBoxLayout *pTopWidgetVLayoutObj = new QVBoxLayout;
    pTopWidgetVLayoutObj->setMargin(0);
    pTopWidgetVLayoutObj->setSpacing(0);
    pTopWidgetVLayoutObj->addWidget(pSplitterObj);
    pTopWidget->setLayout(pTopWidgetVLayoutObj);

    QVariant var;
    var.setValue(pTopWidget);
    this->setProperty("DesignerWidget", var);

    //--------------------------------------------------------------------------

    QVBoxLayout *pVLayoutObj = new QVBoxLayout;
    pVLayoutObj->setMargin(0);
    pVLayoutObj->setSpacing(0);

    MiniSplitter *splitter = new MiniSplitter(Qt::Horizontal);
    pVLayoutObj->addWidget(splitter);

    splitter->addWidget(m_formWidgetView);

    MiniSplitter *s = new MiniSplitter(Qt::Vertical);
    s->addWidget(m_propertyView);
    s->addWidget(m_objectListWidget);
    splitter->addWidget(s);
    splitter->setStretchFactor(0,1);
    splitter->setStretchFactor(1,0);

    connect(m_formWidgetView, SIGNAL(select(QAbstractHost*)),
            m_propertyView, SLOT(selectWidget(QAbstractHost*)));
    connect(m_formWidgetView, SIGNAL(select(QAbstractHost*)),
            m_objectListWidget, SLOT(set_select(QAbstractHost*)));
    connect(m_propertyView, SIGNAL(notifyPropertyEdit(QAbstractProperty*, QVariant)),
            m_formWidgetView,SIGNAL(notifyPropertyEdit(QAbstractProperty*, QVariant)));
    connect(m_objectListWidget, SIGNAL(select(QAbstractHost*)),
            m_formWidgetView, SLOT(set_select(QAbstractHost*)));
    connect(m_formNameListWidget, SIGNAL(notifySelectPage(QAbstractHost*)),
            m_formWidgetView, SLOT(set_select(QAbstractHost*)));
    connect(m_formNameListWidget, SIGNAL(notifyPropertyChange(QAbstractProperty*, const QVariant&)),
            m_formWidgetView,SIGNAL(notifyPropertyEdit(QAbstractProperty*, QVariant)));
    this->setLayout(pVLayoutObj);
}

void QDesignerWidget::setUndoStack(QUndoStack *stack)
{
    m_undo_stack = stack;
    m_formWidgetView->setUndoStack(m_undo_stack);
    m_propertyView->setUndoStack(m_undo_stack);
    m_objectListWidget->setUndoStack(m_undo_stack);
}


bool QDesignerWidget::event(QEvent *ev)
{
    if(ev->type() == UserEvent::EVT_USER_SHOW_UPDATE) {
        UserEvent *pEvObj = dynamic_cast<UserEvent *>(ev);
        if(pEvObj) {
            m_formNameListWidget->updateUI();
        }
        return true;
    } else if(ev->type() == UserEvent::EVT_USER_HIDE_UPDATE) {
        return true;
    }
    return QWidget::event(ev);
}
