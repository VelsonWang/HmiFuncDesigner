
#include "EArrow.h"
#include "ElementArrow.h"

EArrow::EArrow()
{

}


/**
 * @brief EArrow::initialize
 * @details 初始化
 */
void EArrow::initialize()
{

}

/**
 * @brief EArrow::getElementName
 * @details 获取元素名称
 * @return 元素名称
 */
QString EArrow::getElementName()
{
    return trUtf8("箭头");
}

/**
 * @brief EArrow::getElementID
 * @details 获取元素ID
 * @return 元素ID
 */
int EArrow::getElementID()
{
    return ArrowItemType;
}


/**
 * @brief EArrow::getElementIDString
 * @details 获取元素ID String
 * @return 元素ID String
 */
QString EArrow::getElementIDString()
{
    return "Arrow";
}

/**
 * @brief createElement
 * @details 创建元素
 * @return 元素对象
 */
Element* EArrow::createElement()
{
    return new ElementArrow();
}



