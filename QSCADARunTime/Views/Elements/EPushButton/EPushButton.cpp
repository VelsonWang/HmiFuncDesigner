
#include "EPushButton.h"
#include "ElementPushButton.h"


EPushButton::EPushButton()
{

}


/**
 * @brief EPushButton::initialize
 * @details 初始化
 */
void EPushButton::initialize()
{

}


/**
 * @brief EPushButton::getElementName
 * @details 获取元素名称
 * @return 元素名称
 */
QString EPushButton::getElementName()
{
    return tr("弹出按钮");
}

/**
 * @brief EPushButton::getElementID
 * @details 获取元素ID
 * @return 元素ID
 */
int EPushButton::getElementID()
{
    return PushButtonItemType;
}


/**
 * @brief EPushButton::getElementIDString
 * @details 获取元素ID String
 * @return 元素ID String
 */
QString EPushButton::getElementIDString()
{
    return "PushButton";
}

/**
 * @brief createElement
 * @details 创建元素
 * @return 元素对象
 */
Element* EPushButton::createElement(QWidget *owner)
{
    ElementPushButton *pEle = new ElementPushButton();
    pEle->setOwnerWidget(owner);
    return pEle;
}
