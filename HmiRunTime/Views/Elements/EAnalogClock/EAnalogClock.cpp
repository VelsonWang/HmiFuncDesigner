
#include "EAnalogClock.h"
#include "ElementAnalogClock.h"


EAnalogClock::EAnalogClock()
{

}


/**
 * @brief EAnalogClock::initialize
 * @details 初始化
 */
void EAnalogClock::initialize()
{

}


/**
 * @brief EAnalogClock::getElementName
 * @details 获取元素名称
 * @return 元素名称
 */
QString EAnalogClock::getElementName()
{
    return tr("模拟时钟");
}

/**
 * @brief EAnalogClock::getElementID
 * @details 获取元素ID
 * @return 元素ID
 */
int EAnalogClock::getElementID()
{
    return AnalogClockItemType;
}


/**
 * @brief EAnalogClock::getElementIDString
 * @details 获取元素ID String
 * @return 元素ID String
 */
QString EAnalogClock::getElementIDString()
{
    return "AnalogClock";
}

/**
 * @brief createElement
 * @details 创建元素
 * @return 元素对象
 */
Element* EAnalogClock::createElement(QWidget *owner)
{
    ElementAnalogClock *pEle = new ElementAnalogClock();
    pEle->setOwnerWidget(owner);
    return pEle;
}
