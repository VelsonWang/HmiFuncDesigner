#include "rtdbplugin.h"
#include "rtdbwin.h"
#include <qplugin.h>
#include <QIcon>
#include <QVariant>

void RTDBPlugin::createWidget()
{
    m_widget = new RTDBWin;
    m_widget->setProperty("panelwidget", true);
}

QString RTDBPlugin::getPageName()
{
    return "RTDBWin";
}

QString RTDBPlugin::getPluginName()
{
    return PLUGIN_NAME;
}

QIcon RTDBPlugin::getPageIcon()
{
    return QIcon();
}

int RTDBPlugin::getIndex()
{
    return 5;
}

bool RTDBPlugin::getEnabled()
{
    return false;
}

