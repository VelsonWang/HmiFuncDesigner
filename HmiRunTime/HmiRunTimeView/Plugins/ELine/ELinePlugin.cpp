#include "ELinePlugin.h"
#include "ElementLine.h"

ELinePlugin::ELinePlugin()
{

}


/**
 * @brief ELinePlugin::initialize
 * @details 插件初始化
 */
void ELinePlugin::initialize()
{

}


/**
 * @brief ELinePlugin::getElementName
 * @details 获取元素名称
 * @return 元素名称
 */
QString ELinePlugin::getElementName()
{
    return tr("直线");
}

/**
 * @brief ELinePlugin::getElementID
 * @details 获取元素ID
 * @return 元素ID
 */
int ELinePlugin::getElementID()
{
    return LineItemType;
}


/**
 * @brief ELine::getElementIDString
 * @details 获取元素ID String
 * @return 元素ID String
 */
QString ELinePlugin::getElementIDString()
{
    return "Line";
}

/**
 * @brief ELinePlugin::createElement
 * @details 创建元素
 * @return 元素对象
 */
Element* ELinePlugin::createElement(QWidget *owner)
{
    ElementLine *pEle = new ElementLine();
    pEle->setOwnerWidget(owner);
    return pEle;
}


