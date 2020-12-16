#include "qmemerydataproperty.h"

#include "../xmlobject.h"
#include "../qcommonstruct.h"

QMemeryDataProperty::QMemeryDataProperty(QAbstractProperty *parent):
    QAbstractProperty(parent)
{
    setProperty("type","Memery");
}

void QMemeryDataProperty::toObject(XMLObject *xml)
{
    if(xml!=Q_NULLPTR)
    {
        xml->clear();

        xml->setTagName(PROPERTY_TITLE);

        QMapIterator<QString,QVariant>      it(m_propertys);

        while(it.hasNext())
        {
            it.next();
            xml->setProperty(it.key(),it.value().toString());
        }

        xml->setProperty("name",m_attributes.value("name").toString());
        MemeryValueItems items=get_value().value<MemeryValueItems>();

        XMLObject *obj;
        foreach(tagMemeryValueInfo info,items)
        {
            obj=new XMLObject;
            obj->setProperty("property",info.m_propertyName);
            obj->setProperty("value",info.m_valueName);
            obj->setProperty("script",info.m_script);
            obj->setProperty("group",info.m_valueGroup);
            xml->inser_child(-1,obj);
        }
    }
}

void QMemeryDataProperty::fromObject(XMLObject *xml)
{
    if(xml!=Q_NULLPTR)
    {
        if(xml->getTagName()!=PROPERTY_TITLE)
        {
            return;
        }

        QMapIterator<QString,QVariant>      it(m_propertys);

        while(it.hasNext())
        {
            it.next();
            m_propertys.insert(it.key(),xml->getProperty(it.key()));
        }
        QList<XMLObject*>   children=xml->getChildren();
        MemeryValueItems items;
        foreach(XMLObject* child,children)
        {
            tagMemeryValueInfo info;
            info.m_propertyName=child->getProperty("property");
            info.m_valueName=child->getProperty("value");
            info.m_script=child->getProperty("script");
            info.m_valueGroup=child->getProperty("group");
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
