
#include "ETagTextList.h"
#include "ElementTagTextList.h"


ETagTextList::ETagTextList()
{

}


/**
 * @brief ETagTextList::initialize
 * @details 初始化
 */
void ETagTextList::initialize()
{

}


/**
 * @brief ETagTextList::getElementName
 * @details 获取元素名称
 * @return 元素名称
 */
QString ETagTextList::getElementName()
{
    return tr("变量文本列表");
}

/**
 * @brief ETagTextList::getElementID
 * @details 获取元素ID
 * @return 元素ID
 */
int ETagTextList::getElementID()
{
    return TagTextListItemType;
}


/**
 * @brief ETagTextList::getElementIDString
 * @details 获取元素ID String
 * @return 元素ID String
 */
QString ETagTextList::getElementIDString()
{
    return "TagTextList";
}

/**
 * @brief createElement
 * @details 创建元素
 * @return 元素对象
 */
Element* ETagTextList::createElement(QWidget *owner)
{
    ElementTagTextList *pEle = new ElementTagTextList();
    pEle->setOwnerWidget(owner);
    return pEle;
}
