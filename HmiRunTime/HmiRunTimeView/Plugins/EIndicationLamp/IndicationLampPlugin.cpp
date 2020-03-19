
#include "IndicationLampPlugin.h"
#include "ElementIndicationLamp.h"
#include <QDebug>
#include "ProjectData.h"
IndicationLampPlugin::IndicationLampPlugin()
{

}


/**
 * @brief IndicationLampPlugin::initialize
 * @details 插件初始化
 */
void IndicationLampPlugin::initialize()
{

}

/**
 * @brief IndicationLampPlugin::getPluginTypeName
 * @details 获取插件类型名称
 * @return 插件类型名称
 */
QString IndicationLampPlugin::getPluginTypeName()
{
    return tr("基本控件");
}


/**
 * @brief IndicationLampPlugin::getPluginName
 * @details 获取插件名称
 * @return 插件名称
 */
QString IndicationLampPlugin::getPluginName()
{
    return tr("指示灯");
}


/**
 * @brief IndicationLampPlugin::getElementIcon
 * @details 获取元素的图标
 * @return 元素图标
 */
QIcon IndicationLampPlugin::getElementIcon()
{
    QIcon icon;
    icon.addFile(QStringLiteral(":/images/IndicationLamp.png"), QSize(), QIcon::Normal, QIcon::Off);
    return icon;
}


/**
 * @brief IndicationLampPlugin::getElementName
 * @details 获取元素名称
 * @return 元素名称
 */
QString IndicationLampPlugin::getElementName()
{
    return tr("指示灯");
}

/**
 * @brief IndicationLampPlugin::getElementID
 * @details 获取元素ID
 * @return 元素ID
 */
int IndicationLampPlugin::getElementID()
{
    return IndicationLampItemType;
}


/**
 * @brief IndicationLampPlugin::getElementIDString
 * @details 获取元素ID String
 * @return 元素ID String
 */
QString IndicationLampPlugin::getElementIDString()
{
    return "IndicationLamp";
}

/**
 * @brief createElement
 * @details 创建元素
 * @return 元素对象
 */
Element* IndicationLampPlugin::createElement(const QString &szProjPath,
                                             const QString &szProjName,
                                             QtVariantPropertyManager *propertyMgr)
{
    return new ElementIndicationLamp(szProjPath, szProjName, propertyMgr);
}
