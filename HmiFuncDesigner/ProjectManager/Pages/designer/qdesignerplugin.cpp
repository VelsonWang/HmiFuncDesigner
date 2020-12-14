#include "qdesignerplugin.h"

#include "qdesignerwidget.h"

#include <qplugin.h>
#include <QIcon>
#include <QVariant>

void QDesignerPlugin::create_widget()
{
    m_widget=new QDesignerWidget;
    m_widget->setProperty("panelwidget",true);
    ((QDesignerWidget*)m_widget)->set_undo_stack(m_undo_stack);
}

QString QDesignerPlugin::get_page_name()
{
    return tr("Designer");
}

QString QDesignerPlugin::get_plugin_name()
{
    return DESIGNER_PLUGIN_NAME;
}

QIcon QDesignerPlugin::get_page_icon()
{
    return QIcon(":/images/design.png");
}

int QDesignerPlugin::get_index()
{
    return 2;
}

bool QDesignerPlugin::get_enabled()
{
    return false;
}

