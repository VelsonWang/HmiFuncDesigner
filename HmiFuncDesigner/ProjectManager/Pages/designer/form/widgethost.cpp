#include "widgethost.h"

#include "formresizer.h"

#include <QPainter>

WidgetHost::WidgetHost(QWidget *parent):
    QScrollArea(parent),
    m_formResizer(new FormResizer(this))
{
    setWidget(m_formResizer);
    setFrameStyle(QFrame::NoFrame);
    connect(m_formResizer,SIGNAL(size_changed(QRect,QRect)),this,SIGNAL(size_changed(QRect,QRect)));
}

WidgetHost::~WidgetHost()
{
}

void WidgetHost::showHandle()
{
    m_formResizer->showHandle();
}

void WidgetHost::hideHandle()
{
    m_formResizer->hideHandle();
}

bool WidgetHost::isHandleVisible()
{
    return m_formResizer->isHandleVisible();
}

void WidgetHost::setFormWidget(QWidget *widget)
{
    this->setFocusProxy(widget);
    m_formwidget=widget;
    m_formResizer->setWidget(widget);
}

QWidget *WidgetHost::formWidget()
{
    return m_formwidget;
}

void WidgetHost::updateFormGeometry()
{
    m_formResizer->updateFormGeometry();
}
