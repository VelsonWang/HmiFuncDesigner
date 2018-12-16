
#include "ELinePlugin.h"
#include "ELine.h"


ELinePlugin::ELinePlugin()
{

}

/*
* 插件初始化
*/
void ELinePlugin::Initialize()
{

}


/*
* 插件类型
*/
QString ELinePlugin::GetPluginType()
{
    return tr("基本图元");
}


/*
* 插件ID
*/
tGraphType ELinePlugin::GetPluginID()
{
    return GRAPH_LINE;
}


/*
* 插件名称
*/
QString ELinePlugin::GetPluginName()
{
    return tr("直线");
}


/*
* 获取元素的图标
*/
QIcon ELinePlugin::GetElementIcon()
{
    QIcon icon;
    icon.addFile(QStringLiteral(":/images/toolline.png"), QSize(), QIcon::Normal, QIcon::Off);
    return icon;
}

/*
* 获取元素名称
*/
QString ELinePlugin::GetElementName()
{
    return tr("直线");
}

// 创建元素
StdElement* ELinePlugin::CreateElement()
{
    return static_cast<StdElement *>(new ELine());
}


