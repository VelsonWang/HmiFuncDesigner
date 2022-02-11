#include "EPushButtonPlugin.h"
#include "ElementPushButton.h"



EPushButtonPlugin::EPushButtonPlugin()
{

}


/**
 * @brief EPushButtonPlugin::initialize
 * @details 插件初始化
 */
void EPushButtonPlugin::initialize()
{

}


/**
 * @brief EPushButtonPlugin::getElementName
 * @details 获取元素名称
 * @return 元素名称
 */
QString EPushButtonPlugin::getElementName()
{
    return tr("弹出按钮");
}

/**
 * @brief EPushButtonPlugin::getElementID
 * @details 获取元素ID
 * @return 元素ID
 */
int EPushButtonPlugin::getElementID()
{
    return PushButtonItemType;
}


/**
 * @brief EPushButtonPlugin::getElementIDString
 * @details 获取元素ID String
 * @return 元素ID String
 */
QString EPushButtonPlugin::getElementIDString()
{
    return "PushButton";
}

/**
 * @brief EPushButtonPlugin::createElement
 * @details 创建元素
 * @return 元素对象
 */
Element* EPushButtonPlugin::createElement(QWidget *owner)
{
    ElementPushButton *pEle = new ElementPushButton();
    pEle->setOwnerWidget(owner);
    return pEle;
}




