#include "EMovingTextPlugin.h"
#include "ElementMovingText.h"


EMovingTextPlugin::EMovingTextPlugin()
{

}


/**
 * @brief EMovingTextPlugin::initialize
 * @details 插件初始化
 */
void EMovingTextPlugin::initialize()
{

}


/**
 * @brief EMovingTextPlugin::getElementName
 * @details 获取元素名称
 * @return 元素名称
 */
QString EMovingTextPlugin::getElementName()
{
    return tr("移动文本");
}

/**
 * @brief EMovingTextPlugin::getElementID
 * @details 获取元素ID
 * @return 元素ID
 */
int EMovingTextPlugin::getElementID()
{
    return MovingTextItemType;
}


/**
 * @brief EMovingTextPlugin::getElementIDString
 * @details 获取元素ID String
 * @return 元素ID String
 */
QString EMovingTextPlugin::getElementIDString()
{
    return "MovingText";
}

/**
 * @brief EMovingTextPlugin::createElement
 * @details 创建元素
 * @return 元素对象
 */
Element* EMovingTextPlugin::createElement(QWidget *owner)
{
    ElementMovingText *pEle = new ElementMovingText();
    pEle->setOwnerWidget(owner);
    return pEle;
}


