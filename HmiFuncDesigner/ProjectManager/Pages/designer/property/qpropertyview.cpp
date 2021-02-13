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
    m_select(Q_NULLPTR)
{
    this->setMinimumSize(360, 200);
    QVBoxLayout *l = new QVBoxLayout();
    l->setMargin(0);
    l->setSpacing(0);
    l->addWidget(m_styledBar);
    l->addWidget(m_propertyView);

    this->setLayout(l);

    QLabel *pLabelObj = new QLabel;
    pLabelObj->setText(tr("对象属性"));
    pLabelObj->setFrameShape(QFrame::StyledPanel);
    QVBoxLayout *layout=new QVBoxLayout;
    layout->setMargin(0);
    layout->addWidget(pLabelObj);
    m_styledBar->setLayout(layout);

    this->resize(360, 200);
    connect(m_propertyView, SIGNAL(notifyPropertyEdit(QAbstractProperty*, QVariant)),
            this, SIGNAL(notifyPropertyEdit(QAbstractProperty*, QVariant)));
}

void QPropertyView::selectWidget(QAbstractHost *host)
{
    if(m_select == host) {
        return;
    }
    m_select = host;
    if(host == Q_NULLPTR) {
        m_propertyView->setPropertys(QList<QAbstractProperty*>());
    } else {
        m_propertyView->setPropertys(host->getPropertys());
    }
}

void QPropertyView::setUndoStack(QUndoStack *stack)
{
    m_propertyView->setUndoStack(stack);
}
