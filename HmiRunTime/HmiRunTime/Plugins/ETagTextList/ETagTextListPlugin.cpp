#include "ETagTextListPlugin.h"
#include "ElementTagTextList.h"


ETagTextListPlugin::ETagTextListPlugin()
{

}


/**
 * @brief ETagTextListPlugin::initialize
 * @details 插件初始化
 */
void ETagTextListPlugin::initialize()
{

}


/**
 * @brief ETagTextListPlugin::getElementName
 * @details 获取元素名称
 * @return 元素名称
 */
QString ETagTextListPlugin::getElementName()
{
    return tr("变量文本列表");
}

/**
 * @brief ETagTextListPlugin::getElementID
 * @details 获取元素ID
 * @return 元素ID
 */
int ETagTextListPlugin::getElementID()
{
    return TagTextListItemType;
}


/**
 * @brief ETagTextListPlugin::getElementIDString
 * @details 获取元素ID String
 * @return 元素ID String
 */
QString ETagTextListPlugin::getElementIDString()
{
    return "TagTextList";
}

/**
 * @brief ETagTextListPlugin::createElement
 * @details 创建元素
 * @return 元素对象
 */
Element* ETagTextListPlugin::createElement(QWidget *owner)
{
    ElementTagTextList *pEle = new ElementTagTextList();
    pEle->setOwnerWidget(owner);
    return pEle;
}
