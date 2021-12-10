#include "ElementArrow.h"
#include "math.h"
#include <QtDebug>

static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;

ElementArrow::ElementArrow()
    : Element(),
      arrowSize(10) {
    elementId = tr("Arrow");
    internalElementType = tr("Arrow");
    init();
}

ElementArrow::~ElementArrow()
{

}

QRectF ElementArrow::boundingRect() const
{
    qreal extra = 5;
    const qreal x1 = elementLine.p1().x();
    const qreal x2 = elementLine.p2().x();
    const qreal y1 = elementLine.p1().y();
    const qreal y2 = elementLine.p2().y();
    qreal lx = qMin(x1, x2);
    qreal rx = qMax(x1, x2);
    qreal ty = qMin(y1, y2);
    qreal by = qMax(y1, y2);

    return QRectF(lx, ty, rx - lx, by - ty).normalized()
            .adjusted(-extra, -extra, extra, extra);
}


void ElementArrow::setClickPosition(QPointF position)
{
    setElementXPos(position.x());
    setElementYPos(position.y());
    elementLine.setLine(0, 0, elementWidth, elementHeight);
}

void ElementArrow::updateBoundingElement()
{
    elementLine.setLine(0, 0, elementWidth, elementHeight);
}

void ElementArrow::paint(QPainter *painter)
{
    if(!bShow_) {
        return;
    }
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    painter->translate(QPoint(elementXPos, elementYPos));
    painter->rotate(elemAngle);
    painter->setPen(QPen(borderColor_, borderWidth_));
    painter->drawLine(elementLine);

    double angle = ::acos(elementLine.dx()/elementLine.length());

    if (elementLine.dy() >= 0) {
        angle = TwoPi - angle;
    }

    QPointF sourceArrowP1 = elementLine.p1() + QPointF(sin(angle + Pi/3)*arrowSize,
                                                       cos(angle + Pi/3)*arrowSize);
    QPointF sourceArrowP2 = elementLine.p1() + QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
                                                       cos(angle + Pi - Pi / 3) * arrowSize);

    painter->drawPolyline(QPolygonF() << sourceArrowP1 << elementLine.p1() << sourceArrowP2);
    painter->restore();
}

void ElementArrow::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
}

void ElementArrow::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
}

void ElementArrow::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
}

void ElementArrow::readFromXml(const QXmlStreamAttributes &attributes)
{
    if (attributes.hasAttribute("elementId")) {
        setElementId(attributes.value("elementId").toString());
    }

    if (attributes.hasAttribute("x")) {
        setElementXPos(attributes.value("x").toString().toInt());
    }

    if (attributes.hasAttribute("y")) {
        setElementYPos(attributes.value("y").toString().toInt());
    }

    if (attributes.hasAttribute("z")) {
        setElementZValue(attributes.value("z").toString().toInt());
    }

    if (attributes.hasAttribute("width")) {
        setElementWidth(attributes.value("width").toString().toInt());
    }

    if (attributes.hasAttribute("height")) {
        setElementHeight(attributes.value("height").toString().toInt());
    }

    if (attributes.hasAttribute("borderWidth")) {
        borderWidth_ = attributes.value("borderWidth").toInt();
    }

    if (attributes.hasAttribute("borderColor")) {
        borderColor_ = QColor(attributes.value("borderColor").toString());
    }

    if (attributes.hasAttribute("elemAngle")) {
        setAngle(attributes.value("elemAngle").toString().toInt());
    }

    updateBoundingElement();
}

void ElementArrow::readData(QDataStream &in)
{
    QString id;
    qreal xpos;
    qreal ypos;
    qreal zvalue;
    int width;
    int height;
    int borderWidth;
    QColor borderColor;
    qreal angle;

    in >> id
            >> xpos
            >> ypos
            >> zvalue
            >> width
            >> height
            >> borderWidth
            >> borderColor
            >> angle;

    this->setElementId(id);
    this->setElementXPos(xpos);
    this->setElementYPos(ypos);
    this->setElementZValue(zvalue);
    this->setElementWidth(width);
    this->setElementHeight(height);
    this->borderWidth_ = borderWidth;
    this->borderColor_ = borderColor;
    this->setAngle(angle);
    this->updateBoundingElement();
}

QDataStream &operator>>(QDataStream &in,ElementArrow &line)
{
    QString id;
    qreal xpos;
    qreal ypos;
    qreal zvalue;
    int width;
    int height;
    int borderWidth;
    QColor borderColor;
    qreal angle;

    in >> id
            >> xpos
            >> ypos
            >> zvalue
            >> width
            >> height
            >> borderWidth
            >> borderColor
            >> angle;

    line.setElementId(id);
    line.setElementXPos(xpos);
    line.setElementYPos(ypos);
    line.setElementZValue(zvalue);
    line.setElementWidth(width);
    line.setElementHeight(height);
    line.borderWidth_ = borderWidth;
    line.borderColor_ = borderColor;
    line.setAngle(angle);
    line.updateBoundingElement();

    return in;
}
