#include "qabstractspinboxhost.h"

#include "../property/qabstractproperty.h"
#include "../qpropertyfactory.h"
#include "../qcommonstruct.h"

#include <QAbstractSpinBox>

QAbstractSpinBoxHost::QAbstractSpinBoxHost(QAbstractHost *parent):
    QWidgetHost(parent)
{
}

void QAbstractSpinBoxHost::initProperty()
{
    QWidgetHost::initProperty();

    QAbstractProperty *pro;

    pro = QPropertyFactory::create_property("Bool");
    if(pro != NULL) {
        pro->setObjectProperty("name", "frame");
        pro->setAttribute("show_name", tr("Frame"));
        pro->setAttribute("group", "Attributes");
        pro->setAttribute(ATTR_CAN_SAME, true);
        insertProperty(pro);
    }

    pro = QPropertyFactory::create_property("Bool");
    if(pro != NULL) {
        pro->setObjectProperty("name", "wrapping");
        pro->setAttribute("show_name", tr("Wrapping"));
        pro->setAttribute("group", "Attributes");
        pro->setAttribute(ATTR_CAN_SAME, true);
        insertProperty(pro);
    }

    pro = QPropertyFactory::create_property("Bool");
    if(pro != NULL) {
        pro->setObjectProperty("name", "readOnly");
        pro->setAttribute("show_name", tr("ReadOnly"));
        pro->setAttribute("group", "Attributes");
        pro->setAttribute(ATTR_CAN_SAME, true);
        insertProperty(pro);
    }

    pro = QPropertyFactory::create_property("Enum");
    if(pro != NULL) {
        pro->setObjectProperty("name", "buttonSymbols");
        pro->setAttribute("show_name", tr("ButtonSymbols"));
        pro->setAttribute("group", "Attributes");
        pro->setAttribute(ATTR_CAN_SAME, true);
        ComboItems items;
        tagComboItem item;

        item.m_text = tr("UpDownArrows");
        item.m_value = QAbstractSpinBox::UpDownArrows;
        items.append(item);

        item.m_text = tr("NoButtons");
        item.m_value = QAbstractSpinBox::NoButtons;
        items.append(item);

        QVariant v;
        v.setValue<ComboItems>(items);
        pro->setAttribute("items", v);
        insertProperty(pro);
    }

    pro = QPropertyFactory::create_property("Enum");
    if(pro != NULL) {
        pro->setObjectProperty("name", "alignment");
        pro->setAttribute("show_name", tr("Alignment"));
        pro->setAttribute("group", "Attributes");
        pro->setAttribute(ATTR_CAN_SAME, true);
        ComboItems items;
        tagComboItem item;

        item.m_text = tr("AlignLeft");
        item.m_value = Qt::AlignLeft;
        items.append(item);

        item.m_text = tr("AlignRight");
        item.m_value = Qt::AlignRight;
        items.append(item);

        item.m_text = tr("AlignHCenter");
        item.m_value = Qt::AlignHCenter;
        items.append(item);

        QVariant v;
        v.setValue<ComboItems>(items);
        pro->setAttribute("items", v);
        insertProperty(pro);
    }

    setPropertyValue("alignment", Qt::AlignLeft);
}

void QAbstractSpinBoxHost::setFrame(bool frame)
{
    setPropertyValue("frame", frame);
}

bool QAbstractSpinBoxHost::frame()
{
    return getPropertyValue("frame").toBool();
}

void QAbstractSpinBoxHost::setWraping(bool wraping)
{
    setPropertyValue("wraping", wraping);
}

bool QAbstractSpinBoxHost::wraping()
{
    return getPropertyValue("wraping").toBool();
}

void QAbstractSpinBoxHost::setReadOnly(bool readonly)
{
    setPropertyValue("readOnly", readonly);
}

bool QAbstractSpinBoxHost::readonly()
{
    return getPropertyValue("readOnly").toBool();
}

void QAbstractSpinBoxHost::setButtonSymbols(int buttonSymbols)
{
    setPropertyValue("buttonSymbols", buttonSymbols);
}

int QAbstractSpinBoxHost::buttonSymbols()
{
    return getPropertyValue("buttonSymbols").toInt();
}

void QAbstractSpinBoxHost::setAlignment(int alignment)
{
    setPropertyValue("alignment", alignment);
}

int QAbstractSpinBoxHost::alignment()
{
    return getPropertyValue("alignment").toInt();
}
