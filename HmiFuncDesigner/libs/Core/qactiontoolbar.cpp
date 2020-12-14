#include "qactiontoolbar.h"

#include "qtoolbarbutton.h"

#include <QHBoxLayout>

QActionToolBar::QActionToolBar(Qt::Alignment align, QWidget *parent):
    QWidget(parent),
    m_align(align)
{
    QHBoxLayout *l=new QHBoxLayout();
    l->setContentsMargins(5,0,5,0);//两头空出来
    l->setSpacing(1);
    this->setLayout(l);
    m_emptyWidget=new QWidget(this);
    l->addWidget(m_emptyWidget);
}

void QActionToolBar::addButtonActions(const QList<QAction *> &actions)
{
    qDeleteAll(m_buttons);
    m_buttons.clear();
    QToolBarButton *button;
    QLayout *l=this->layout();
    l->removeWidget(m_emptyWidget);
    if(m_align==Qt::AlignRight)
    {
        l->addWidget(m_emptyWidget);
    }
    for(int i=0;i<actions.size();i++)
    {
        button=new QToolBarButton(actions.at(i),this);
        l->addWidget(button);
        m_buttons.append(button);//添加按钮
    }
    if(m_align==Qt::AlignLeft)
    {
        l->addWidget(m_emptyWidget);
    }
    addActions(actions);
}

void QActionToolBar::setLabelWidget(QWidget *widget)
{
    QHBoxLayout *l=new QHBoxLayout;
    l->setMargin(0);
    l->setSpacing(0);
    l->addWidget(widget);
    m_emptyWidget->setLayout(l);//把扩展控件布局到载体中去
}
