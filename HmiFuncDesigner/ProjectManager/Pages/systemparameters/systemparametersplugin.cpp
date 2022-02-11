#include "systemparametersplugin.h"
#include "systemparameters.h"
#include <qplugin.h>
#include <QIcon>
#include <QVariant>

void SystemParametersPlugin::createWidget()
{
    m_widget = new SystemParameters;
    m_widget->setProperty("panelwidget", true);
}

QString SystemParametersPlugin::getPageName()
{
    return "SystemParameters";
}

QString SystemParametersPlugin::getPluginName()
{
    return PLUGIN_NAME;
}

QIcon SystemParametersPlugin::getPageIcon()
{
    return QIcon();
}

int SystemParametersPlugin::getIndex()
{
    return 1;
}

bool SystemParametersPlugin::getEnabled()
{
    return false;
}

