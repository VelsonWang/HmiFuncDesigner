#include "scriptplugin.h"
#include "scriptwin.h"
#include <qplugin.h>
#include <QIcon>
#include <QVariant>

void ScriptPlugin::createWidget()
{
    m_widget = new ScriptWin;
    m_widget->setProperty("panelwidget", true);
}

QString ScriptPlugin::getPageName()
{
    return "ScriptEditor";
}

QString ScriptPlugin::getPluginName()
{
    return PLUGIN_NAME;
}

QIcon ScriptPlugin::getPageIcon()
{
    return QIcon();
}

int ScriptPlugin::getIndex()
{
    return 6;
}

bool ScriptPlugin::getEnabled()
{
    return false;
}

