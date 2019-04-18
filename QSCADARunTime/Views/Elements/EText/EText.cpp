
#include "EText.h"
#include "ElementText.h"


EText::EText()
{

}


/**
 * @brief EText::initialize
 * @details 初始化
 */
void EText::initialize()
{

}


/**
 * @brief EText::getElementName
 * @details 获取元素名称
 * @return 元素名称
 */
QString EText::getElementName()
{
    return trUtf8("文本");
}

/**
 * @brief EText::getElementID
 * @details 获取元素ID
 * @return 元素ID
 */
int EText::getElementID()
{
    return TextItemType;
}


/**
 * @brief EText::getElementIDString
 * @details 获取元素ID String
 * @return 元素ID String
 */
QString EText::getElementIDString()
{
    return "Text";
}

/**
 * @brief createElement
 * @details 创建元素
 * @return 元素对象
 */
Element* EText::createElement(QWidget *owner)
{
    ElementText *pEle = new ElementText();
    pEle->setOwnerWidget(owner);
    return pEle;
}
