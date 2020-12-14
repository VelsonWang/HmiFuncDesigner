#include "qstylesheetproperty.h"

#include "stylesheetitem/qstylesheetitemfactory.h"
#include "stylesheetitem/qabstractstylesheetitem.h"
#include "stylesheetitem/stylesheetstruct.h"

#include "../xmlobject.h"

QStylesheetProperty::QStylesheetProperty(QAbstractProperty *parent):
    QAbstractProperty(parent)
{
    set_property("type","StyleSheet");
    set_attribute(ATTR_RESET_ABLEABLE,false);
}

QString QStylesheetProperty::get_value_text()
{
    return tr("Clicked for more");
}

QIcon QStylesheetProperty::get_value_icon()
{
    return QIcon();
}

void QStylesheetProperty::fromObject(XMLObject *xml)
{
    if(xml==NULL || xml->get_title()!=PROPERTY_TITLE)
    {
        return;
    }

    QMapIterator<QString,QString>      it(xml->get_propertys());

    while(it.hasNext())
    {
        it.next();
        m_propertys.insert(it.key(),it.value());
    }

    QAbstractStylesheetItem *maker=QStylesheetItemFactory::createItem(get_property("name").toString());

    if(maker==NULL)
    {
        return;
    }

    tagStylesheetItems items;

    QList<XMLObject*> list=xml->getChildren();

    foreach(XMLObject* c,list)
    {

        tagStylesheetItem item;
        maker->clear();
        maker->read(c);
        item=maker->value().value<tagStylesheetItem>();
        item.m_attributes.insert("title",c->get_property("title"));
        items.append(item);
    }

    delete maker;
    QVariant v;
    v.setValue<tagStylesheetItems>(items);
    set_value(v);
}

void QStylesheetProperty::toObject(XMLObject *xml)
{
    tagStylesheetItems items=m_value.value<tagStylesheetItems>();
    if(items.size()==0)
    {
        return;
    }

    xml->set_title(PROPERTY_TITLE);

    QMapIterator<QString,QVariant>      it(m_propertys);

    while(it.hasNext())
    {
        it.next();
        xml->set_property(it.key(),it.value().toString());
    }
    QAbstractStylesheetItem *maker=QStylesheetItemFactory::createItem(get_property("name").toString());
    if(maker!=NULL)
    {
        foreach(tagStylesheetItem item,items)
        {
            QVariant v;
            v.setValue<tagStylesheetItem>(item);
            maker->setProperty("title",item.m_attributes.value("title").toString());
            maker->setValue(v);
            XMLObject *c=new XMLObject(xml);
            maker->write(c);
        }

        delete maker;
    }
}
