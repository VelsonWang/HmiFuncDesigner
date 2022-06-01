#include "qcomboboxhost.h"

#include <QComboBox>

QComboBoxHost::QComboBoxHost(QAbstractHost *parent):
    QWidgetHost(parent)
{
    setProperty("need_frame", false);
    setProperty("accept_drop", false);
}

QString QComboBoxHost::getShowName()
{
    return tr("组合框");//tr("Combo Box");
}

QString QComboBoxHost::getShowGroup()
{
    return tr("输入部件"); //tr("Input Widgets");
}

QString QComboBoxHost::getShowIcon()
{
    return ":/images/combobox.png";
}

void QComboBoxHost::createObject()
{
    m_object = new QComboBox();
    m_object->setObjectName("combobox");
}
