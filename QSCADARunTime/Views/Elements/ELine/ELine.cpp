
#include "ELine.h"
#include "ElementLine.h"


ELine::ELine()
{

}


/**
 * @brief ELine::initialize
 * @details 插件初始化
 */
void ELine::initialize()
{

}


/**
 * @brief ELine::getElementName
 * @details 获取元素名称
 * @return 元素名称
 */
QString ELine::getElementName()
{
    return tr("直线");
}

/**
 * @brief ELine::getElementID
 * @details 获取元素ID
 * @return 元素ID
 */
int ELine::getElementID()
{
    return LineItemType;
}


/**
 * @brief ELine::getElementIDString
 * @details 获取元素ID String
 * @return 元素ID String
 */
QString ELine::getElementIDString()
{
    return "Line";
}

/**
 * @brief createElement
 * @details 创建元素
 * @return 元素对象
 */
Element* ELine::createElement(QWidget *owner)
{
    ElementLine *pEle = new ElementLine();
    pEle->setOwnerWidget(owner);
    return pEle;
}
