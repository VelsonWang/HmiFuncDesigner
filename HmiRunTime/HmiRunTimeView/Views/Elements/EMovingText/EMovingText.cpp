
#include "EMovingText.h"
#include "ElementMovingText.h"


EMovingText::EMovingText()
{

}


/**
 * @brief EMovingText::initialize
 * @details 初始化
 */
void EMovingText::initialize()
{

}


/**
 * @brief EMovingText::getElementName
 * @details 获取元素名称
 * @return 元素名称
 */
QString EMovingText::getElementName()
{
    return tr("移动文本");
}

/**
 * @brief EMovingText::getElementID
 * @details 获取元素ID
 * @return 元素ID
 */
int EMovingText::getElementID()
{
    return MovingTextItemType;
}


/**
 * @brief EMovingText::getElementIDString
 * @details 获取元素ID String
 * @return 元素ID String
 */
QString EMovingText::getElementIDString()
{
    return "MovingText";
}

/**
 * @brief createElement
 * @details 创建元素
 * @return 元素对象
 */
Element* EMovingText::createElement(QWidget *owner)
{
    ElementMovingText *pEle = new ElementMovingText();
    pEle->setOwnerWidget(owner);
    return pEle;
}
