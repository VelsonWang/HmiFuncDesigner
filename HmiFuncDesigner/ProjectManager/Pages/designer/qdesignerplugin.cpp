#include "qdesignerplugin.h"

#include "qdesignerwidget.h"

#include <qplugin.h>
#include <QIcon>
#include <QVariant>

void QDesignerPlugin::createWidget()
{
    m_widget = new QDesignerWidget;
    m_widget->setProperty("panelwidget", true);
    (dynamic_cast<QDesignerWidget*>(m_widget))->setUndoStack(m_undo_stack);
}

QString QDesignerPlugin::getPageName()
{
    return "Designer";
}

QString QDesignerPlugin::getPluginName()
{
    return DESIGNER_PLUGIN_NAME;
}

QIcon QDesignerPlugin::getPageIcon()
{
    return QIcon(":/images/design.png");
}

int QDesignerPlugin::getIndex()
{
    return 2;
}

bool QDesignerPlugin::getEnabled()
{
    return false;
}

