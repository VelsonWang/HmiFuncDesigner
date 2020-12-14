#include "qcomboboxhost.h"

#include <QComboBox>

QComboBoxHost::QComboBoxHost(QAbstractHost *parent):
    QWidgetHost(parent)
{
    setProperty("need_frame",false);
    setProperty("accept_drop",false);
}

QString QComboBoxHost::get_show_name()
{
    return tr("Combo Box");
}

QString QComboBoxHost::get_show_group()
{
    return tr("Input Widgets");
}

QString QComboBoxHost::get_show_icon()
{
    return ":/images/combobox.png";
}

void QComboBoxHost::create_object()
{
    m_object=new QComboBox();
    m_object->setObjectName("combobox");
}
