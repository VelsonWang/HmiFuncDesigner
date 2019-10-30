
#include "ValueStickPlugin.h"
#include "ElementValueStick.h"


ValueStickPlugin::ValueStickPlugin()
{

}


/**
 * @brief ValueStickPlugin::initialize
 * @details 插件初始化
 */
void ValueStickPlugin::initialize()
{

}

/**
 * @brief ValueStickPlugin::getPluginTypeName
 * @details 获取插件类型名称
 * @return 插件类型名称
 */
QString ValueStickPlugin::getPluginTypeName()
{
    return tr("基本控件");
}


/**
 * @brief ValueStickPlugin::getPluginName
 * @details 获取插件名称
 * @return 插件名称
 */
QString ValueStickPlugin::getPluginName()
{
    return tr("数值棒图");
}


/**
 * @brief ValueStickPlugin::getElementIcon
 * @details 获取元素的图标
 * @return 元素图标
 */
QIcon ValueStickPlugin::getElementIcon()
{
    QIcon icon;
    icon.addFile(QStringLiteral(":/images/ValueStick.png"), QSize(), QIcon::Normal, QIcon::Off);
    return icon;
}


/**
 * @brief ValueStickPlugin::getElementName
 * @details 获取元素名称
 * @return 元素名称
 */
QString ValueStickPlugin::getElementName()
{
    return tr("数值棒图");
}

/**
 * @brief ValueStickPlugin::getElementID
 * @details 获取元素ID
 * @return 元素ID
 */
int ValueStickPlugin::getElementID()
{
    return ValueStickItemType;
}


/**
 * @brief ValueStickPlugin::getElementIDString
 * @details 获取元素ID String
 * @return 元素ID String
 */
QString ValueStickPlugin::getElementIDString()
{
    return "ValueStick";
}

/**
 * @brief createElement
 * @details 创建元素
 * @return 元素对象
 */
Element* ValueStickPlugin::createElement(const QString &szProjPath,
                                         const QString &szProjName,
                                         QtVariantPropertyManager *propertyMgr)
{
    return new ElementValueStick(szProjPath, szProjName, propertyMgr);
}
