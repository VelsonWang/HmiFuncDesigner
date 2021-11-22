#include "qdoublespinboxhost.h"

#include "../property/qabstractproperty.h"
#include "../qpropertyfactory.h"
#include "../qcommonstruct.h"

#include <QDoubleSpinBox>

QDoubleSpinBoxHost::QDoubleSpinBoxHost(QAbstractHost *parent):
    QAbstractSpinBoxHost(parent)
{
    setProperty("need_frame", false);
    setProperty("accept_drop", false);
}

QString QDoubleSpinBoxHost::getShowName()
{
    return tr("Double Spin Box");
}

QString QDoubleSpinBoxHost::getShowGroup()
{
    return tr("Input Widgets");
}

QString QDoubleSpinBoxHost::getShowIcon()
{
    return ":/images/doublespinbox.png";
}

void QDoubleSpinBoxHost::createObject()
{
    m_object = new QDoubleSpinBox();
    m_object->setObjectName("doublespinbox");
}

void QDoubleSpinBoxHost::initProperty()
{
    QAbstractSpinBoxHost::initProperty();

    QAbstractProperty *pro;

    pro = QPropertyFactory::create_property("Float");
    if(pro != NULL) {
        pro->setObjectProperty("name", "maximum");
        pro->setAttribute("show_name", tr("Maximum"));
        pro->setAttribute("group", "Attributes");
        pro->setAttribute(ATTR_CAN_SAME, true);
        insertProperty(pro, 1);
    }

    pro = QPropertyFactory::create_property("Float");
    if(pro != NULL) {
        pro->setObjectProperty("name", "minimum");
        pro->setAttribute("show_name", tr("Minimum"));
        pro->setAttribute("group", "Attributes");
        pro->setAttribute(ATTR_CAN_SAME, true);
        insertProperty(pro);
    }

    pro = QPropertyFactory::create_property("Float");
    if(pro != NULL) {
        pro->setObjectProperty("name", "value");
        pro->setAttribute("show_name", tr("Value"));
        pro->setAttribute("group", "Attributes");
        pro->setAttribute(ATTR_CAN_SAME, true);
        insertProperty(pro);
    }

    pro = QPropertyFactory::create_property("Float");
    if(pro != NULL) {
        pro->setObjectProperty("name", "singleStep");
        pro->setAttribute("show_name", tr("SingleStep"));
        pro->setAttribute("group", "Attributes");
        pro->setAttribute(ATTR_CAN_SAME, true);
        insertProperty(pro);
    }

    pro = QPropertyFactory::create_property("Number");
    if(pro != NULL) {
        pro->setObjectProperty("name", "decimals");
        pro->setAttribute("show_name", tr("Decimals"));
        pro->setAttribute("group", "Attributes");
        pro->setAttribute(ATTR_CAN_SAME, true);
        insertProperty(pro);
    }

    pro = QPropertyFactory::create_property("Script");
    if(pro != NULL) {
        pro->setObjectProperty("name", "valueChanged");
        pro->setAttribute("show_name", tr("ValueChanged"));
        pro->setAttribute("group", "Events");
        m_object->setProperty("valueChanged", "");
        insertProperty(pro);
    }

    setPropertyValue("geometry", QRect(0, 0, 100, 20));

    QDoubleSpinBox *s = (QDoubleSpinBox*)m_object;
    connect(s, SIGNAL(valueChanged(QString)), this, SLOT(valueChanged(QString)));
}

void QDoubleSpinBoxHost::setValue(float value)
{
    setPropertyValue("value", value);
}

float QDoubleSpinBoxHost::value()
{
    return getPropertyValue("value").toFloat();
}

void QDoubleSpinBoxHost::setSingleStep(float singleStep)
{
    setPropertyValue("singleStep", singleStep);
}

float QDoubleSpinBoxHost::singleStep()
{
    return getPropertyValue("singleStep").toFloat();
}

void QDoubleSpinBoxHost::setDecimals(int decimals)
{
    setPropertyValue("decimals", decimals);
}

int QDoubleSpinBoxHost::decimals()
{
    return getPropertyValue("decimals").toInt();
}

void QDoubleSpinBoxHost::valueChanged(const QString &value)
{
    QString code = getPropertyValue("valueChanged").toString();
    if(code != "") {
        QMap<QString, QString> param;
        param.insert("_value", value);
        exec(code, param);
    }
}

/**
 * @brief QDoubleSpinBoxHost::supportFuncEvents
 * @details 控件支持的功能事件
 * @return
 */
QStringList QDoubleSpinBoxHost::supportFuncEvents()
{
    return QStringList();
}
