
#include "EArrowPlugin.h"
#include "ElementArrow.h"


EArrowPlugin::EArrowPlugin()
{

}


/**
 * @brief EArrowPlugin::initialize
 * @details 插件初始化
 */
void EArrowPlugin::initialize()
{

}

/**
 * @brief EArrowPlugin::getPluginTypeName
 * @details 获取插件类型名称
 * @return 插件类型名称
 */
QString EArrowPlugin::getPluginTypeName()
{
    return trUtf8("基本图元");
}


/**
 * @brief EArrowPlugin::getPluginName
 * @details 获取插件名称
 * @return 插件名称
 */
QString EArrowPlugin::getPluginName()
{
    return trUtf8("箭头");
}


/**
 * @brief EArrowPlugin::getElementIcon
 * @details 获取元素的图标
 * @return 元素图标
 */
QIcon EArrowPlugin::getElementIcon()
{
    QIcon icon;
    icon.addFile(QStringLiteral(":/images/arrowitem.png"), QSize(), QIcon::Normal, QIcon::Off);
    return icon;
}


/**
 * @brief EArrowPlugin::getElementName
 * @details 获取元素名称
 * @return 元素名称
 */
QString EArrowPlugin::getElementName()
{
    return trUtf8("箭头");
}

/**
 * @brief EArrowPlugin::getElementID
 * @details 获取元素ID
 * @return 元素ID
 */
int EArrowPlugin::getElementID()
{
    return ArrowItemType;
}


/**
 * @brief EArrowPlugin::getElementIDString
 * @details 获取元素ID String
 * @return 元素ID String
 */
QString EArrowPlugin::getElementIDString()
{
    return "Arrow";
}

/**
 * @brief createElement
 * @details 创建元素
 * @return 元素对象
 */
Element* EArrowPlugin::createElement()
{
    return new ElementArrow();
}
