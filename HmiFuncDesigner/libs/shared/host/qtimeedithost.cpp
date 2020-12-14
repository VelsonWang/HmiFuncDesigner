#include "qtimeedithost.h"

#include "../property/qabstractproperty.h"
#include "../qpropertyfactory.h"
#include "../qcommonstruct.h"

#include <QTimeEdit>

QTimeEditHost::QTimeEditHost(QAbstractHost *parent):
    QAbstractSpinBoxHost(parent)
{
    setProperty("need_frame",false);
    setProperty("accept_drop",false);
}

QString QTimeEditHost::get_show_name()
{
    return tr("Time Edit");
}

QString QTimeEditHost::get_show_group()
{
    return tr("Input Widgets");
}


QString QTimeEditHost::get_show_icon()
{
    return ":/images/timeedit.png";
}

void QTimeEditHost::create_object()
{
    m_object=new QTimeEdit();
    m_object->setObjectName("timeedit");
}

void QTimeEditHost::init_property()
{
    QAbstractSpinBoxHost::init_property();

    QAbstractProperty *pro;

    pro=QPropertyFactory::create_property("Time");
    if(pro!=NULL)
    {
        pro->set_property("name","maximumTime");
        pro->set_attribute("show_name",tr("MaximumTime"));
        pro->set_attribute("group","Attributes");
        pro->set_attribute(ATTR_CAN_SAME,true);
        insert_property(pro,1);
    }

    pro=QPropertyFactory::create_property("Time");
    if(pro!=NULL)
    {
        pro->set_property("name","minimumTime");
        pro->set_attribute("show_name",tr("MinimumTime"));
        pro->set_attribute("group","Attributes");
        pro->set_attribute(ATTR_CAN_SAME,true);
        insert_property(pro);
    }

    pro=QPropertyFactory::create_property("Time");
    if(pro!=NULL)
    {
        pro->set_property("name","time");
        pro->set_attribute("show_name",tr("Time"));
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
        pro->set_property("name","timeChanged");
        pro->set_attribute("show_name",tr("TimeChanged"));
        pro->set_attribute("group","Events");
        insert_property(pro);
    }
    set_property_value("geometry",QRect(0,0,100,20));

    QDateTimeEdit* e=(QDateTimeEdit*)m_object;
    connect(e,SIGNAL(timeChanged(QTime)),this,SLOT(timeChanged(QTime)));
}

void QTimeEditHost::setTime(int hour, int minute, int second)
{
    QTime ti(hour,minute,second);
    set_property_value("time",ti);
}

int QTimeEditHost::hour()
{
    QTime ti=get_property_value("time").toTime();
    return ti.hour();
}

int QTimeEditHost::minute()
{
    QTime ti=get_property_value("time").toTime();
    return ti.minute();
}

int QTimeEditHost::second()
{
    QTime ti=get_property_value("time").toTime();
    return ti.second();
}

void QTimeEditHost::setCurrentSection(int currentSection)
{
    set_property_value("currentSection",currentSection);
}

int QTimeEditHost::currentSection()
{
    return get_property_value("currentSection").toInt();
}

void QTimeEditHost::timeChanged(const QTime &time)
{
    QString code=get_property_value("stateChanged").toString();
    if(code!="")
    {
        QMap<QString,QString> param;
        param.insert("_hour",QString::number(time.hour()));
        param.insert("_minute",QString::number(time.minute()));
        param.insert("_second",QString::number(time.second()));
        exec(code,param);
    }
}
