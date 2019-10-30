
#include "EllipsePlugin.h"
#include "ElementEllipse.h"


EllipsePlugin::EllipsePlugin()
{

}


/**
 * @brief EllipsePlugin::initialize
 * @details 插件初始化
 */
void EllipsePlugin::initialize()
{

}

/**
 * @brief EllipsePlugin::getPluginTypeName
 * @details 获取插件类型名称
 * @return 插件类型名称
 */
QString EllipsePlugin::getPluginTypeName()
{
    return tr("基本图元");
}


/**
 * @brief EllipsePlugin::getPluginName
 * @details 获取插件名称
 * @return 插件名称
 */
QString EllipsePlugin::getPluginName()
{
    return tr("椭圆形");
}


/**
 * @brief EllipsePlugin::getElementIcon
 * @details 获取元素的图标
 * @return 元素图标
 */
QIcon EllipsePlugin::getElementIcon()
{
    QIcon icon;
    icon.addFile(QStringLiteral(":/images/ellipseitem.png"), QSize(), QIcon::Normal, QIcon::Off);
    return icon;
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
 * @brief createElement
 * @details 创建元素
 * @return 元素对象
 */
Element* EllipsePlugin::createElement(const QString &szProjPath,
                                      const QString &szProjName,
                                      QtVariantPropertyManager *propertyMgr,
                                      ProjectData *pProjDataObj)
{
    return new ElementEllipse(szProjPath, szProjName, propertyMgr, pProjDataObj);
}
