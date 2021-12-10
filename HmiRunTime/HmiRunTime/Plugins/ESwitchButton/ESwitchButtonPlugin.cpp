#include "ESwitchButtonPlugin.h"
#include "ElementSwitchButton.h"


ESwitchButtonPlugin::ESwitchButtonPlugin()
{

}


/**
 * @brief ESwitchButtonPlugin::initialize
 * @details 插件初始化
 */
void ESwitchButtonPlugin::initialize()
{

}


/**
 * @brief ESwitchButtonPlugin::getElementName
 * @details 获取元素名称
 * @return 元素名称
 */
QString ESwitchButtonPlugin::getElementName()
{
    return tr("切换按钮");
}

/**
 * @brief ESwitchButtonPlugin::getElementID
 * @details 获取元素ID
 * @return 元素ID
 */
int ESwitchButtonPlugin::getElementID()
{
    return SwitchButtonItemType;
}


/**
 * @brief ESwitchButtonPlugin::getElementIDString
 * @details 获取元素ID String
 * @return 元素ID String
 */
QString ESwitchButtonPlugin::getElementIDString()
{
    return "SwitchButton";
}

/**
 * @brief ESwitchButtonPlugin::createElement
 * @details 创建元素
 * @return 元素对象
 */
Element* ESwitchButtonPlugin::createElement(QWidget *owner)
{
    ElementSwitchButton *pEle = new ElementSwitchButton();
    pEle->setOwnerWidget(owner);
    return pEle;
}



