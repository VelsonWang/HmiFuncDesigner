#include "ETextPlugin.h"
#include "ElementText.h"


ETextPlugin::ETextPlugin()
{

}


/**
 * @brief ETextPlugin::initialize
 * @details 插件初始化
 */
void ETextPlugin::initialize()
{

}


/**
 * @brief ETextPlugin::getElementName
 * @details 获取元素名称
 * @return 元素名称
 */
QString ETextPlugin::getElementName()
{
    return tr("文本");
}

/**
 * @brief ETextPlugin::getElementID
 * @details 获取元素ID
 * @return 元素ID
 */
int ETextPlugin::getElementID()
{
    return TextItemType;
}


/**
 * @brief ETextPlugin::getElementIDString
 * @details 获取元素ID String
 * @return 元素ID String
 */
QString ETextPlugin::getElementIDString()
{
    return "Text";
}

/**
 * @brief ETextPlugin::createElement
 * @details 创建元素
 * @return 元素对象
 */
Element* ETextPlugin::createElement(QWidget *owner)
{
    ElementText *pEle = new ElementText();
    pEle->setOwnerWidget(owner);
    return pEle;
}


