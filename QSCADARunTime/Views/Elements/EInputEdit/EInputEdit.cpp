
#include "EInputEdit.h"
#include "ElementInputEdit.h"


EInputEdit::EInputEdit()
{

}


/**
 * @brief EInputEdit::initialize
 * @details 初始化
 */
void EInputEdit::initialize()
{

}


/**
 * @brief EInputEdit::getElementName
 * @details 获取元素名称
 * @return 元素名称
 */
QString EInputEdit::getElementName()
{
    return trUtf8("输入编辑框");
}

/**
 * @brief EInputEdit::getElementID
 * @details 获取元素ID
 * @return 元素ID
 */
int EInputEdit::getElementID()
{
    return InputEditItemType;
}


/**
 * @brief EInputEdit::getElementIDString
 * @details 获取元素ID String
 * @return 元素ID String
 */
QString EInputEdit::getElementIDString()
{
    return "InputEdit";
}

/**
 * @brief createElement
 * @details 创建元素
 * @return 元素对象
 */
Element* EInputEdit::createElement()
{
    return new ElementInputEdit();
}
