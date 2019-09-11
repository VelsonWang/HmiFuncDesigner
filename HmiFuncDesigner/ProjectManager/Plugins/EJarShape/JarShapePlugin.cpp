
#include "JarShapePlugin.h"
#include "ElementJarShape.h"


JarShapePlugin::JarShapePlugin()
{

}


/**
 * @brief JarShapePlugin::initialize
 * @details 插件初始化
 */
void JarShapePlugin::initialize()
{

}

/**
 * @brief JarShapePlugin::getPluginTypeName
 * @details 获取插件类型名称
 * @return 插件类型名称
 */
QString JarShapePlugin::getPluginTypeName()
{
    return tr("基本控件");
}


/**
 * @brief JarShapePlugin::getPluginName
 * @details 获取插件名称
 * @return 插件名称
 */
QString JarShapePlugin::getPluginName()
{
    return tr("罐形容器");
}


/**
 * @brief JarShapePlugin::getElementIcon
 * @details 获取元素的图标
 * @return 元素图标
 */
QIcon JarShapePlugin::getElementIcon()
{
    QIcon icon;
    icon.addFile(QStringLiteral(":/images/JarShape.png"), QSize(), QIcon::Normal, QIcon::Off);
    return icon;
}


/**
 * @brief JarShapePlugin::getElementName
 * @details 获取元素名称
 * @return 元素名称
 */
QString JarShapePlugin::getElementName()
{
    return tr("罐形容器");
}

/**
 * @brief JarShapePlugin::getElementID
 * @details 获取元素ID
 * @return 元素ID
 */
int JarShapePlugin::getElementID()
{
    return JarShapeItemType;
}


/**
 * @brief JarShapePlugin::getElementIDString
 * @details 获取元素ID String
 * @return 元素ID String
 */
QString JarShapePlugin::getElementIDString()
{
    return "JarShape";
}

/**
 * @brief createElement
 * @details 创建元素
 * @return 元素对象
 */
Element* JarShapePlugin::createElement(const QString &szProjPath,
                                       const QString &szProjName,
                                       QtVariantPropertyManager *propertyMgr)
{
    return new ElementJarShape(szProjPath, szProjName, propertyMgr);
}
