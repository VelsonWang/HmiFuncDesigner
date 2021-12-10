#include "qdateedithost.h"

#include "../property/qabstractproperty.h"
#include "../qpropertyfactory.h"
#include "../qcommonstruct.h"

#include <QDateEdit>

QDateEditHost::QDateEditHost(QAbstractHost *parent):
    QAbstractSpinBoxHost(parent)
{
    setProperty("need_frame", false);
    setProperty("accept_drop", false);
}

QString QDateEditHost::getShowName()
{
    return tr("Date Edit");
}

QString QDateEditHost::getShowGroup()
{
    return tr("Input Widgets");
}

QString QDateEditHost::getShowIcon()
{
    return ":/images/dateedit.png";
}

void QDateEditHost::createObject()
{
    m_object = new QDateEdit();
    m_object->setObjectName("dateedit");
}

void QDateEditHost::initProperty()
{
    QAbstractSpinBoxHost::initProperty();

    QAbstractProperty *pro;

    pro = QPropertyFactory::create_property("Date");
    if(pro != NULL) {
        pro->setObjectProperty("name", "maximumDate");
        pro->setAttribute("show_name", tr("MaximumDate"));
        pro->setAttribute("group", "Attributes");
        pro->setAttribute(ATTR_CAN_SAME, true);
        insertProperty(pro, 1);
    }

    pro = QPropertyFactory::create_property("Date");
    if(pro != NULL) {
        pro->setObjectProperty("name", "minimumDate");
        pro->setAttribute("show_name", tr("MinimumDate"));
        pro->setAttribute("group", "Attributes");
        pro->setAttribute(ATTR_CAN_SAME, true);
        insertProperty(pro);
    }

    pro = QPropertyFactory::create_property("Date");
    if(pro != NULL) {
        pro->setObjectProperty("name", "date");
        pro->setAttribute("show_name", tr("Date"));
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

        item.m_text = tr("YearSection");
        item.m_value = QDateTimeEdit::YearSection;
        items.append(item);

        item.m_text = tr("MonthSection");
        item.m_value = QDateTimeEdit::MonthSection;
        items.append(item);

        item.m_text = tr("SecondSection");
        item.m_value = QDateTimeEdit::DaySection;
        items.append(item);

        QVariant v;
        v.setValue<ComboItems>(items);
        pro->setAttribute("items", v);
        insertProperty(pro);
    }


    pro = QPropertyFactory::create_property("Script");
    if(pro != NULL) {
        pro->setObjectProperty("name", "dateChanged");
        pro->setAttribute("show_name", tr("DateChanged"));
        pro->setAttribute("group", "Events");
        insertProperty(pro);
    }

    setPropertyValue("geometry", QRect(0, 0, 100, 20));

    QDateEdit *e = (QDateEdit*)m_object;
    connect(e, SIGNAL(dateChanged(QDate)), this, SLOT(dateChanged(QDate)));
}

void QDateEditHost::setDate(int year, int month, int day)
{
    QDate dt(year, month, day);
    setPropertyValue("date", dt);
}

int QDateEditHost::year()
{
    QDate dt = getPropertyValue("date").toDate();
    return dt.year();
}

int QDateEditHost::month()
{
    QDate dt = getPropertyValue("date").toDate();
    return dt.month();
}

int QDateEditHost::day()
{
    QDate dt = getPropertyValue("date").toDate();
    return dt.day();
}

void QDateEditHost::setCurrentSection(int currentSection)
{
    setPropertyValue("currentSection", currentSection);
}

int QDateEditHost::currentSection()
{
    return getPropertyValue("currentSection").toInt();
}

void QDateEditHost::dateChanged(const QDate &date)
{
    QString code = getPropertyValue("dateChanged").toString();
    if(code != "") {
        QMap<QString, QString> param;
        param.insert("_year", QString::number(date.year()));
        param.insert("_month", QString::number(date.month()));
        param.insert("_day", QString::number(date.day()));
        exec(code, param);
    }
}
