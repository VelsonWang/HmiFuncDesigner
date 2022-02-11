#include "tagmanagerplugin.h"
#include "tagmanagerwin.h"
#include <qplugin.h>
#include <QIcon>
#include <QVariant>

void TagManagerPlugin::createWidget()
{
    m_widget = new TagManagerWin;
    m_widget->setProperty("panelwidget", true);
}

QString TagManagerPlugin::getPageName()
{
    return "TagMgr";
}

QString TagManagerPlugin::getPluginName()
{
    return PLUGIN_NAME;
}

QIcon TagManagerPlugin::getPageIcon()
{
    return QIcon();
}

int TagManagerPlugin::getIndex()
{
    return 4;
}

bool TagManagerPlugin::getEnabled()
{
    return false;
}

