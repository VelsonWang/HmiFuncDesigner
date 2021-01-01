#include "qdateproperty.h"

QDateProperty::QDateProperty(QAbstractProperty *parent):
    QAbstractProperty(parent)
{
    setObjectProperty("type","Date");
}

QString QDateProperty::get_value_text()
{
    QDate ti=get_value().toDate();
    return QString("%1:%2:%3").arg(ti.year(),4).arg(ti.month(),2).arg(ti.day(),2);
}


QIcon QDateProperty::get_value_icon()
{
    return QIcon();
}

void QDateProperty::make_value()
{
    int y=m_propertys.value("year").toInt();
    int mon=m_propertys.value("month").toInt();
    int d=m_propertys.value("day").toInt();

    QDate dt(y,mon,d);
    m_value.setValue<QDate>(dt);
}

void QDateProperty::write_value()
{
    QDate dt=m_value.toDate();
    m_propertys.insert("year",dt.year());
    m_propertys.insert("month",dt.month());
    m_propertys.insert("day",dt.day());
}
