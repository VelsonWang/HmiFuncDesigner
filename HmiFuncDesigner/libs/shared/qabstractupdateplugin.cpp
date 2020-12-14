#include "qabstractupdateplugin.h"

void QAbstractUpdatePlugin::init_plugin()
{
}

QString QAbstractUpdatePlugin::get_plugin_type()
{
    return UPDATE_PLUGIN_TYPE;
}

QAbstractUpdate* QAbstractUpdatePlugin::create(const QString &name)
{
    return NULL;
}

QStringList QAbstractUpdatePlugin::supports()
{
    QStringList list;
    return list;
}
