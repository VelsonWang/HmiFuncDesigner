#include "qspinboxhost.h"

#include "../property/qabstractproperty.h"
#include "../qpropertyfactory.h"
#include "../qcommonstruct.h"

#include <QSpinBox>

QSpinBoxHost::QSpinBoxHost(QAbstractHost *parent):
    QAbstractSpinBoxHost(parent)
{
    setProperty("need_frame", false);
    setProperty("accept_drop", false);
}

QString QSpinBoxHost::getShowName()
{
    return tr("Spin Box");
}

QString QSpinBoxHost::getShowGroup()
{
    return tr("Input Widgets");
}


QString QSpinBoxHost::getShowIcon()
{
    return ":/images/spinbox.png";
}

void QSpinBoxHost::createObject()
{
    m_object = new QSpinBox();
    m_object->setObjectName("spinbox");
}

void QSpinBoxHost::initProperty()
{
    QAbstractSpinBoxHost::initProperty();

    QAbstractProperty *pro;

    pro = QPropertyFactory::create_property("Number");
    if(pro != NULL) {
        pro->setObjectProperty("name", "maximum");
        pro->setAttribute("show_name", tr("Maximum"));
        pro->setAttribute("group", "Attributes");
        pro->setAttribute(ATTR_CAN_SAME, true);
        insertProperty(pro, 1);
    }

    pro = QPropertyFactory::create_property("Number");
    if(pro != NULL) {
        pro->setObjectProperty("name", "minimum");
        pro->setAttribute("show_name", tr("Minimum"));
        pro->setAttribute("group", "Attributes");
        pro->setAttribute(ATTR_CAN_SAME, true);
        insertProperty(pro);
    }

    pro = QPropertyFactory::create_property("Number");
    if(pro != NULL) {
        pro->setObjectProperty("name", "value");
        pro->setAttribute("show_name", tr("Value"));
        pro->setAttribute("group", "Attributes");
        pro->setAttribute(ATTR_CAN_SAME, true);
        insertProperty(pro);
    }

    pro = QPropertyFactory::create_property("Number");
    if(pro != NULL) {
        pro->setObjectProperty("name", "singleStep");
        pro->setAttribute("show_name", tr("SingleStep"));
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

    QSpinBox *e = (QSpinBox*)m_object;
    connect(e, SIGNAL(valueChanged(QString)), this, SLOT(valueChanged(QString)));
}

void QSpinBoxHost::setValue(int value)
{
    setPropertyValue("value", value);
}

int QSpinBoxHost::value()
{
    return getPropertyValue("value").toInt();
}

void QSpinBoxHost::setSingleStep(int singleStep)
{
    setPropertyValue("singleStep", singleStep);
}

int QSpinBoxHost::singleStep()
{
    return getPropertyValue("singleStep").toInt();
}

void QSpinBoxHost::valueChanged(const QString &value)
{
    QString code = getPropertyValue("valueChanged").toString();
    if(code != "") {
        QMap<QString, QString> param;
        param.insert("_value", value);
        exec(code, param);
    }
}
