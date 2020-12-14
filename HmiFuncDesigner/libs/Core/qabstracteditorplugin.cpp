#include "qabstracteditorplugin.h"

void QAbstractEditorPlugin::init_plugin()
{
}

QString QAbstractEditorPlugin::get_plugin_type()
{
    return RESOURCE_EDIOTR_PLUGIN_TYPE;
}
