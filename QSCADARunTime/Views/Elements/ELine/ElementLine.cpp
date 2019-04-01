#include "elementline.h"
#include <QtDebug>

ElementLine::ElementLine()
{
    elementId = trUtf8("直线");
    internalElementType = trUtf8("Line");
    init();
}

QRectF ElementLine::boundingRect() const {

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
            .adjusted(-extra,-extra,extra,extra);;
}


void ElementLine::setClickPosition(QPointF position) {

    prepareGeometryChange();
    setElementXPos(position.x());
    setElementYPos(position.y());

    elementLine.setLine(0,0, elementWidth,elementHeight);
}

void ElementLine::updateBoundingElement() {
    elementLine.setLine(0,0,elementWidth,elementHeight);
}

void ElementLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

    Q_UNUSED(option)
    Q_UNUSED(widget)

    QPen itemPen;
    itemPen.setColor(backgroundColor);
    itemPen.setWidth(borderWidth);

    painter->setRenderHints(QPainter::HighQualityAntialiasing | QPainter::TextAntialiasing);
    painter->setPen(itemPen);
    painter->drawLine(elementLine);

}

void ElementLine::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    Q_UNUSED(event)
}

void ElementLine::mousePressEvent(QGraphicsSceneMouseEvent *event) {

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

void ElementLine::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {

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

void ElementLine::readFromXml(const QXmlStreamAttributes &attributes) {

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

    updateBoundingElement();
}


void ElementLine::readData(QDataStream &in) {

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


QDataStream &operator>>(QDataStream &in,ElementLine &line) {

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
