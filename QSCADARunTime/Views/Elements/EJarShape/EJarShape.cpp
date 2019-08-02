
#include "EJarShape.h"
#include "ElementJarShape.h"


EJarShape::EJarShape()
{

}


/**
 * @brief EJarShape::initialize
 * @details 初始化
 */
void EJarShape::initialize()
{

}


/**
 * @brief EJarShape::getElementName
 * @details 获取元素名称
 * @return 元素名称
 */
QString EJarShape::getElementName()
{
    return tr("罐形容器");
}

/**
 * @brief EJarShape::getElementID
 * @details 获取元素ID
 * @return 元素ID
 */
int EJarShape::getElementID()
{
    return JarShapeItemType;
}


/**
 * @brief EJarShape::getElementIDString
 * @details 获取元素ID String
 * @return 元素ID String
 */
QString EJarShape::getElementIDString()
{
    return "JarShape";
}

/**
 * @brief createElement
 * @details 创建元素
 * @return 元素对象
 */
Element* EJarShape::createElement(QWidget *owner)
{
    ElementJarShape *pEle = new ElementJarShape();
    pEle->setOwnerWidget(owner);
    return pEle;
}
