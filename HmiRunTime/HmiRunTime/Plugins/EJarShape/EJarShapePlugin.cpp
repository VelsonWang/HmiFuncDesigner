#include "EJarShapePlugin.h"
#include "ElementJarShape.h"


EJarShapePlugin::EJarShapePlugin()
{

}


/**
 * @brief EJarShapePlugin::initialize
 * @details 插件初始化
 */
void EJarShapePlugin::initialize()
{

}


/**
 * @brief EJarShapePlugin::getElementName
 * @details 获取元素名称
 * @return 元素名称
 */
QString EJarShapePlugin::getElementName()
{
    return tr("罐形容器");
}

/**
 * @brief EJarShapePlugin::getElementID
 * @details 获取元素ID
 * @return 元素ID
 */
int EJarShapePlugin::getElementID()
{
    return JarShapeItemType;
}


/**
 * @brief EJarShapePlugin::getElementIDString
 * @details 获取元素ID String
 * @return 元素ID String
 */
QString EJarShapePlugin::getElementIDString()
{
    return "JarShape";
}

/**
 * @brief EJarShapePlugin::createElement
 * @details 创建元素
 * @return 元素对象
 */
Element* EJarShapePlugin::createElement(QWidget *owner)
{
    ElementJarShape *pEle = new ElementJarShape();
    pEle->setOwnerWidget(owner);
    return pEle;
}


