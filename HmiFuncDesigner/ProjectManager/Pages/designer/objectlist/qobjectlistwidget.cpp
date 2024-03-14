#include "qobjectlistwidget.h"
#include "qobjectlistview.h"
#include "../../../libs/core/styledbar.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QDebug>

QObjectListWidget::QObjectListWidget(QWidget *parent)
    : QWidget(parent),
      m_styledBar(new StyledBar),
      m_object_list(new QObjectListView)
{
    this->setMinimumSize(100, 100);
    QVBoxLayout *l = new QVBoxLayout;
    l->setMargin(0);
    l->setSpacing(0);
    l->addWidget(m_styledBar);
    l->addWidget(m_object_list);
    this->setLayout(l);

    this->resize(100, 100);

    QLabel *pLabelObj = new QLabel;
    pLabelObj->setText(tr("画面对象列表"));
    pLabelObj->setFrameShape(QFrame::StyledPanel);
    l = new QVBoxLayout;
    l->setMargin(0);
    l->setSpacing(0);
    l->addWidget(pLabelObj);
    m_styledBar->setLayout(l);

    connect(m_object_list, SIGNAL(select(QAbstractHost*)), this, SIGNAL(select(QAbstractHost*)));
}

void QObjectListWidget::set_select(QAbstractHost *host)
{
    m_object_list->set_select(host);
}

void QObjectListWidget::setUndoStack(QUndoStack *stack)
{
    m_object_list->setUndoStack(stack);
}

void QObjectListWidget::remove(QList<QAbstractHost*> hosts)
{
    //qDebug() << __FILE__ << __LINE__ << __FUNCTION__;
    if(hosts.size() > 0) {
        m_object_list->remove(hosts);
    }
}
