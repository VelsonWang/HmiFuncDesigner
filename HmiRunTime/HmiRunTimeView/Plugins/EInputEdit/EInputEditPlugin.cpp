#include "EInputEditPlugin.h"
#include "ElementInputEdit.h"



EInputEditPlugin::EInputEditPlugin()
{

}


/**
 * @brief EInputEditPlugin::initialize
 * @details 插件初始化
 */
void EInputEditPlugin::initialize()
{

}


/**
 * @brief EInputEditPlugin::getElementName
 * @details 获取元素名称
 * @return 元素名称
 */
QString EInputEditPlugin::getElementName()
{
    return tr("输入编辑框");
}

/**
 * @brief EInputEditPlugin::getElementID
 * @details 获取元素ID
 * @return 元素ID
 */
int EInputEditPlugin::getElementID()
{
    return InputEditItemType;
}


/**
 * @brief EInputEditPlugin::getElementIDString
 * @details 获取元素ID String
 * @return 元素ID String
 */
QString EInputEditPlugin::getElementIDString()
{
    return "InputEdit";
}

/**
 * @brief EInputEditPlugin::createElement
 * @details 创建元素
 * @return 元素对象
 */
Element* EInputEditPlugin::createElement(QWidget *owner)
{
    ElementInputEdit *pEle = new ElementInputEdit();
    pEle->setOwnerWidget(owner);
    return pEle;
}


