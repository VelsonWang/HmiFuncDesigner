#include "EClockPlugin.h"
#include "ElementClock.h"


EClockPlugin::EClockPlugin()
{

}


/**
 * @brief EClockPlugin::initialize
 * @details 插件初始化
 */
void EClockPlugin::initialize()
{

}


/**
 * @brief EClockPlugin::getElementName
 * @details 获取元素名称
 * @return 元素名称
 */
QString EClockPlugin::getElementName()
{
    return tr("时钟");
}

/**
 * @brief EClockPlugin::getElementID
 * @details 获取元素ID
 * @return 元素ID
 */
int EClockPlugin::getElementID()
{
    return ClockItemType;
}


/**
 * @brief EClockPlugin::getElementIDString
 * @details 获取元素ID String
 * @return 元素ID String
 */
QString EClockPlugin::getElementIDString()
{
    return "Clock";
}

/**
 * @brief EClockPlugin::createElement
 * @details 创建元素
 * @return 元素对象
 */
Element* EClockPlugin::createElement(QWidget *owner)
{
    ElementClock *pEle = new ElementClock();
    pEle->setOwnerWidget(owner);
    return pEle;
}


