#include "QAbstractButtonHost.h"

#include <QAbstractButton>

QAbstractButtonHost::QAbstractButtonHost(QAbstractHost *parent):
    QWidgetHost(parent)
{
}


void QAbstractButtonHost::setText(const QString &text)
{
    setProperty("text",text);
}

QString QAbstractButtonHost::text()
{
    return property("text").toString();
}

void QAbstractButtonHost::setCheckable(bool checkable)
{
    setProperty("checkable",checkable);
}

bool QAbstractButtonHost::checkable()
{
    return property("checkable").toBool();
}

void QAbstractButtonHost::setChecked(bool checked)
{
    setProperty("checked",checked);
}

bool QAbstractButtonHost::checked()
{
    return property("checked").toBool();
}

void QAbstractButtonHost::clickedSlot()
{

}
