#include "qobjectlistwidget.h"
#include "qobjectlistview.h"
#include "../../../libs/core/styledbar.h"
#include <QVBoxLayout>
#include <QLabel>

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

    QLabel *label = new QLabel;
    label->setText(tr("Widget List"));
    label->setStyleSheet("color:rgb(255,255,255);");
    l = new QVBoxLayout;
    l->setMargin(5);
    l->setSpacing(0);
    l->addWidget(label);
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
