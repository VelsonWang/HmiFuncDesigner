#include "EArrowPlugin.h"
#include "ElementArrow.h"


EArrowPlugin::EArrowPlugin()
{

}


/**
 * @brief EArrowPlugin::initialize
 * @details 插件初始化
 */
void EArrowPlugin::initialize()
{

}


/**
 * @brief EArrowPlugin::getElementName
 * @details 获取元素名称
 * @return 元素名称
 */
QString EArrowPlugin::getElementName()
{
    return tr("箭头");
}

/**
 * @brief EArrowPlugin::getElementID
 * @details 获取元素ID
 * @return 元素ID
 */
int EArrowPlugin::getElementID()
{
    return ArrowItemType;
}


/**
 * @brief EArrowPlugin::getElementIDString
 * @details 获取元素ID String
 * @return 元素ID String
 */
QString EArrowPlugin::getElementIDString()
{
    return "Arrow";
}

/**
 * @brief EArrowPlugin::createElement
 * @details 创建元素
 * @return 元素对象
 */
Element* EArrowPlugin::createElement(QWidget *owner)
{
    ElementArrow *pEle = new ElementArrow();
    pEle->setOwnerWidget(owner);
    return pEle;
}


