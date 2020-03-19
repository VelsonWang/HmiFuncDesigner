#include "EllipsePlugin.h"
#include "ElementEllipse.h"


EllipsePlugin::EllipsePlugin()
{

}


/**
 * @brief ELinePlugin::initialize
 * @details 插件初始化
 */
void EllipsePlugin::initialize()
{

}


/**
 * @brief EllipsePlugin::getElementName
 * @details 获取元素名称
 * @return 元素名称
 */
QString EllipsePlugin::getElementName()
{
    return tr("椭圆形");
}

/**
 * @brief EllipsePlugin::getElementID
 * @details 获取元素ID
 * @return 元素ID
 */
int EllipsePlugin::getElementID()
{
    return EllipseItemType;
}


/**
 * @brief EllipsePlugin::getElementIDString
 * @details 获取元素ID String
 * @return 元素ID String
 */
QString EllipsePlugin::getElementIDString()
{
    return "Ellipse";
}

/**
 * @brief EllipsePlugin::createElement
 * @details 创建元素
 * @return 元素对象
 */
Element* EllipsePlugin::createElement(QWidget *owner)
{
    ElementEllipse *pEle = new ElementEllipse();
    pEle->setOwnerWidget(owner);
    return pEle;
}
