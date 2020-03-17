
#include "EValueStick.h"
#include "ElementValueStick.h"


EValueStick::EValueStick()
{

}


/**
 * @brief EValueStick::initialize
 * @details 初始化
 */
void EValueStick::initialize()
{

}


/**
 * @brief EValueStick::getElementName
 * @details 获取元素名称
 * @return 元素名称
 */
QString EValueStick::getElementName()
{
    return tr("数值棒图");
}

/**
 * @brief EValueStick::getElementID
 * @details 获取元素ID
 * @return 元素ID
 */
int EValueStick::getElementID()
{
    return ValueStickItemType;
}


/**
 * @brief EValueStick::getElementIDString
 * @details 获取元素ID String
 * @return 元素ID String
 */
QString EValueStick::getElementIDString()
{
    return "ValueStick";
}

/**
 * @brief createElement
 * @details 创建元素
 * @return 元素对象
 */
Element* EValueStick::createElement(QWidget *owner)
{
    ElementValueStick *pEle = new ElementValueStick();
    pEle->setOwnerWidget(owner);
    return pEle;
}
