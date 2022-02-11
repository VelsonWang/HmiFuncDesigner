#include "qtimeedithost.h"

#include "../property/qabstractproperty.h"
#include "../qpropertyfactory.h"
#include "../qcommonstruct.h"

#include <QTimeEdit>

QTimeEditHost::QTimeEditHost(QAbstractHost *parent):
    QAbstractSpinBoxHost(parent)
{
    setProperty("need_frame", false);
    setProperty("accept_drop", false);
}

QString QTimeEditHost::getShowName()
{
    return tr("Time Edit");
}

QString QTimeEditHost::getShowGroup()
{
    return tr("Input Widgets");
}


QString QTimeEditHost::getShowIcon()
{
    return ":/images/timeedit.png";
}

void QTimeEditHost::createObject()
{
    m_object = new QTimeEdit();
    m_object->setObjectName("timeedit");
}

void QTimeEditHost::initProperty()
{
    QAbstractSpinBoxHost::initProperty();

    QAbstractProperty *pro;

    pro = QPropertyFactory::create_property("Time");
    if(pro != NULL) {
        pro->setObjectProperty("name", "maximumTime");
        pro->setAttribute("show_name", tr("MaximumTime"));
        pro->setAttribute("group", "Attributes");
        pro->setAttribute(ATTR_CAN_SAME, true);
        insertProperty(pro, 1);
    }

    pro = QPropertyFactory::create_property("Time");
    if(pro != NULL) {
        pro->setObjectProperty("name", "minimumTime");
        pro->setAttribute("show_name", tr("MinimumTime"));
        pro->setAttribute("group", "Attributes");
        pro->setAttribute(ATTR_CAN_SAME, true);
        insertProperty(pro);
    }

    pro = QPropertyFactory::create_property("Time");
    if(pro != NULL) {
        pro->setObjectProperty("name", "time");
        pro->setAttribute("show_name", tr("Time"));
        pro->setAttribute("group", "Attributes");
        pro->setAttribute(ATTR_CAN_SAME, true);
        insertProperty(pro);
    }

    pro = QPropertyFactory::create_property("Enum");
    if(pro != NULL) {
        pro->setObjectProperty("name", "currentSection");
        pro->setAttribute("show_name", tr("CurrentSection"));
        pro->setAttribute("group", "Attributes");
        pro->setAttribute(ATTR_CAN_SAME, true);
        ComboItems items;
        tagComboItem item;

        item.m_text = tr("HourSection");
        item.m_value = QDateTimeEdit::HourSection;
        items.append(item);

        item.m_text = tr("MinuteSection");
        item.m_value = QDateTimeEdit::MinuteSection;
        items.append(item);

        item.m_text = tr("SecondSection");
        item.m_value = QDateTimeEdit::SecondSection;
        items.append(item);

        QVariant v;
        v.setValue<ComboItems>(items);
        pro->setAttribute("items", v);
        insertProperty(pro);
    }


    pro = QPropertyFactory::create_property("Script");
    if(pro != NULL) {
        pro->setObjectProperty("name", "timeChanged");
        pro->setAttribute("show_name", tr("TimeChanged"));
        pro->setAttribute("group", "Events");
        insertProperty(pro);
    }
    setPropertyValue("geometry", QRect(0, 0, 100, 20));

    QDateTimeEdit* e = (QDateTimeEdit*)m_object;
    connect(e, SIGNAL(timeChanged(QTime)), this, SLOT(timeChanged(QTime)));
}

void QTimeEditHost::setTime(int hour, int minute, int second)
{
    QTime ti(hour, minute, second);
    setPropertyValue("time", ti);
}

int QTimeEditHost::hour()
{
    QTime ti = getPropertyValue("time").toTime();
    return ti.hour();
}

int QTimeEditHost::minute()
{
    QTime ti = getPropertyValue("time").toTime();
    return ti.minute();
}

int QTimeEditHost::second()
{
    QTime ti = getPropertyValue("time").toTime();
    return ti.second();
}

void QTimeEditHost::setCurrentSection(int currentSection)
{
    setPropertyValue("currentSection", currentSection);
}

int QTimeEditHost::currentSection()
{
    return getPropertyValue("currentSection").toInt();
}

void QTimeEditHost::timeChanged(const QTime &time)
{
    QString code = getPropertyValue("stateChanged").toString();
    if(code != "") {
        QMap<QString, QString> param;
        param.insert("_hour", QString::number(time.hour()));
        param.insert("_minute", QString::number(time.minute()));
        param.insert("_second", QString::number(time.second()));
        exec(code, param);
    }
}
