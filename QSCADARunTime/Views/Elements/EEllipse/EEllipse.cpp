
#include "EEllipse.h"
#include "ElementEllipse.h"


EEllipse::EEllipse()
{

}


/**
 * @brief EEllipse::initialize
 * @details 初始化
 */
void EEllipse::initialize()
{

}


/**
 * @brief EEllipse::getElementName
 * @details 获取元素名称
 * @return 元素名称
 */
QString EEllipse::getElementName()
{
    return tr("椭圆形");
}

/**
 * @brief EEllipse::getElementID
 * @details 获取元素ID
 * @return 元素ID
 */
int EEllipse::getElementID()
{
    return EllipseItemType;
}


/**
 * @brief EEllipse::getElementIDString
 * @details 获取元素ID String
 * @return 元素ID String
 */
QString EEllipse::getElementIDString()
{
    return "EEllipse";
}

/**
 * @brief createElement
 * @details 创建元素
 * @return 元素对象
 */
Element* EEllipse::createElement(QWidget *owner)
{
    ElementEllipse *pEle = new ElementEllipse();
    pEle->setOwnerWidget(owner);
    return pEle;
}
