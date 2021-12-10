#include "qindicationlamphost.h"
#include "../property/qabstractproperty.h"
#include "../qpropertyfactory.h"
#include "../qcommonstruct.h"
#include "../widgets/qindicationlamp.h"

QIndicationLampHost::QIndicationLampHost(QAbstractHost *parent) : QFrameHost(parent)
{
    setProperty("need_frame", false);
    setProperty("accept_drop", false);
    setProperty("scaledContents", true);
}

QString QIndicationLampHost::getShowName()
{
    return tr("IndicationLamp");
}

QString QIndicationLampHost::getShowGroup()
{
    return tr("Display Widgets");
}

QString QIndicationLampHost::getShowIcon()
{
    return ":/images/indicationlamp.png";
}

void QIndicationLampHost::createObject()
{
    m_object = new QIndicationLamp();
    m_object->setObjectName("IndicationLamp");
}

void QIndicationLampHost::initProperty()
{
    QFrameHost::initProperty();

    QAbstractProperty *pPropObj;

    pPropObj = QPropertyFactory::create_property("Tag");
    if(pPropObj != NULL) {
        pPropObj->setObjectProperty("name", "tag");
        pPropObj->setAttribute("show_name", tr("选择变量"));
        pPropObj->setAttribute("group", "HMI");
        pPropObj->setAttribute(ATTR_CAN_SAME, true);
        insertProperty(pPropObj);
    }

    pPropObj = QPropertyFactory::create_property("Bool");
    if(pPropObj != NULL) {
        pPropObj->setObjectProperty("name", "StateOnInitial");
        pPropObj->setAttribute("show_name", tr("初始状态"));
        pPropObj->setAttribute("group", "Attributes");
        insertProperty(pPropObj);
    }

    pPropObj = QPropertyFactory::create_property("Image");
    if(pPropObj != NULL) {
        pPropObj->setObjectProperty("name", "ResetImageFile");
        pPropObj->setAttribute("show_name", tr("选择复位图片"));
        pPropObj->setAttribute("filters", "Images (*.png *.xpm *.jpg)");
        pPropObj->setAttribute("group", "Attributes");
        insertProperty(pPropObj);
    }

    pPropObj = QPropertyFactory::create_property("Image");
    if(pPropObj != NULL) {
        pPropObj->setObjectProperty("name", "SetImageFile");
        pPropObj->setAttribute("show_name", tr("选择置位图片"));
        pPropObj->setAttribute("filters", "Images (*.png *.xpm *.jpg)");
        pPropObj->setAttribute("group", "Attributes");
        insertProperty(pPropObj);
    }

    pPropObj = QPropertyFactory::create_property("Bool");
    if(pPropObj != NULL) {
        pPropObj->setObjectProperty("name", "NoScale");
        pPropObj->setAttribute("show_name", tr("原尺寸显示"));
        pPropObj->setAttribute("group", "Attributes");
        insertProperty(pPropObj);
    }

    pPropObj = QPropertyFactory::create_property("Number");
    if(pPropObj != NULL) {
        pPropObj->setObjectProperty("name", "BoardWidth");
        pPropObj->setAttribute("show_name", tr("边框宽度"));
        pPropObj->setAttribute("group", "Attributes");
        insertProperty(pPropObj);
    }

    pPropObj = QPropertyFactory::create_property("Color");
    if(pPropObj != NULL) {
        pPropObj->setObjectProperty("name", "BoardColor");
        pPropObj->setAttribute("show_name", tr("边框颜色"));
        pPropObj->setAttribute("group", "Attributes");
        insertProperty(pPropObj);
    }

    setPropertyValue("geometry", QRect(0, 0, 80, 80));
    removeProperty("frameShape");
    removeProperty("frameShadow");
}

/**
 * @brief QIndicationLampHost::supportFuncEvents
 * @details 控件支持的功能事件
 * @return
 */
QStringList QIndicationLampHost::supportFuncEvents()
{
    return QStringList();
}

