#include "EAnalogClockPlugin.h"
#include "ElementAnalogClock.h"


EAnalogClockPlugin::EAnalogClockPlugin()
{

}


/**
 * @brief EAnalogClockPlugin::initialize
 * @details 插件初始化
 */
void EAnalogClockPlugin::initialize()
{

}

/**
 * @brief EAnalogClockPlugin::getElementName
 * @details 获取元素名称
 * @return 元素名称
 */
QString EAnalogClockPlugin::getElementName()
{
    return tr("模拟时钟");
}

/**
 * @brief EAnalogClockPlugin::getElementID
 * @details 获取元素ID
 * @return 元素ID
 */
int EAnalogClockPlugin::getElementID()
{
    return AnalogClockItemType;
}

/**
 * @brief EAnalogClockPlugin::getElementIDString
 * @details 获取元素ID String
 * @return 元素ID String
 */
QString EAnalogClockPlugin::getElementIDString()
{
    return "AnalogClock";
}

/**
 * @brief EAnalogClockPlugin::createElement
 * @details 创建元素
 * @return 元素对象
 */
Element* EAnalogClockPlugin::createElement(QWidget *owner)
{
    ElementAnalogClock *pEle = new ElementAnalogClock();
    pEle->setOwnerWidget(owner);
    return pEle;
}









