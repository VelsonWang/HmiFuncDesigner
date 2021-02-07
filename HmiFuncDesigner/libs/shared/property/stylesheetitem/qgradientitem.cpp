#include "qgradientitem.h"

#include "stylesheetstruct.h"

#include "../../xmlobject.h"

#include <QLinearGradient>
#include <QRadialGradient>
#include <QConicalGradient>

QGradientItem::QGradientItem(QAbstractStylesheetItem *parent) :
    QAbstractStylesheetItem(parent)
{
}

void QGradientItem::read(XMLObject *xml)
{
    while(m_children.size() > 0) {
        delete m_children.first();
    }
    m_attributes = xml->getPropertys();
    QAbstractStylesheetItem *item;
    QList<XMLObject*> list = xml->getChildren();
    foreach(XMLObject* c, list) {
        item = new QAbstractStylesheetItem(this);
        item->setProperty("title", c->getProperty("title"));
        item->read(c);
    }
}

void QGradientItem::subValueChanged()
{
    QGradient g;
    QString type = attribute("type");
    if(type == "qlineargradient") {
        qreal x1 = attribute("x1", "0").toDouble();
        qreal y1 = attribute("y1", "0").toDouble();
        qreal x2 = attribute("x2", "0").toDouble();
        qreal y2 = attribute("y2", "1").toDouble();
        g = QLinearGradient(x1, y1, x2, y2);
    } else if(type == "qradialgradient") {
        qreal cx = attribute("cx", "0.5").toDouble();
        qreal cy = attribute("cy", "0.5").toDouble();
        qreal radius = attribute("radius", "0.5").toDouble();
        qreal fx = attribute("fx", "0.5").toDouble();
        qreal fy = attribute("fy", "0.5").toDouble();
        g = QRadialGradient(cx, cy, radius, fx, fy);
    } else if(type == "qconicalgradient") {
        qreal cx = attribute("cx", "0.5").toDouble();
        qreal cy = attribute("cy", "0.5").toDouble();
        qreal angle = attribute("angle", "0").toDouble();
        g = QConicalGradient(cx, cy, angle);
    }

    QColor color;
    foreach(QAbstractStylesheetItem* item, m_children) {
        int c = item->value().toInt();
        color = QColor((c>>16)&0xff, (c>>8)&0xff, c&0xff, (c>>24)&0xff);
        g.setColorAt(item->attribute("stop", "0").toDouble(), color);
    }
    if(g != m_value.value<QGradient>()) {
        m_value.setValue<QGradient>(g);
        emit valueChanged();
    }
}

void QGradientItem::setValue(const QVariant &value)
{
    if(m_value != value) {
        m_value = value;
        QGradient g = value.value<QGradient>();
        QGradient::Type t = g.type();
        while(m_children.size() >0 ) {
            delete m_children.first();
        }
        if(t == QGradient::LinearGradient) {
            const QLinearGradient *lg = static_cast<const QLinearGradient*>(&g);
            setAttribute("x1", QString::number(lg->start().x()));
            setAttribute("y1", QString::number(lg->start().y()));
            setAttribute("x2", QString::number(lg->finalStop().x()));
            setAttribute("y2", QString::number(lg->finalStop().y()));
            setAttribute("type", "qlineargradient");
        } else if(t == QGradient::RadialGradient) {
            const QRadialGradient *rg = static_cast<const QRadialGradient*>(&g);
            setAttribute("cx", QString::number(rg->center().x()));
            setAttribute("cy", QString::number(rg->center().y()));
            setAttribute("radius", QString::number(rg->radius()));
            setAttribute("fx", QString::number(rg->focalPoint().x()));
            setAttribute("fy", QString::number(rg->focalPoint().y()));
            setAttribute("type", "qradialgradient");
        } else if(t == QGradient::ConicalGradient) {
            const QConicalGradient *cg = static_cast<const QConicalGradient*>(&g);
            setAttribute("cx", QString::number(cg->center().x()));
            setAttribute("cy", QString::number(cg->center().y()));
            setAttribute("angle", QString::number(cg->angle()));
            setAttribute("type", "qconicalgradient");
        } else {
            return;
        }

        QVector<QPair<qreal, QColor> >stops = g.stops();

        for(int i=0; i<stops.size(); i++) {
            QPair<qreal,QColor> stop = stops.at(i);
            QAbstractStylesheetItem *item = new QAbstractStylesheetItem(this);
            item->setProperty("title", QString("color_%1").arg(i+1));
            disconnect(item, SIGNAL(valueChanged()), this, SLOT(subValueChanged()));
            item->setDefault(attribute("default"));
            uint c=((stop.second.alpha()&0xff)<<24) + ((stop.second.red()&0xff)<<16)
                    + ((stop.second.green()&0xff)<<8) + (stop.second.blue()&0xff);
            item->setValue(c);
            item->setAttribute("stop", QString::number(stop.first));
            connect(item, SIGNAL(valueChanged()), this, SLOT(subValueChanged()));
        }
    }
}

void QGradientItem::setDefault(const QVariant &value)
{
    setAttribute("default", value.toString());
}
