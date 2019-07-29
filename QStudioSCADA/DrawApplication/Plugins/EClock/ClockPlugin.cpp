
#include "ClockPlugin.h"
#include "ElementClock.h"


ClockPlugin::ClockPlugin()
{

}


/**
 * @brief ClockPlugin::initialize
 * @details 插件初始化
 */
void ClockPlugin::initialize()
{

}

/**
 * @brief ClockPlugin::getPluginTypeName
 * @details 获取插件类型名称
 * @return 插件类型名称
 */
QString ClockPlugin::getPluginTypeName()
{
    return tr("基本控件");
}


/**
 * @brief ClockPlugin::getPluginName
 * @details 获取插件名称
 * @return 插件名称
 */
QString ClockPlugin::getPluginName()
{
    return tr("时钟");
}


/**
 * @brief ClockPlugin::getElementIcon
 * @details 获取元素的图标
 * @return 元素图标
 */
QIcon ClockPlugin::getElementIcon()
{
    QIcon icon;
    icon.addFile(QStringLiteral(":/images/Clock.png"), QSize(), QIcon::Normal, QIcon::Off);
    return icon;
}


/**
 * @brief ClockPlugin::getElementName
 * @details 获取元素名称
 * @return 元素名称
 */
QString ClockPlugin::getElementName()
{
    return tr("时钟");
}

/**
 * @brief ClockPlugin::getElementID
 * @details 获取元素ID
 * @return 元素ID
 */
int ClockPlugin::getElementID()
{
    return ClockItemType;
}


/**
 * @brief ClockPlugin::getElementIDString
 * @details 获取元素ID String
 * @return 元素ID String
 */
QString ClockPlugin::getElementIDString()
{
    return "Clock";
}

/**
 * @brief createElement
 * @details 创建元素
 * @return 元素对象
 */
Element* ClockPlugin::createElement(const QString &szProjPath, const QString &szProjName)
{
    return new ElementClock(szProjPath, szProjName);
}
