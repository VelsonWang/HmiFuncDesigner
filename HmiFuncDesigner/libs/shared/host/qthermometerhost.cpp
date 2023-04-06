#include "qthermometerhost.h"
#include "../property/qabstractproperty.h"
#include "../qpropertyfactory.h"
#include "../qcommonstruct.h"
#include "../widgets/qthermometer.h"

QThermometerHost::QThermometerHost(QAbstractHost *parent) : QFrameHost(parent)
{
    setProperty("need_frame", false);
    setProperty("accept_drop", false);
    setProperty("scaledContents", true);
}

QString QThermometerHost::getShowName()
{
    return tr("温度计");//tr("Thermometer");
}

QString QThermometerHost::getShowGroup()
{
    return tr("显示部件");//tr("Display Widgets");
}

QString QThermometerHost::getShowIcon()
{
    return ":/images/thermometericon.png";
}

void QThermometerHost::createObject()
{
    m_object = new QThermometer();
    m_object->setObjectName("thermometer");
}

void QThermometerHost::initProperty()
{
    QFrameHost::initProperty();

    QAbstractProperty* pObj = QPropertyFactory::create_property("Tag");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "tag");
        pObj->setAttribute("show_name", tr("选择变量"));
        pObj->setAttribute("group", "HMI");
        pObj->setAttribute(ATTR_CAN_SAME, true);
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("Float");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "min");
        pObj->setAttribute("show_name", tr("最小值"));
        pObj->setAttribute("group", "Attributes");
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("Float");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "max");
        pObj->setAttribute("show_name", tr("最大值"));
        pObj->setAttribute("group", "Attributes");
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("Number");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "precision");
        pObj->setAttribute("show_name", tr("小数位数"));
        pObj->setAttribute("group", "Attributes");
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("Number");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "numTicks");
        pObj->setAttribute("show_name", tr("刻度段数"));
        pObj->setAttribute("group", "Attributes");
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("Float");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "threshold");
        pObj->setAttribute("show_name", tr("阈值"));
        pObj->setAttribute("group", "Attributes");
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("Enum");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "borderStyle");
        pObj->setAttribute("show_name", tr("边框样式"));
        pObj->setAttribute("group", "Attributes");
        pObj->setAttribute(ATTR_CAN_SAME, true);
        ComboItems items;
        QStringList itemValues;
        itemValues << tr("扁平|0") << tr("边框|1") << tr("凸起|2") << tr("凹陷|3");
        foreach(QString szEv, itemValues) {
            tagComboItem item;
            QStringList evs = szEv.split("|");
            item.m_text = evs[0];
            item.m_value = evs[1].toInt();
            items.append(item);
        }
        QVariant v;
        v.setValue<ComboItems>(items);
        pObj->setAttribute("items", v);
        insertProperty(pObj);
    }

    setPropertyValue("geometry", QRect(0, 0, 180, 180));
    removeProperty("frameShape");
    removeProperty("frameShadow");
}
