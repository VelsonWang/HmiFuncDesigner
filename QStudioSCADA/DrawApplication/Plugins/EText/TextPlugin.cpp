
#include "TextPlugin.h"
#include "ElementText.h"


TextPlugin::TextPlugin()
{

}


/**
 * @brief TextPlugin::initialize
 * @details 插件初始化
 */
void TextPlugin::initialize()
{

}

/**
 * @brief TextPlugin::getPluginTypeName
 * @details 获取插件类型名称
 * @return 插件类型名称
 */
QString TextPlugin::getPluginTypeName()
{
    return tr("基本图元");
}


/**
 * @brief TextPlugin::getPluginName
 * @details 获取插件名称
 * @return 插件名称
 */
QString TextPlugin::getPluginName()
{
    return tr("文本");
}


/**
 * @brief TextPlugin::getElementIcon
 * @details 获取元素的图标
 * @return 元素图标
 */
QIcon TextPlugin::getElementIcon()
{
    QIcon icon;
    icon.addFile(QStringLiteral(":/images/textitem.png"), QSize(), QIcon::Normal, QIcon::Off);
    return icon;
}


/**
 * @brief TextPlugin::getElementName
 * @details 获取元素名称
 * @return 元素名称
 */
QString TextPlugin::getElementName()
{
    return tr("文本");
}

/**
 * @brief TextPlugin::getElementID
 * @details 获取元素ID
 * @return 元素ID
 */
int TextPlugin::getElementID()
{
    return TextItemType;
}


/**
 * @brief TextPlugin::getElementIDString
 * @details 获取元素ID String
 * @return 元素ID String
 */
QString TextPlugin::getElementIDString()
{
    return "Text";
}

/**
 * @brief createElement
 * @details 创建元素
 * @return 元素对象
 */
Element* TextPlugin::createElement(const QString &szProjPath, const QString &szProjName)
{
    return new ElementText(szProjPath, szProjName);
}
