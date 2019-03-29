#include "elementarrow.h"
#include "math.h"
#include "ObjectCreator.h"
#include <QtDebug>

static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;

ElementArrow::ElementArrow()
    : arrowSize(10)
{
    elementId = trUtf8("箭头");
    internalElementType = trUtf8("Arrow");
    elementIcon = QIcon(":/images/arrowitem.png");

    init();
}

QRectF ElementArrow::boundingRect() const {

    qreal extra = 5;

    const qreal x1 = elementLine.p1().x();
    const qreal x2 = elementLine.p2().x();
    const qreal y1 = elementLine.p1().y();
    const qreal y2 = elementLine.p2().y();
    qreal lx = qMin(x1,x2);
    qreal rx = qMax(x1,x2);
    qreal ty = qMin(y1,y2);
    qreal by = qMax(y1,y2);

    return QRectF(lx,ty,rx - lx, by - ty).normalized()
            .adjusted(-extra,-extra,extra,extra);
}

void ElementArrow::updateElementProperty(uint id, const QVariant &value) {

    switch (id) {

       case EL_ID:
           elementId = value.toString();
           break;
       case EL_X:
           elementXPos = value.toInt();
           setElementXPos(elementXPos);
           break;
       case EL_Y:
           elementYPos = value.toInt();
           setElementYPos(elementYPos);
           break;
       case EL_Z_VALUE:
           elementZValue = value.toInt();
           setZValue(elementZValue);
           break;
       case EL_WIDTH:
           elementWidth = value.toInt();
           updateBoundingElement();
           break;
       case EL_HEIGHT:
           elementHeight = value.toInt();
           updateBoundingElement();
           break;
       case EL_BACKGROUND:
           backgroundColor = value.value<QColor>();
           break;
       case EL_BORDER_WIDTH:
           borderWidth = value.toInt();
           break;
       case EL_ANGLE:
           elemAngle = value.toInt();
           setAngle(elemAngle);
           break;
       }


    update();
    scene()->update();
}


void ElementArrow::setClickPosition(QPointF position) {

    prepareGeometryChange();
    setElementXPos(position.x());
    setElementYPos(position.y());

    elementLine.setLine(0,0, elementWidth,elementHeight);
}

void ElementArrow::updateBoundingElement() {
    elementLine.setLine(0,0,elementWidth,elementHeight);
}

void ElementArrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

    Q_UNUSED(option)
    Q_UNUSED(widget)

    QPen itemPen;
    itemPen.setColor(backgroundColor);
    itemPen.setWidth(borderWidth);

    painter->setRenderHints(QPainter::HighQualityAntialiasing | QPainter::TextAntialiasing);
    painter->setPen(itemPen);
    painter->drawLine(elementLine);
    painter->setBrush(backgroundColor);

    double angle = ::acos(elementLine.dx()/elementLine.length());

    if (elementLine.dy() >= 0) {
        angle = TwoPi - angle;
    }

    QPointF sourceArrowP1 = elementLine.p1() + QPointF(sin(angle + Pi/3)*arrowSize,
                                                       cos(angle + Pi/3)*arrowSize);
    QPointF sourceArrowP2 = elementLine.p1() + QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
                                                  cos(angle + Pi - Pi / 3) * arrowSize);

    painter->drawPolyline(QPolygonF() << sourceArrowP1 << elementLine.p1() << sourceArrowP2);

    if (isSelected()) {
        setCursor(Qt::SizeAllCursor);
        painter->setBrush(Qt::red);
        painter->setPen(Qt::red);
        painter->drawRect(QRectF(elementLine.p1() - QPointF(3,3),elementLine.p1() + QPointF(3,3)));
        painter->drawRect(QRectF(elementLine.p2() - QPointF(3,3),elementLine.p2() + QPointF(3,3)));
    }
}

void ElementArrow::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {

}

void ElementArrow::mousePressEvent(QGraphicsSceneMouseEvent *event) {

    QPointF mousePoint = event->pos();
    QPointF mouseHandler = QPointF(3,3);
    QPointF pp1 = elementLine.p1();
    QPointF pp2 = elementLine.p2();

    if (mousePoint.x() <= (pp1.x() + mouseHandler.x()) &&
        mousePoint.x() >= (pp1.x() - mouseHandler.x()) &&
        mousePoint.y() <= (pp1.y() + mouseHandler.y()) &&
        mousePoint.y() >= (pp1.y() - mouseHandler.y()))
    {
        rd = RdTopLeft;
        resizing = true;
        setCursor(Qt::SizeFDiagCursor);
    }
    else if (mousePoint.x() <= (pp2.x() + mouseHandler.x()) &&
             mousePoint.x() >= (pp2.x() - mouseHandler.x()) &&
             mousePoint.y() <= (pp2.y() + mouseHandler.y()) &&
             mousePoint.y() >= (pp2.y() - mouseHandler.y()))
    {
        rd = RdBottomRight;
        resizing = true;
        setCursor(Qt::SizeFDiagCursor);
    }
    else {
        resizing = false;
        rd = RdNone;
    }

    oldPos = pos();
    oldWidth = elementWidth;
    oldHeight = elementHeight;

    QGraphicsObject::mousePressEvent(event);
}

void ElementArrow::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {

    setCursor(Qt::ArrowCursor);
    elementXPos = pos().x();
    elementYPos = pos().y();

    if (oldPos != pos()) {
        emit elementMoved(oldPos);
    }

    if (resizing) {
        emit elementResized(oldWidth,oldHeight,oldPos);
    }

    QGraphicsObject::mouseReleaseEvent(event);
}

void ElementArrow::readFromXml(const QXmlStreamAttributes &attributes) {

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
        setZValue(attributes.value("z").toString().toInt());
    }

    if (attributes.hasAttribute("width")) {
        setElementWidth(attributes.value("width").toString().toInt());
    }

    if (attributes.hasAttribute("height")) {
        setElementHeight(attributes.value("height").toString().toInt());
    }

    if (attributes.hasAttribute("background")) {
        backgroundColor = QColor(attributes.value("background").toString());
    }

    if (attributes.hasAttribute("borderWidth")) {
        borderWidth = attributes.value("borderWidth").toString().toInt();
    }

    if (attributes.hasAttribute("elemAngle")) {
        setAngle(attributes.value("elemAngle").toString().toInt());
    }

    if (attributes.hasAttribute("block")) {
        setBlocked(attributes.value("block").toString().toInt());
    }

    updateBoundingElement();
}


void ElementArrow::readData(QDataStream &in) {

    QString id;
    qreal xpos;
    qreal ypos;
    qreal zvalue;
    int width;
    int height;
    QColor backColor;
    int borderWidth;
    qreal angle;

    in >> id
       >> xpos
       >> ypos
       >> zvalue
       >> width
       >> height
       >> backColor
       >> borderWidth
       >> angle;

    this->setElementId(id);
    this->setElementXPos(xpos);
    this->setElementYPos(ypos);
    this->setElementZValue(zvalue);
    this->setElementWidth(width);
    this->setElementHeight(height);
    this->backgroundColor = backColor;
    this->borderWidth = borderWidth;
    this->setAngle(angle);
    this->updateBoundingElement();
}

QDataStream &operator>>(QDataStream &in,ElementArrow &line) {

    QString id;
    qreal xpos;
    qreal ypos;
    qreal zvalue;
    int width;
    int height;
    QColor backColor;
    int borderWidth;
    qreal angle;

    in >> id
       >> xpos
       >> ypos
       >> zvalue
       >> width
       >> height
       >> backColor
       >> borderWidth
       >> angle;

    line.setElementId(id);
    line.setElementXPos(xpos);
    line.setElementYPos(ypos);
    line.setElementZValue(zvalue);
    line.setElementWidth(width);
    line.setElementHeight(height);
    line.backgroundColor = backColor;
    line.borderWidth = borderWidth;
    line.setAngle(angle);
    line.updateBoundingElement();

    return in;
}
