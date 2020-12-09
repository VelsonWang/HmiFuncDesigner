#include "QWidgetHost.h"

#include <QWidget>

QWidgetHost::QWidgetHost(QAbstractHost *parent):
    QAbstractWidgetHost(parent)
{
    this->setProperty("need_frame",true);
    setProperty("accept_drop",true);
}

QString QWidgetHost::getShowName()
{
    return tr("Widget");
}

QString QWidgetHost::getShowGroup()
{
    return tr("Containners");
}

QString QWidgetHost::getShowIcon()
{
    return ":/images/widget.png";
}

void QWidgetHost::createObject()
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

void QWidgetHost::setFocus()
{
    QWidget* wid=(QWidget*)m_object;
    wid->setFocus();
}

void QWidgetHost::killFocus()
{
    QWidget* wid=(QWidget*)m_object;
    wid->clearFocus();
}
