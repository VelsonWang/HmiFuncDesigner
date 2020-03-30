#include "EValueStickPlugin.h"
#include "ElementValueStick.h"


EValueStickPlugin::EValueStickPlugin()
{

}


/**
 * @brief EValueStickPlugin::initialize
 * @details 插件初始化
 */
void EValueStickPlugin::initialize()
{

}


/**
 * @brief EValueStickPlugin::getElementName
 * @details 获取元素名称
 * @return 元素名称
 */
QString EValueStickPlugin::getElementName()
{
    return tr("数值棒图");
}

/**
 * @brief EValueStickPlugin::getElementID
 * @details 获取元素ID
 * @return 元素ID
 */
int EValueStickPlugin::getElementID()
{
    return ValueStickItemType;
}


/**
 * @brief EValueStickPlugin::getElementIDString
 * @details 获取元素ID String
 * @return 元素ID String
 */
QString EValueStickPlugin::getElementIDString()
{
    return "ValueStick";
}

/**
 * @brief EValueStickPlugin::createElement
 * @details 创建元素
 * @return 元素对象
 */
Element* EValueStickPlugin::createElement(QWidget *owner)
{
    ElementValueStick *pEle = new ElementValueStick();
    pEle->setOwnerWidget(owner);
    return pEle;
}


