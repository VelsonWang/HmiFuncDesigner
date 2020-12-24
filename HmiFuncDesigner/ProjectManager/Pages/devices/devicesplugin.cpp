#include "devicesplugin.h"
#include "communicationdevice.h"
#include <qplugin.h>
#include <QIcon>
#include <QVariant>

void DevicesPlugin::createWidget()
{
    m_widget = new CommunicationDevice;
    m_widget->setProperty("panelwidget", true);
}

QString DevicesPlugin::getPageName()
{
    return "CommunicationDevice";
}

QString DevicesPlugin::getPluginName()
{
    return PLUGIN_NAME;
}

QIcon DevicesPlugin::getPageIcon()
{
    return QIcon();
}

int DevicesPlugin::getIndex()
{
    return 3;
}

bool DevicesPlugin::getEnabled()
{
    return false;
}

