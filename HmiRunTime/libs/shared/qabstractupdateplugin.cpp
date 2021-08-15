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
    return Q_NULLPTR;
}

QStringList QAbstractUpdatePlugin::supports()
{
    QStringList list;
    return list;
}
