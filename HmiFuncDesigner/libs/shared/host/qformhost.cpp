#include "qformhost.h"

#include "../property/qabstractproperty.h"

#include <QWidget>

QFormHost::QFormHost(QAbstractHost *parent):
    QAbstractWidgetHost(parent)
{
    setProperty("title",FORM_TITLE);
}

void QFormHost::createObject()
{
    m_object=new QWidget();
}

void QFormHost::initProperty()
{
    QAbstractWidgetHost::initProperty();


    QAbstractProperty *pro = this->getProperty("geometry");
    pro->setAttribute("show_name", tr("坐标大小"));// tr("Size")
    pro->setAttribute(QString("x:")+ATTR_VISIBLE, false);
    pro->setAttribute(QString("y:")+ATTR_VISIBLE, false);

    pro = this->getProperty("objectName");
    pro->setAttribute("editabled", false);

    setPropertyValue("geometry", QRect(0, 0, 800, 600));
}
