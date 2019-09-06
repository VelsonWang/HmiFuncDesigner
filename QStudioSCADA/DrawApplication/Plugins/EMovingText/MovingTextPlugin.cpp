
#include "MovingTextPlugin.h"
#include "ElementMovingText.h"


MovingTextPlugin::MovingTextPlugin()
{

}


/**
 * @brief MovingTextPlugin::initialize
 * @details 插件初始化
 */
void MovingTextPlugin::initialize()
{

}

/**
 * @brief MovingTextPlugin::getPluginTypeName
 * @details 获取插件类型名称
 * @return 插件类型名称
 */
QString MovingTextPlugin::getPluginTypeName()
{
    return tr("基本控件");
}


/**
 * @brief MovingTextPlugin::getPluginName
 * @details 获取插件名称
 * @return 插件名称
 */
QString MovingTextPlugin::getPluginName()
{
    return tr("移动文本");
}


/**
 * @brief MovingTextPlugin::getElementIcon
 * @details 获取元素的图标
 * @return 元素图标
 */
QIcon MovingTextPlugin::getElementIcon()
{
    QIcon icon;
    icon.addFile(QStringLiteral(":/images/MovingText.png"), QSize(), QIcon::Normal, QIcon::Off);
    return icon;
}


/**
 * @brief MovingTextPlugin::getElementName
 * @details 获取元素名称
 * @return 元素名称
 */
QString MovingTextPlugin::getElementName()
{
    return tr("移动文本");
}

/**
 * @brief MovingTextPlugin::getElementID
 * @details 获取元素ID
 * @return 元素ID
 */
int MovingTextPlugin::getElementID()
{
    return MovingTextItemType;
}


/**
 * @brief MovingTextPlugin::getElementIDString
 * @details 获取元素ID String
 * @return 元素ID String
 */
QString MovingTextPlugin::getElementIDString()
{
    return "MovingText";
}

/**
 * @brief createElement
 * @details 创建元素
 * @return 元素对象
 */
Element* MovingTextPlugin::createElement(const QString &szProjPath,
                                         const QString &szProjName,
                                         QtVariantPropertyManager *propertyMgr)
{
    return new ElementMovingText(szProjPath, szProjName, propertyMgr);
}
