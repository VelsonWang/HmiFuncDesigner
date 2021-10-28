#include "qtimeproperty.h"

QTimeProperty::QTimeProperty(QAbstractProperty *parent) :
    QAbstractProperty(parent)
{
    setObjectProperty("type", "Time");
}

QString QTimeProperty::getValueText()
{
    QTime ti = get_value().toTime();
    return QString("%1:%2:%3").arg(ti.hour(), 2).arg(ti.minute(), 2).arg(ti.second(), 2);
}


QIcon QTimeProperty::getValueIcon()
{
    return QIcon();
}

void QTimeProperty::makeValue()
{
    int h = m_propertys.value("hour").toInt();
    int m = m_propertys.value("minute").toInt();
    int s = m_propertys.value("second").toInt();

    QTime ti(h, m, s);
    m_value.setValue<QTime>(ti);
}

void QTimeProperty::writeValue()
{
    QTime dt = m_value.toTime();
    m_propertys.insert("hour", dt.hour());
    m_propertys.insert("minute", dt.minute());
    m_propertys.insert("second", dt.second());
}

