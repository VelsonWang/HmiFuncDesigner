#include "qabstracteditorplugin.h"

void QAbstractEditorPlugin::initPlugin()
{
}

QString QAbstractEditorPlugin::getPluginType()
{
    return RESOURCE_EDIOTR_PLUGIN_TYPE;
}
