#include "ComboBoxPlugin.h"
#include "ElementComboBox.h"



ComboBoxPlugin::ComboBoxPlugin()
{

}


/**
 * @brief ComboBoxPlugin::initialize
 * @details 插件初始化
 */
void ComboBoxPlugin::initialize()
{

}

/**
 * @brief ComboBoxPlugin::getPluginTypeName
 * @details 获取插件类型名称
 * @return 插件类型名称
 */
QString ComboBoxPlugin::getPluginTypeName()
{
    return tr("基本控件");
}


/**
 * @brief ComboBoxPlugin::getPluginName
 * @details 获取插件名称
 * @return 插件名称
 */
QString ComboBoxPlugin::getPluginName()
{
    return tr("下拉菜单列表");
}


/**
 * @brief ComboBoxPlugin::getElementIcon
 * @details 获取元素的图标
 * @return 元素图标
 */
QIcon ComboBoxPlugin::getElementIcon()
{
    QIcon icon;
    icon.addFile(QStringLiteral(":/images/ComboBox.png"), QSize(), QIcon::Normal, QIcon::Off);
    return icon;
}


/**
 * @brief ComboBoxPlugin::getElementName
 * @details 获取元素名称
 * @return 元素名称
 */
QString ComboBoxPlugin::getElementName()
{
    return tr("下拉菜单列表");
}


/**
 * @brief ComboBoxPlugin::getElementID
 * @details 获取元素ID
 * @return 元素ID
 */
int ComboBoxPlugin::getElementID()
{
    return ComboBoxItemType;
}


/**
 * @brief ComboBoxPlugin::getElementIDString
 * @details 获取元素ID String
 * @return 元素ID String
 */
QString ComboBoxPlugin::getElementIDString()
{
    return "ComboBox";
}

/**
 * @brief createElement
 * @details 创建元素
 * @return 元素对象
 */
Element* ComboBoxPlugin::createElement(ProjectData* pProjDataObj, QtVariantPropertyManager *propertyMgr)
{
    return new ElementComboBox(pProjDataObj, propertyMgr);
}
