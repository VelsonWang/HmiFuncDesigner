
#include "EClock.h"
#include "ElementClock.h"


EClock::EClock()
{

}


/**
 * @brief EClock::initialize
 * @details 初始化
 */
void EClock::initialize()
{

}


/**
 * @brief EClock::getElementName
 * @details 获取元素名称
 * @return 元素名称
 */
QString EClock::getElementName()
{
    return tr("时钟");
}

/**
 * @brief EClock::getElementID
 * @details 获取元素ID
 * @return 元素ID
 */
int EClock::getElementID()
{
    return ClockItemType;
}


/**
 * @brief EClock::getElementIDString
 * @details 获取元素ID String
 * @return 元素ID String
 */
QString EClock::getElementIDString()
{
    return "Clock";
}

/**
 * @brief createElement
 * @details 创建元素
 * @return 元素对象
 */
Element* EClock::createElement(QWidget *owner)
{
    ElementClock *pEle = new ElementClock();
    pEle->setOwnerWidget(owner);
    return pEle;
}
