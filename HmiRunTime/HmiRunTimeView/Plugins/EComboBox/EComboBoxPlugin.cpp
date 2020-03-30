
#include "EComboBoxPlugin.h"
#include "ElementComboBox.h"


EComboBoxPlugin::EComboBoxPlugin()
{

}


/**
 * @brief EComboBoxPlugin::initialize
 * @details 插件初始化
 */
void EComboBoxPlugin::initialize()
{

}


/**
 * @brief EComboBoxPlugin::getElementName
 * @details 获取元素名称
 * @return 元素名称
 */
QString EComboBoxPlugin::getElementName()
{
    return tr("下拉菜单列表");
}

/**
 * @brief EComboBoxPlugin::getElementID
 * @details 获取元素ID
 * @return 元素ID
 */
int EComboBoxPlugin::getElementID()
{
    return ComboBoxItemType;
}


/**
 * @brief EComboBoxPlugin::getElementIDString
 * @details 获取元素ID String
 * @return 元素ID String
 */
QString EComboBoxPlugin::getElementIDString()
{
    return "ComboBox";
}

/**
 * @brief EComboBoxPlugin::createElement
 * @details 创建元素
 * @return 元素对象
 */
Element* EComboBoxPlugin::createElement(QWidget *owner)
{
    ElementComboBox *pEle = new ElementComboBox();
    pEle->setOwnerWidget(owner);
    return pEle;
}



