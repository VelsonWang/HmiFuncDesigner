#include "qmemerydataproperty.h"

#include "../xmlobject.h"
#include "../qcommonstruct.h"

QMemeryDataProperty::QMemeryDataProperty(QAbstractProperty *parent):
    QAbstractProperty(parent)
{
    set_property("type","Memery");
}

void QMemeryDataProperty::toObject(XMLObject *xml)
{
    if(xml!=NULL)
    {
        xml->clear();

        xml->set_title(PROPERTY_TITLE);

        QMapIterator<QString,QVariant>      it(m_propertys);

        while(it.hasNext())
        {
            it.next();
            xml->set_property(it.key(),it.value().toString());
        }

        xml->set_property("name",m_attributes.value("name").toString());
        MemeryValueItems items=get_value().value<MemeryValueItems>();

        XMLObject *obj;
        foreach(tagMemeryValueInfo info,items)
        {
            obj=new XMLObject;
            obj->set_property("property",info.m_propertyName);
            obj->set_property("value",info.m_valueName);
            obj->set_property("script",info.m_script);
            obj->set_property("group",info.m_valueGroup);
            xml->inser_child(-1,obj);
        }
    }
}

void QMemeryDataProperty::fromObject(XMLObject *xml)
{
    if(xml!=NULL)
    {
        if(xml->get_title()!=PROPERTY_TITLE)
        {
            return;
        }

        QMapIterator<QString,QVariant>      it(m_propertys);

        while(it.hasNext())
        {
            it.next();
            m_propertys.insert(it.key(),xml->get_property(it.key()));
        }
        QList<XMLObject*>   children=xml->getChildren();
        MemeryValueItems items;
        foreach(XMLObject* child,children)
        {
            tagMemeryValueInfo info;
            info.m_propertyName=child->get_property("property");
            info.m_valueName=child->get_property("value");
            info.m_script=child->get_property("script");
            info.m_valueGroup=child->get_property("group");
            items.append(info);
        }
        m_value.setValue<MemeryValueItems>(items);
        refresh();
    }
}

QString QMemeryDataProperty::get_value_text()
{
    MemeryValueItems c=get_value().value<MemeryValueItems>();
    if(c.size()==0)
    {
        return "None";
    }
    else
    {
        return QString("count:%1").arg(c.size());
    }
}
QIcon QMemeryDataProperty::get_value_icon()
{
    return QIcon();
}
