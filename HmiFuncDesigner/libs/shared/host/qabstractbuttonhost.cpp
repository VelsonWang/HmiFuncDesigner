#include "qabstractbuttonhost.h"

#include "../property/qabstractproperty.h"
#include "../qpropertyfactory.h"
#include "../qcommonstruct.h"

#include <QAbstractButton>

QAbstractButtonHost::QAbstractButtonHost(QAbstractHost *parent):
    QWidgetHost(parent)
{
}

void QAbstractButtonHost::init_property()
{
    QWidgetHost::init_property();

    QAbstractProperty *pro;

    pro=QPropertyFactory::create_property("String");
    if(pro!=NULL)
    {
        pro->set_property("name","text");
        pro->set_attribute("show_name",tr("Text"));
        pro->set_attribute("group","Attributes");
        pro->set_attribute(ATTR_CAN_SAME,true);
        insert_property(pro,1);
    }

    pro=QPropertyFactory::create_property("Bool");
    if(pro!=NULL)
    {
        pro->set_property("name","checkable");
        pro->set_attribute("show_name",tr("Checkable"));
        pro->set_attribute("group","Attributes");
        pro->set_attribute(ATTR_CAN_SAME,true);
        insert_property(pro);
    }

    pro=QPropertyFactory::create_property("Bool");
    if(pro!=NULL)
    {
        pro->set_property("name","checked");
        pro->set_attribute("show_name",tr("Checked"));
        pro->set_attribute("group","Attributes");
        pro->set_attribute(ATTR_CAN_SAME,true);
        insert_property(pro);
    }

    pro=QPropertyFactory::create_property("Script");
    if(pro!=NULL)
    {
        pro->set_property("name","clicked");
        pro->set_attribute("show_name",tr("Clicked"));
        pro->set_attribute("group","Events");
        insert_property(pro);
    }



    QAbstractButton *button=(QAbstractButton*)m_object;
    connect(button,SIGNAL(clicked()),this,SLOT(clicked_slot()));
}

void QAbstractButtonHost::setText(const QString &text)
{
    set_property_value("text",text);
}

QString QAbstractButtonHost::text()
{
    return get_property_value("text").toString();
}

void QAbstractButtonHost::setCheckable(bool checkable)
{
    set_property_value("checkable",checkable);
}

bool QAbstractButtonHost::checkable()
{
    return get_property_value("checkable").toBool();
}

void QAbstractButtonHost::setChecked(bool checked)
{
    set_property_value("checked",checked);
}

bool QAbstractButtonHost::checked()
{
    return get_property_value("checked").toBool();
}

void QAbstractButtonHost::clicked_slot()
{
    QString code=get_property_value("clicked").toString();
    if(code!="")
    {
        QMap<QString,QString> param;
        exec(code,param);
    }
}
