
#include "RectPlugin.h"
#include "ElementRect.h"


RectPlugin::RectPlugin()
{

}


/**
 * @brief RectPlugin::initialize
 * @details 插件初始化
 */
void RectPlugin::initialize()
{

}

/**
 * @brief RectPlugin::getPluginTypeName
 * @details 获取插件类型名称
 * @return 插件类型名称
 */
QString RectPlugin::getPluginTypeName()
{
    return trUtf8("基本图元");
}


/**
 * @brief RectPlugin::getPluginName
 * @details 获取插件名称
 * @return 插件名称
 */
QString RectPlugin::getPluginName()
{
    return trUtf8("矩形");
}


/**
 * @brief RectPlugin::getElementIcon
 * @details 获取元素的图标
 * @return 元素图标
 */
QIcon RectPlugin::getElementIcon()
{
    QIcon icon;
    icon.addFile(QStringLiteral(":/images/rectitem.png"), QSize(), QIcon::Normal, QIcon::Off);
    return icon;
}


/**
 * @brief RectPlugin::getElementName
 * @details 获取元素名称
 * @return 元素名称
 */
QString RectPlugin::getElementName()
{
    return trUtf8("矩形");
}

/**
 * @brief RectPlugin::getElementID
 * @details 获取元素ID
 * @return 元素ID
 */
int RectPlugin::getElementID()
{
    return RectItemType;
}


/**
 * @brief RectPlugin::getElementIDString
 * @details 获取元素ID String
 * @return 元素ID String
 */
QString RectPlugin::getElementIDString()
{
    return "Rect";
}

/**
 * @brief createElement
 * @details 创建元素
 * @return 元素对象
 */
Element* RectPlugin::createElement(const QString &szProjPath, const QString &szProjName)
{
    return new ElementRect(szProjPath, szProjName);
}
