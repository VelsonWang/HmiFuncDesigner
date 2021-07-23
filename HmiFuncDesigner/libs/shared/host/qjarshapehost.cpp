#include "qjarshapehost.h"
#include "../property/qabstractproperty.h"
#include "../qpropertyfactory.h"
#include "../qcommonstruct.h"
#include "../widgets/qjarshape.h"

QJarShapeHost::QJarShapeHost(QAbstractHost *parent) : QWidgetHost(parent)
{
    setProperty("need_frame", false);
    setProperty("accept_drop", false);
    setProperty("scaledContents", true);
}

QString QJarShapeHost::getShowName()
{
    return tr("JarShape");
}

QString QJarShapeHost::getShowGroup()
{
    return tr("Display Widgets");
}

QString QJarShapeHost::getShowIcon()
{
    return ":/images/jarshape.png";
}

void QJarShapeHost::createObject()
{
    m_object = new QJarShape();
    m_object->setObjectName("jarshape");
}

void QJarShapeHost::initProperty()
{
    QAbstractProperty* pObj = QPropertyFactory::create_property("Tag");
    if(pObj != Q_NULLPTR) {
        pObj->setObjectProperty("name", "tag");
        pObj->setAttribute("show_name", tr("选择变量"));
        pObj->setAttribute("group", "HMI");
        pObj->setAttribute(ATTR_CAN_SAME, true);
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("String");
    if(pObj != Q_NULLPTR) {
        pObj->setObjectProperty("name", "jarShape");
        pObj->setAttribute("show_name", tr("容器标题"));
        pObj->setAttribute("group", "Attributes");
        insertProperty(pObj);
    }

    QWidgetHost::initProperty();

    pObj = QPropertyFactory::create_property("Font");
    if(pObj != Q_NULLPTR) {
        pObj->setObjectProperty("name", "font");
        pObj->setAttribute("show_name", tr("字体"));
        pObj->setAttribute("group", "Attributes");
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("Color");
    if(pObj != Q_NULLPTR) {
        pObj->setObjectProperty("name", "borderColor");
        pObj->setAttribute("show_name", tr("罐体颜色"));
        pObj->setAttribute("group", "Attributes");
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("Color");
    if(pObj != Q_NULLPTR) {
        pObj->setObjectProperty("name", "lessThanLowerLimitColor");
        pObj->setAttribute("show_name", tr("低于下限颜色"));
        pObj->setAttribute("group", "Attributes");
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("Color");
    if(pObj != Q_NULLPTR) {
        pObj->setObjectProperty("name", "normalColor");
        pObj->setAttribute("show_name", tr("正常液面颜色"));
        pObj->setAttribute("group", "Attributes");
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("Color");
    if(pObj != Q_NULLPTR) {
        pObj->setObjectProperty("name", "greaterThanUpperLimitColor");
        pObj->setAttribute("show_name", tr("高于上限颜色"));
        pObj->setAttribute("group", "Attributes");
        insertProperty(pObj);
    }

    pObj=QPropertyFactory::create_property("Float");
    if(pObj != Q_NULLPTR) {
        pObj->setObjectProperty("name", "upperLimitValue");
        pObj->setAttribute("show_name", tr("上限值"));
        pObj->setAttribute("group", "Attributes");
        insertProperty(pObj);
    }

    pObj=QPropertyFactory::create_property("Float");
    if(pObj != Q_NULLPTR) {
        pObj->setObjectProperty("name", "lowerLimitValue");
        pObj->setAttribute("show_name", tr("下限值"));
        pObj->setAttribute("group", "Attributes");
        insertProperty(pObj);
    }
    // 满量程值-刻度最大值
    pObj=QPropertyFactory::create_property("Float");
    if(pObj != Q_NULLPTR) {
        pObj->setObjectProperty("name", "maxValue");
        pObj->setAttribute("show_name", tr("满量程值"));
        pObj->setAttribute("group", "Attributes");
        insertProperty(pObj);
    }

    pObj=QPropertyFactory::create_property("Number");
    if(pObj != Q_NULLPTR) {
        pObj->setObjectProperty("name", "scaleNum");
        pObj->setAttribute("show_name", tr("刻度个数"));
        pObj->setAttribute("group", "Attributes");
        insertProperty(pObj);
    }

    pObj=QPropertyFactory::create_property("Bool");
    if(pObj != Q_NULLPTR) {
        pObj->setObjectProperty("name", "showOnInitial");
        pObj->setAttribute("show_name", tr("初始可见性"));
        pObj->setAttribute("group", "Attributes");
        insertProperty(pObj);
    }

    setPropertyValue("geometry", QRect(0, 0, 80, 120));
    //removeProperty("frameShape");
    //removeProperty("frameShadow");
}
