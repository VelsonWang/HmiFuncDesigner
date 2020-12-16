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

QString QDateTimeEditHost::getShowName()
{
    return tr("DateTime Edit");
}

QString QDateTimeEditHost::getShowGroup()
{
    return tr("Input Widgets");
}

QString QDateTimeEditHost::getShowIcon()
{
    return ":/images/datetimeedit.png";
}

void QDateTimeEditHost::createObject()
{
    m_object=new QDateTimeEdit();
    m_object->setObjectName("datetimeedit");
}

void QDateTimeEditHost::initProperty()
{
    QAbstractSpinBoxHost::initProperty();

    QAbstractProperty *pro;

    pro=QPropertyFactory::create_property("DateTime");
    if(pro!=Q_NULLPTR)
    {
        pro->setProperty("name","maximumDateTime");
        pro->setAttribute("show_name",tr("MaximumDateTime"));
        pro->setAttribute("group","Attributes");
        pro->setAttribute(ATTR_CAN_SAME,true);
        insertProperty(pro,1);
    }

    pro=QPropertyFactory::create_property("DateTime");
    if(pro!=Q_NULLPTR)
    {
        pro->setProperty("name","minimumDateTime");
        pro->setAttribute("show_name",tr("MinimumDateTime"));
        pro->setAttribute("group","Attributes");
        pro->setAttribute(ATTR_CAN_SAME,true);
        insertProperty(pro);
    }

    pro=QPropertyFactory::create_property("DateTime");
    if(pro!=Q_NULLPTR)
    {
        pro->setProperty("name","dateTime");
        pro->setAttribute("show_name",tr("DateTime"));
        pro->setAttribute("group","Attributes");
        pro->setAttribute(ATTR_CAN_SAME,true);
        insertProperty(pro);
    }

    pro=QPropertyFactory::create_property("Enum");
    if(pro!=Q_NULLPTR)
    {
        pro->setProperty("name","currentSection");
        pro->setAttribute("show_name",tr("CurrentSection"));
        pro->setAttribute("group","Attributes");
        pro->setAttribute(ATTR_CAN_SAME,true);
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
        pro->setAttribute("items",v);
        insertProperty(pro);
    }

    pro=QPropertyFactory::create_property("Script");
    if(pro!=Q_NULLPTR)
    {
        pro->setProperty("name","dateTimeChanged");
        pro->setAttribute("show_name",tr("DateTimeChanged"));
        pro->setAttribute("group","Events");
        insertProperty(pro);
    }

    setPropertyValue("geometry",QRect(0,0,140,20));

    QDateTimeEdit *e=(QDateTimeEdit*)m_object;
    connect(e,SIGNAL(dateTimeChanged(QDateTime)),this,SLOT(dateTimeChanged(QDateTime)));
}

void QDateTimeEditHost::setDateTime(int year, int month, int day, int hour, int minute, int second)
{
    QDateTime dt(QDate(year,month,day),QTime(hour,minute,second));
    setPropertyValue("dateTime",dt);
}

int QDateTimeEditHost::year()
{
    QDateTime dt=getPropertyValue("dateTime").toDateTime();
    return dt.date().year();
}

int QDateTimeEditHost::month()
{
    QDateTime dt=getPropertyValue("dateTime").toDateTime();
    return dt.date().month();
}

int QDateTimeEditHost::day()
{
    QDateTime dt=getPropertyValue("dateTime").toDateTime();
    return dt.date().day();
}

int QDateTimeEditHost::hour()
{
    QDateTime dt=getPropertyValue("dateTime").toDateTime();
    return dt.time().hour();
}

int QDateTimeEditHost::minute()
{
    QDateTime dt=getPropertyValue("dateTime").toDateTime();
    return dt.time().minute();
}

int QDateTimeEditHost::second()
{
    QDateTime dt=getPropertyValue("dateTime").toDateTime();
    return dt.time().second();
}

void QDateTimeEditHost::setCurrentSection(int currentSection)
{
    setPropertyValue("currentSection",currentSection);
}

int QDateTimeEditHost::currentSection()
{
    return getPropertyValue("currentSection").toInt();
}

void QDateTimeEditHost::dateTimeChanged(const QDateTime &datetime)
{
    QString code=getPropertyValue("dateTimeChanged").toString();
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
