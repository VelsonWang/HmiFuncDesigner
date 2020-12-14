#include "qformhost.h"

#include "../property/qabstractproperty.h"

#include <QWidget>

QFormHost::QFormHost(QAbstractHost *parent):
    QAbstractWidgetHost(parent)
{
    setProperty("title",FORM_TITLE);
}

void QFormHost::create_object()
{
    m_object=new QWidget();
}

void QFormHost::init_property()
{
    QAbstractWidgetHost::init_property();


    QAbstractProperty *pro=this->get_property("geometry");
    pro->set_attribute("show_name",tr("Size"));
    pro->set_attribute(QString("x:")+ATTR_VISIBLE,false);
    pro->set_attribute(QString("y:")+ATTR_VISIBLE,false);


    pro=this->get_property("objectName");
    pro->set_attribute("editabled",false);

    set_property_value("geometry",QRect(0,0,800,600));
}
