
#include "PushButtonPlugin.h"
#include "ElementPushButton.h"


PushButtonPlugin::PushButtonPlugin()
{

}


/**
 * @brief PushButtonPlugin::initialize
 * @details 插件初始化
 */
void PushButtonPlugin::initialize()
{

}

/**
 * @brief PushButtonPlugin::getPluginTypeName
 * @details 获取插件类型名称
 * @return 插件类型名称
 */
QString PushButtonPlugin::getPluginTypeName()
{
    return tr("基本控件");
}


/**
 * @brief PushButtonPlugin::getPluginName
 * @details 获取插件名称
 * @return 插件名称
 */
QString PushButtonPlugin::getPluginName()
{
    return tr("弹出按钮");
}


/**
 * @brief PushButtonPlugin::getElementIcon
 * @details 获取元素的图标
 * @return 元素图标
 */
QIcon PushButtonPlugin::getElementIcon()
{
    QIcon icon;
    icon.addFile(QStringLiteral(":/images/PushButton.png"), QSize(), QIcon::Normal, QIcon::Off);
    return icon;
}


/**
 * @brief PushButtonPlugin::getElementName
 * @details 获取元素名称
 * @return 元素名称
 */
QString PushButtonPlugin::getElementName()
{
    return tr("弹出按钮");
}

/**
 * @brief PushButtonPlugin::getElementID
 * @details 获取元素ID
 * @return 元素ID
 */
int PushButtonPlugin::getElementID()
{
    return PushButtonItemType;
}


/**
 * @brief PushButtonPlugin::getElementIDString
 * @details 获取元素ID String
 * @return 元素ID String
 */
QString PushButtonPlugin::getElementIDString()
{
    return "PushButton";
}

/**
 * @brief createElement
 * @details 创建元素
 * @return 元素对象
 */
Element* PushButtonPlugin::createElement(const QString &szProjPath,
                                         const QString &szProjName,
                                         QtVariantPropertyManager *propertyMgr)
{
    return new ElementPushButton(szProjPath, szProjName, propertyMgr);
}
