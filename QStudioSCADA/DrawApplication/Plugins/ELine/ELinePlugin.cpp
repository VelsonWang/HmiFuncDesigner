
#include "ELinePlugin.h"
#include "ElementLine.h"


ELinePlugin::ELinePlugin()
{

}


/**
 * @brief ELinePlugin::initialize
 * @details 插件初始化
 */
void ELinePlugin::initialize()
{

}

/**
 * @brief ELinePlugin::getPluginTypeName
 * @details 获取插件类型名称
 * @return 插件类型名称
 */
QString ELinePlugin::getPluginTypeName()
{
    return tr("基本图元");
}


/**
 * @brief ELinePlugin::getPluginName
 * @details 获取插件名称
 * @return 插件名称
 */
QString ELinePlugin::getPluginName()
{
    return tr("直线");
}


/**
 * @brief ELinePlugin::getElementIcon
 * @details 获取元素的图标
 * @return 元素图标
 */
QIcon ELinePlugin::getElementIcon()
{
    QIcon icon;
    icon.addFile(QStringLiteral(":/images/lineitem.png"), QSize(), QIcon::Normal, QIcon::Off);
    return icon;
}


/**
 * @brief ELinePlugin::getElementName
 * @details 获取元素名称
 * @return 元素名称
 */
QString ELinePlugin::getElementName()
{
    return tr("直线");
}

/**
 * @brief ELinePlugin::getElementID
 * @details 获取元素ID
 * @return 元素ID
 */
int ELinePlugin::getElementID()
{
    return LineItemType;
}


/**
 * @brief ELinePlugin::getElementIDString
 * @details 获取元素ID String
 * @return 元素ID String
 */
QString ELinePlugin::getElementIDString()
{
    return "Line";
}

/**
 * @brief createElement
 * @details 创建元素
 * @return 元素对象
 */
Element* ELinePlugin::createElement(const QString &szProjPath,
                                    const QString &szProjName,
                                    QtVariantPropertyManager *propertyMgr)
{
    return new ElementLine(szProjPath, szProjName, propertyMgr);
}
