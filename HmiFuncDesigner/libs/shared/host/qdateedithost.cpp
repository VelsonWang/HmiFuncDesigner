#include "qdateedithost.h"

#include "../property/qabstractproperty.h"
#include "../qpropertyfactory.h"
#include "../qcommonstruct.h"

#include <QDateEdit>

QDateEditHost::QDateEditHost(QAbstractHost *parent):
    QAbstractSpinBoxHost(parent)
{
    setProperty("need_frame",false);
    setProperty("accept_drop",false);
}

QString QDateEditHost::get_show_name()
{
    return tr("Date Edit");
}

QString QDateEditHost::get_show_group()
{
    return tr("Input Widgets");
}

QString QDateEditHost::get_show_icon()
{
    return ":/images/dateedit.png";
}

void QDateEditHost::create_object()
{
    m_object=new QDateEdit();
    m_object->setObjectName("dateedit");
}

void QDateEditHost::init_property()
{
    QAbstractSpinBoxHost::init_property();

    QAbstractProperty *pro;

    pro=QPropertyFactory::create_property("Date");
    if(pro!=NULL)
    {
        pro->set_property("name","maximumDate");
        pro->set_attribute("show_name",tr("MaximumDate"));
        pro->set_attribute("group","Attributes");
        pro->set_attribute(ATTR_CAN_SAME,true);
        insert_property(pro,1);
    }

    pro=QPropertyFactory::create_property("Date");
    if(pro!=NULL)
    {
        pro->set_property("name","minimumDate");
        pro->set_attribute("show_name",tr("MinimumDate"));
        pro->set_attribute("group","Attributes");
        pro->set_attribute(ATTR_CAN_SAME,true);
        insert_property(pro);
    }

    pro=QPropertyFactory::create_property("Date");
    if(pro!=NULL)
    {
        pro->set_property("name","date");
        pro->set_attribute("show_name",tr("Date"));
        pro->set_attribute("group","Attributes");
        pro->set_attribute(ATTR_CAN_SAME,true);
        insert_property(pro);
    }

    pro=QPropertyFactory::create_property("Enum");
    if(pro!=NULL)
    {
        pro->set_property("name","currentSection");
        pro->set_attribute("show_name",tr("CurrentSection"));
        pro->set_attribute("group","Attributes");
        pro->set_attribute(ATTR_CAN_SAME,true);
        ComboItems items;
        tagComboItem item;

        item.m_text=tr("YearSection");
        item.m_value=QDateTimeEdit::YearSection;
        items.append(item);

        item.m_text=tr("MonthSection");
        item.m_value=QDateTimeEdit::MonthSection;
        items.append(item);

        item.m_text=tr("SecondSection");
        item.m_value=QDateTimeEdit::DaySection;
        items.append(item);

        QVariant v;
        v.setValue<ComboItems>(items);
        pro->set_attribute("items",v);
        insert_property(pro);
    }


    pro=QPropertyFactory::create_property("Script");
    if(pro!=NULL)
    {
        pro->set_property("name","dateChanged");
        pro->set_attribute("show_name",tr("DateChanged"));
        pro->set_attribute("group","Events");
        insert_property(pro);
    }

    set_property_value("geometry",QRect(0,0,100,20));

    QDateEdit *e=(QDateEdit*)m_object;
    connect(e,SIGNAL(dateChanged(QDate)),this,SLOT(dateChanged(QDate)));
}

void QDateEditHost::setDate(int year, int month, int day)
{
    QDate dt(year,month,day);
    set_property_value("date",dt);
}

int QDateEditHost::year()
{
    QDate dt=get_property_value("date").toDate();
    return dt.year();
}

int QDateEditHost::month()
{
    QDate dt=get_property_value("date").toDate();
    return dt.month();
}

int QDateEditHost::day()
{
    QDate dt=get_property_value("date").toDate();
    return dt.day();
}

void QDateEditHost::setCurrentSection(int currentSection)
{
    set_property_value("currentSection",currentSection);
}

int QDateEditHost::currentSection()
{
    return get_property_value("currentSection").toInt();
}

void QDateEditHost::dateChanged(const QDate &date)
{
    QString code=get_property_value("dateChanged").toString();
    if(code!="")
    {
        QMap<QString,QString> param;
        param.insert("_year",QString::number(date.year()));
        param.insert("_month",QString::number(date.month()));
        param.insert("_day",QString::number(date.day()));
        exec(code,param);
    }
}
