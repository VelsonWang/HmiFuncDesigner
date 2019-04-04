
#include "PolygonPlugin.h"
#include "ElementPolygon.h"


PolygonPlugin::PolygonPlugin()
{

}


/**
 * @brief PolygonPlugin::initialize
 * @details 插件初始化
 */
void PolygonPlugin::initialize()
{

}

/**
 * @brief PolygonPlugin::getPluginTypeName
 * @details 获取插件类型名称
 * @return 插件类型名称
 */
QString PolygonPlugin::getPluginTypeName()
{
    return trUtf8("基本图元");
}


/**
 * @brief PolygonPlugin::getPluginName
 * @details 获取插件名称
 * @return 插件名称
 */
QString PolygonPlugin::getPluginName()
{
    return trUtf8("多边形");
}


/**
 * @brief PolygonPlugin::getElementIcon
 * @details 获取元素的图标
 * @return 元素图标
 */
QIcon PolygonPlugin::getElementIcon()
{
    QIcon icon;
    icon.addFile(QStringLiteral(":/images/polygon.png"), QSize(), QIcon::Normal, QIcon::Off);
    return icon;
}


/**
 * @brief PolygonPlugin::getElementName
 * @details 获取元素名称
 * @return 元素名称
 */
QString PolygonPlugin::getElementName()
{
    return trUtf8("多边形");
}

/**
 * @brief PolygonPlugin::getElementID
 * @details 获取元素ID
 * @return 元素ID
 */
int PolygonPlugin::getElementID()
{
    return PolygonItemType;
}


/**
 * @brief PolygonPlugin::getElementIDString
 * @details 获取元素ID String
 * @return 元素ID String
 */
QString PolygonPlugin::getElementIDString()
{
    return "Polygon";
}

/**
 * @brief createElement
 * @details 创建元素
 * @return 元素对象
 */
Element* PolygonPlugin::createElement(const QString &projPath)
{
    return new ElementPolygon(projPath);
}
