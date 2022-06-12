#include "qabstractupdateplugin.h"

void QAbstractUpdatePlugin::initPlugin()
{
}

QString QAbstractUpdatePlugin::getPluginType()
{
    return UPDATE_PLUGIN_TYPE;
}

QAbstractUpdate* QAbstractUpdatePlugin::create(const QString &name)
{
    (void)name;
    return NULL;
}

QStringList QAbstractUpdatePlugin::supports()
{
    QStringList list;
    return list;
}
