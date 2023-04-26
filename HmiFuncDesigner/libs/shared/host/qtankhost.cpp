#include "qtankhost.h"
#include "../property/qabstractproperty.h"
#include "../qpropertyfactory.h"
#include "../qcommonstruct.h"
#include "../widgets/qtank.h"

QTankHost::QTankHost(QAbstractHost *parent) : QFrameHost(parent)
{
    setProperty("need_frame", false);
    setProperty("accept_drop", false);
    setProperty("scaledContents", true);
}

QString QTankHost::getShowName()
{
    return tr("罐");//tr("Tank");
}

QString QTankHost::getShowGroup()
{
    return tr("显示部件");//tr("Display Widgets");
}

QString QTankHost::getShowIcon()
{
    return ":/images/tank.png";
}

void QTankHost::createObject()
{
    m_object = new QTank();
    m_object->setObjectName("tank");
}

void QTankHost::initProperty()
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
        tagComboItem item;

        item.m_text = tr("扁平");
        item.m_value = QTank::Flat;
        items.append(item);

        item.m_text = tr("边框");
        item.m_value = QTank::Frame;
        items.append(item);

        item.m_text = tr("凸起");
        item.m_value = QTank::Raised;
        items.append(item);

        item.m_text = tr("凹陷");
        item.m_value = QTank::Sunken;
        items.append(item);

        QVariant v;
        v.setValue<ComboItems>(items);
        pObj->setAttribute("items", v);
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("Color");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "tankColor");
        pObj->setAttribute("show_name", tr("罐体颜色"));
        pObj->setAttribute("group", "Attributes");
        insertProperty(pObj);
    }

    setPropertyValue("geometry", QRect(0, 0, 200, 180));
    removeProperty("frameShape");
    removeProperty("frameShadow");
}
