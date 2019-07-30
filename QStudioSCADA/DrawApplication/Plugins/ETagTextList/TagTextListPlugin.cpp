
#include "TagTextListPlugin.h"
#include "ElementTagTextList.h"


TagTextListPlugin::TagTextListPlugin()
{

}


/**
 * @brief TagTextListPlugin::initialize
 * @details 插件初始化
 */
void TagTextListPlugin::initialize()
{

}

/**
 * @brief TagTextListPlugin::getPluginTypeName
 * @details 获取插件类型名称
 * @return 插件类型名称
 */
QString TagTextListPlugin::getPluginTypeName()
{
    return tr("基本控件");
}


/**
 * @brief TagTextListPlugin::getPluginName
 * @details 获取插件名称
 * @return 插件名称
 */
QString TagTextListPlugin::getPluginName()
{
    return tr("变量文本列表");
}


/**
 * @brief TagTextListPlugin::getElementIcon
 * @details 获取元素的图标
 * @return 元素图标
 */
QIcon TagTextListPlugin::getElementIcon()
{
    QIcon icon;
    icon.addFile(QStringLiteral(":/images/TagTextList.png"), QSize(), QIcon::Normal, QIcon::Off);
    return icon;
}


/**
 * @brief TagTextListPlugin::getElementName
 * @details 获取元素名称
 * @return 元素名称
 */
QString TagTextListPlugin::getElementName()
{
    return tr("变量文本列表");
}

/**
 * @brief TagTextListPlugin::getElementID
 * @details 获取元素ID
 * @return 元素ID
 */
int TagTextListPlugin::getElementID()
{
    return TagTextListItemType;
}


/**
 * @brief TagTextListPlugin::getElementIDString
 * @details 获取元素ID String
 * @return 元素ID String
 */
QString TagTextListPlugin::getElementIDString()
{
    return "TagTextList";
}

/**
 * @brief createElement
 * @details 创建元素
 * @return 元素对象
 */
Element* TagTextListPlugin::createElement(const QString &szProjPath, const QString &szProjName)
{
    return new ElementTagTextList(szProjPath, szProjName);
}
