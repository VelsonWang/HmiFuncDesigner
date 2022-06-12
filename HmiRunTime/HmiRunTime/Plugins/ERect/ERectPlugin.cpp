
#include "ERectPlugin.h"
#include "ElementRect.h"


ERectPlugin::ERectPlugin()
{

}


/**
 * @brief ERectPlugin::initialize
 * @details 插件初始化
 */
void ERectPlugin::initialize()
{

}

/**
 * @brief ERectPlugin::getElementName
 * @details 获取元素名称
 * @return 元素名称
 */
QString ERectPlugin::getElementName()
{
    return tr("矩形");
}

/**
 * @brief ERectPlugin::getElementID
 * @details 获取元素ID
 * @return 元素ID
 */
int ERectPlugin::getElementID()
{
    return RectItemType;
}

/**
 * @brief ERectPlugin::getElementIDString
 * @details 获取元素ID String
 * @return 元素ID String
 */
QString ERectPlugin::getElementIDString()
{
    return "Rect";
}


/**
 * @brief ERectPlugin::createElement
 * @details 创建元素
 * @return 元素对象
 */
Element* ERectPlugin::createElement(QWidget *owner)
{
    ElementRect *pEle = new ElementRect();
    pEle->setOwnerWidget(owner);
    return pEle;
}


