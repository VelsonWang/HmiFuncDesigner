#include "qformhost.h"

#include <QWidget>

QFormHost::QFormHost(QAbstractHost *parent):
    QAbstractWidgetHost(parent)
{
    setProperty("title", FORM_TITLE);
}

void QFormHost::createObject()
{
    m_object = new QWidget();
}

