#include "qdatetimeedithost.h"

#include "../property/qabstractproperty.h"
#include "../qpropertyfactory.h"
#include "../qcommonstruct.h"

#include <QDateTimeEdit>

QDateTimeEditHost::QDateTimeEditHost(QAbstractHost *parent):
    QAbstractSpinBoxHost(parent)
{
    setProperty("need_frame",false);
    setProperty("accept_drop",false);
}

QString QDateTimeEditHost::get_show_name()
{
    return tr("DateTime Edit");
}

QString QDateTimeEditHost::get_show_group()
{
    return tr("Input Widgets");
}

QString QDateTimeEditHost::get_show_icon()
{
    return ":/images/datetimeedit.png";
}

void QDateTimeEditHost::create_object()
{
    m_object=new QDateTimeEdit();
    m_object->setObjectName("datetimeedit");
}

void QDateTimeEditHost::init_property()
{
    QAbstractSpinBoxHost::init_property();

    QAbstractProperty *pro;

    pro=QPropertyFactory::create_property("DateTime");
    if(pro!=NULL)
    {
        pro->set_property("name","maximumDateTime");
        pro->set_attribute("show_name",tr("MaximumDateTime"));
        pro->set_attribute("group","Attributes");
        pro->set_attribute(ATTR_CAN_SAME,true);
        insert_property(pro,1);
    }

    pro=QPropertyFactory::create_property("DateTime");
    if(pro!=NULL)
    {
        pro->set_property("name","minimumDateTime");
        pro->set_attribute("show_name",tr("MinimumDateTime"));
        pro->set_attribute("group","Attributes");
        pro->set_attribute(ATTR_CAN_SAME,true);
        insert_property(pro);
    }

    pro=QPropertyFactory::create_property("DateTime");
    if(pro!=NULL)
    {
        pro->set_property("name","dateTime");
        pro->set_attribute("show_name",tr("DateTime"));
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

        item.m_text=tr("HourSection");
        item.m_value=QDateTimeEdit::HourSection;
        items.append(item);

        item.m_text=tr("MinuteSection");
        item.m_value=QDateTimeEdit::MinuteSection;
        items.append(item);

        item.m_text=tr("SecondSection");
        item.m_value=QDateTimeEdit::SecondSection;
        items.append(item);

        QVariant v;
        v.setValue<ComboItems>(items);
        pro->set_attribute("items",v);
        insert_property(pro);
    }

    pro=QPropertyFactory::create_property("Script");
    if(pro!=NULL)
    {
        pro->set_property("name","dateTimeChanged");
        pro->set_attribute("show_name",tr("DateTimeChanged"));
        pro->set_attribute("group","Events");
        insert_property(pro);
    }

    set_property_value("geometry",QRect(0,0,140,20));

    QDateTimeEdit *e=(QDateTimeEdit*)m_object;
    connect(e,SIGNAL(dateTimeChanged(QDateTime)),this,SLOT(dateTimeChanged(QDateTime)));
}

void QDateTimeEditHost::setDateTime(int year, int month, int day, int hour, int minute, int second)
{
    QDateTime dt(QDate(year,month,day),QTime(hour,minute,second));
    set_property_value("dateTime",dt);
}

int QDateTimeEditHost::year()
{
    QDateTime dt=get_property_value("dateTime").toDateTime();
    return dt.date().year();
}

int QDateTimeEditHost::month()
{
    QDateTime dt=get_property_value("dateTime").toDateTime();
    return dt.date().month();
}

int QDateTimeEditHost::day()
{
    QDateTime dt=get_property_value("dateTime").toDateTime();
    return dt.date().day();
}

int QDateTimeEditHost::hour()
{
    QDateTime dt=get_property_value("dateTime").toDateTime();
    return dt.time().hour();
}

int QDateTimeEditHost::minute()
{
    QDateTime dt=get_property_value("dateTime").toDateTime();
    return dt.time().minute();
}

int QDateTimeEditHost::second()
{
    QDateTime dt=get_property_value("dateTime").toDateTime();
    return dt.time().second();
}

void QDateTimeEditHost::setCurrentSection(int currentSection)
{
    set_property_value("currentSection",currentSection);
}

int QDateTimeEditHost::currentSection()
{
    return get_property_value("currentSection").toInt();
}

void QDateTimeEditHost::dateTimeChanged(const QDateTime &datetime)
{
    QString code=get_property_value("dateTimeChanged").toString();
    if(code!="")
    {
        QMap<QString,QString> param;
        param.insert("_year",QString::number(datetime.date().year()));
        param.insert("_month",QString::number(datetime.date().month()));
        param.insert("_day",QString::number(datetime.date().day()));
        param.insert("_hour",QString::number(datetime.time().hour()));
        param.insert("_minute",QString::number(datetime.time().minute()));
        param.insert("_second",QString::number(datetime.time().second()));
        exec(code,param);
    }
}
