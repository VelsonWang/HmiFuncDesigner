
#include "EComboBox.h"
#include "ElementComboBox.h"


EComboBox::EComboBox()
{

}


/**
 * @brief EComboBox::initialize
 * @details 初始化
 */
void EComboBox::initialize()
{

}


/**
 * @brief EComboBox::getElementName
 * @details 获取元素名称
 * @return 元素名称
 */
QString EComboBox::getElementName()
{
    return tr("下拉菜单列表");
}

/**
 * @brief EComboBox::getElementID
 * @details 获取元素ID
 * @return 元素ID
 */
int EComboBox::getElementID()
{
    return ComboBoxItemType;
}


/**
 * @brief EComboBox::getElementIDString
 * @details 获取元素ID String
 * @return 元素ID String
 */
QString EComboBox::getElementIDString()
{
    return "ComboBox";
}

/**
 * @brief createElement
 * @details 创建元素
 * @return 元素对象
 */
Element* EComboBox::createElement(QWidget *owner)
{
    ElementComboBox *pEle = new ElementComboBox();
    pEle->setOwnerWidget(owner);
    return pEle;
}
