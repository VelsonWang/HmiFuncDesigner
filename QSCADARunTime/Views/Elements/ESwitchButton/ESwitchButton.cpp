
#include "ESwitchButton.h"
#include "ElementSwitchButton.h"


ESwitchButton::ESwitchButton()
{

}


/**
 * @brief ESwitchButton::initialize
 * @details 初始化
 */
void ESwitchButton::initialize()
{

}


/**
 * @brief ESwitchButton::getElementName
 * @details 获取元素名称
 * @return 元素名称
 */
QString ESwitchButton::getElementName()
{
    return tr("切换按钮");
}

/**
 * @brief ESwitchButton::getElementID
 * @details 获取元素ID
 * @return 元素ID
 */
int ESwitchButton::getElementID()
{
    return SwitchButtonItemType;
}


/**
 * @brief ESwitchButton::getElementIDString
 * @details 获取元素ID String
 * @return 元素ID String
 */
QString ESwitchButton::getElementIDString()
{
    return "SwitchButton";
}

/**
 * @brief createElement
 * @details 创建元素
 * @return 元素对象
 */
Element* ESwitchButton::createElement(QWidget *owner)
{
    ElementSwitchButton *pEle = new ElementSwitchButton();
    pEle->setOwnerWidget(owner);
    return pEle;
}
