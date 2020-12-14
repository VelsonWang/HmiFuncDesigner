#include "qwidgetboxview.h"

#include "widgetboxtreewidget.h"

#include "../../../libs/core/styledbar.h"

#include <QVBoxLayout>
#include <QLabel>

QWidgetBoxView::QWidgetBoxView(QWidget *parent) :
    QWidget(parent),
    m_widgetBox(new WidgetBoxTreeWidget),
    m_styledBar(new StyledBar)
{
    this->setMinimumSize(100,100);
    QVBoxLayout *l=new QVBoxLayout;
    l->setMargin(0);
    l->setSpacing(0);

    l->addWidget(m_styledBar);
    l->addWidget(m_widgetBox);

    this->setLayout(l);

    this->resize(100,100);

    QLabel *label=new QLabel;
    label->setText(tr("Widget Box"));
    label->setStyleSheet("color:rgb(255,255,255);");
    l=new QVBoxLayout;
    l->setMargin(5);
    l->setSpacing(0);
    l->addWidget(label);
    m_styledBar->setLayout(l);
}
