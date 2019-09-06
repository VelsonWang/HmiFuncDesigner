
#include "SwitchButtonPlugin.h"
#include "ElementSwitchButton.h"


SwitchButtonPlugin::SwitchButtonPlugin()
{

}


/**
 * @brief SwitchButtonPlugin::initialize
 * @details 插件初始化
 */
void SwitchButtonPlugin::initialize()
{

}

/**
 * @brief SwitchButtonPlugin::getPluginTypeName
 * @details 获取插件类型名称
 * @return 插件类型名称
 */
QString SwitchButtonPlugin::getPluginTypeName()
{
    return tr("基本控件");
}


/**
 * @brief SwitchButtonPlugin::getPluginName
 * @details 获取插件名称
 * @return 插件名称
 */
QString SwitchButtonPlugin::getPluginName()
{
    return tr("切换按钮");
}


/**
 * @brief SwitchButtonPlugin::getElementIcon
 * @details 获取元素的图标
 * @return 元素图标
 */
QIcon SwitchButtonPlugin::getElementIcon()
{
    QIcon icon;
    icon.addFile(QStringLiteral(":/images/SwitchButton.png"), QSize(), QIcon::Normal, QIcon::Off);
    return icon;
}


/**
 * @brief SwitchButtonPlugin::getElementName
 * @details 获取元素名称
 * @return 元素名称
 */
QString SwitchButtonPlugin::getElementName()
{
    return tr("切换按钮");
}

/**
 * @brief SwitchButtonPlugin::getElementID
 * @details 获取元素ID
 * @return 元素ID
 */
int SwitchButtonPlugin::getElementID()
{
    return SwitchButtonItemType;
}


/**
 * @brief SwitchButtonPlugin::getElementIDString
 * @details 获取元素ID String
 * @return 元素ID String
 */
QString SwitchButtonPlugin::getElementIDString()
{
    return "SwitchButton";
}

/**
 * @brief createElement
 * @details 创建元素
 * @return 元素对象
 */
Element* SwitchButtonPlugin::createElement(const QString &szProjPath,
                                           const QString &szProjName,
                                           QtVariantPropertyManager *propertyMgr)
{
    return new ElementSwitchButton(szProjPath, szProjName, propertyMgr);
}
