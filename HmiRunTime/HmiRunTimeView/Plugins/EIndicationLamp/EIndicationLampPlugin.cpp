#include "EIndicationLampPlugin.h"
#include "ElementIndicationLamp.h"
#include "ProjectData.h"


EIndicationLampPlugin::EIndicationLampPlugin()
{

}


/**
 * @brief EIndicationLampPlugin::initialize
 * @details 插件初始化
 */
void EIndicationLampPlugin::initialize()
{

}


/**
 * @brief EIndicationLampPlugin::getElementName
 * @details 获取元素名称
 * @return 元素名称
 */
QString EIndicationLampPlugin::getElementName()
{
    return tr("指示灯");
}

/**
 * @brief EIndicationLampPlugin::getElementID
 * @details 获取元素ID
 * @return 元素ID
 */
int EIndicationLampPlugin::getElementID()
{
    return IndicationLampItemType;
}


/**
 * @brief EIndicationLampPlugin::getElementIDString
 * @details 获取元素ID String
 * @return 元素ID String
 */
QString EIndicationLampPlugin::getElementIDString()
{
    return "IndicationLamp";
}

/**
 * @brief EIndicationLampPlugin::createElement
 * @details 创建元素
 * @return 元素对象
 */
Element* EIndicationLampPlugin::createElement(QWidget *owner)
{
    ElementIndicationLamp *pEle = new ElementIndicationLamp();
    pEle->setOwnerWidget(owner);
    return pEle;
}

