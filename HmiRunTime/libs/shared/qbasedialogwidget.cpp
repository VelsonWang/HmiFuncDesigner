#include "qbasedialogwidget.h"

#include <QPainter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMouseEvent>

QBaseDialogWidget::QBaseDialogWidget(QWidget *parent) :
    QWidget(parent)
{
}

void QBaseDialogWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.fillRect(this->rect(), QColor(0, 0, 0, 60));
}

void QBaseDialogWidget::addWidget(QWidget *wid)
{
    wid->setFixedSize(wid->size());
    wid->setParent(this);
    QVBoxLayout *v = new QVBoxLayout;
    QHBoxLayout *l = new QHBoxLayout;
    v->setMargin(0);
    v->setSpacing(0);
    l->setMargin(0);
    l->setSpacing(0);
    l->addWidget(wid);
    v->addLayout(l);
    this->setLayout(v);
    m_widget = wid;
}

QWidget* QBaseDialogWidget::getWidget()
{
    return m_widget;
}
