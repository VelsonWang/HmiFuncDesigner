#include "qabstractwidgethost.h"
#include <QDynamicPropertyChangeEvent>

QAbstractWidgetHost::QAbstractWidgetHost(QAbstractHost *parent):
    QAbstractHost(parent)
{
}



void QAbstractWidgetHost::setGeometry(int x, int y, int width, int height)
{
    QRect rect(x,y,width,height);
    setProperty("geometry",rect);
}

void QAbstractWidgetHost::setEnabled(bool enabled)
{
    setProperty("enabled",enabled);
}

void QAbstractWidgetHost::setToolTip(const QString &tooltip)
{
    setProperty("toolTip",tooltip);
}

int QAbstractWidgetHost::x()
{
    return property("geometry").toRect().x();
}

int QAbstractWidgetHost::y()
{
    return property("geometry").toRect().y();
}

int QAbstractWidgetHost::width()
{
    return property("geometry").toRect().width();
}

int QAbstractWidgetHost::height()
{
    return property("geometry").toRect().height();
}

bool QAbstractWidgetHost::enabled()
{
    return property("enabled").toBool();
}

QString QAbstractWidgetHost::toolTip()
{
    return property("toolTip").toString();
}

void QAbstractWidgetHost::setCursor(int cursor)
{
    QCursor c((Qt::CursorShape)cursor);
    QVariant v;
    v.setValue<QCursor>(c);
    setProperty("cursor",v);
}

int QAbstractWidgetHost::cursor()
{
    QCursor c=property("cursor").value<QCursor>();
    return c.shape();
}
