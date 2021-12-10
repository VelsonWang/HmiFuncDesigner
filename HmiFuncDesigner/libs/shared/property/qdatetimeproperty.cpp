#include "qdatetimeproperty.h"

QDateTimeProperty::QDateTimeProperty(QAbstractProperty *parent) :
    QAbstractProperty(parent)
{
    setObjectProperty("type", "DateTime");
}

QString QDateTimeProperty::getValueText()
{
    QDateTime ti = getValue().toDateTime();
    return QString("%1-%2-%3 %4:%5:%6").arg(ti.date().year(), 2).arg(ti.date().month(), 2)
           .arg(ti.date().day(), 2).arg(ti.time().hour(), 2).arg(ti.time().minute(), 2)
           .arg(ti.time().second(), 2);
}


QIcon QDateTimeProperty::getValueIcon()
{
    return QIcon();
}

void QDateTimeProperty::makeValue()
{
    int y = m_propertys.value("year").toInt();
    int mon = m_propertys.value("month").toInt();
    int d = m_propertys.value("day").toInt();
    int h = m_propertys.value("hour").toInt();
    int m = m_propertys.value("minute").toInt();
    int s = m_propertys.value("second").toInt();

    QDateTime dt(QDate(y, mon, d), QTime(h, m, s));
    m_value.setValue<QDateTime>(dt);
}

void QDateTimeProperty::writeValue()
{
    QDateTime dt = m_value.toDateTime();
    m_propertys.insert("year", dt.date().year());
    m_propertys.insert("month", dt.date().month());
    m_propertys.insert("day", dt.date().day());
    m_propertys.insert("hour", dt.time().hour());
    m_propertys.insert("minute", dt.time().minute());
    m_propertys.insert("second", dt.time().second());
}


