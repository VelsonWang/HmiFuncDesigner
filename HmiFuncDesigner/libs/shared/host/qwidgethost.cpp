#include "qwidgethost.h"

#include <QWidget>

QWidgetHost::QWidgetHost(QAbstractHost *parent):
    QAbstractWidgetHost(parent)
{
    this->setProperty("need_frame",true);
    setProperty("accept_drop",true);
}

QString QWidgetHost::get_show_name()
{
    return tr("Widget");
}

QString QWidgetHost::get_show_group()
{
    return tr("Containners");
}

QString QWidgetHost::get_show_icon()
{
    return ":/images/widget.png";
}

void QWidgetHost::create_object()
{
    m_object=new QWidget();
    m_object->setObjectName("widget");
}

void QWidgetHost::setVisible(bool visible)
{
    m_object->setProperty("visible",visible);
}

bool QWidgetHost::visible()
{
    return m_object->property("visible").toBool();
}

void QWidgetHost::set_focus()
{
    QWidget* wid=(QWidget*)m_object;
    wid->setFocus();
}

void QWidgetHost::kill_focus()
{
    QWidget* wid=(QWidget*)m_object;
    wid->clearFocus();
}
