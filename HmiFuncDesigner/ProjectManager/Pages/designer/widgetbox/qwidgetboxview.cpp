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
    QVBoxLayout *l = new QVBoxLayout;
    l->setMargin(0);
    l->setSpacing(0);

    l->addWidget(m_styledBar);
    l->addWidget(m_widgetBox);

    this->setLayout(l);

    this->resize(100,100);

    QLabel *pLabelObj = new QLabel;
    pLabelObj->setText(tr("工具箱"));
    pLabelObj->setFrameShape(QFrame::StyledPanel);
    l = new QVBoxLayout;
    l->setMargin(0);
    l->setSpacing(0);
    l->addWidget(pLabelObj);
    m_styledBar->setLayout(l);
}
