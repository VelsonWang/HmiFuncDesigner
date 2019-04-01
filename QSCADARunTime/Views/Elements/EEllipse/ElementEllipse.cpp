#include "ElementEllipse.h"
#include <QDebug>

ElementEllipse::ElementEllipse()
{
    elementId = trUtf8("椭圆形");
    internalElementType = trUtf8("Ellipse");
    init();
}

QRectF ElementEllipse::boundingRect() const {

    qreal extra = 5;

    QRectF rect = elementRect.toRect();
    return rect.normalized().adjusted(-extra,-extra,extra,extra);
}

QPainterPath ElementEllipse::shape() const {
    QPainterPath path;
    path.addEllipse(elementRect);
    return path;
}

void ElementEllipse::setClickPosition(QPointF position) {

    prepareGeometryChange();
    elementXPos = position.x();
    elementYPos = position.y();
    setX(elementXPos);
    setY(elementYPos);

    elementRect.setRect(0,0,elementWidth,elementHeight);
}

void ElementEllipse::updateBoundingElement() {
    elementRect.setRect(0,0,elementWidth,elementHeight);
}

void ElementEllipse::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->setRenderHints(QPainter::HighQualityAntialiasing | QPainter::TextAntialiasing);

    painter->setPen(QPen(borderColor,borderWidth));
    painter->setBrush(QBrush(backgroundColor));
    painter->drawPath(shape());
}

void ElementEllipse::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    Q_UNUSED(event)
}

void ElementEllipse::mousePressEvent(QGraphicsSceneMouseEvent *event) {

    QPointF mousePoint = event->pos();
    QPointF mouseHandler = QPointF(3,3);
    QPointF topLeft = elementRect.topLeft();
    QPointF bottomRight = elementRect.bottomRight();

    if (mousePoint.x() <= (topLeft.x() + mouseHandler.x()) &&
        mousePoint.x() >= (topLeft.x() - mouseHandler.x()) &&
        mousePoint.y() <= (topLeft.y() + mouseHandler.y()) &&
        mousePoint.y() >= (topLeft.y() - mouseHandler.y()))
    {
        rd = RdTopLeft;
        resizing = true;
        setCursor(Qt::SizeFDiagCursor);
    }
    else if (mousePoint.x() <= (bottomRight.x() + mouseHandler.x()) &&
             mousePoint.x() >= (bottomRight.x() - mouseHandler.x()) &&
             mousePoint.y() <= (bottomRight.y() + mouseHandler.y()) &&
             mousePoint.y() >= (bottomRight.y() - mouseHandler.y()))
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

void ElementEllipse::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {

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

void ElementEllipse::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {

    QPointF mousePoint = event->pos();
    QPointF mouseHandler = QPointF(3,3);
    QPointF topLeft = elementRect.topLeft();
    QPointF bottomRight = elementRect.bottomRight();

    if (mousePoint.x() <= (topLeft.x() + mouseHandler.x()) &&
        mousePoint.x() >= (topLeft.x() - mouseHandler.x()) &&
        mousePoint.y() <= (topLeft.y() + mouseHandler.y()) &&
        mousePoint.y() >= (topLeft.y() - mouseHandler.y()))
    {

        setCursor(Qt::SizeFDiagCursor);
    }
    else if (mousePoint.x() <= (bottomRight.x() + mouseHandler.x()) &&
             mousePoint.x() >= (bottomRight.x() - mouseHandler.x()) &&
             mousePoint.y() <= (bottomRight.y() + mouseHandler.y()) &&
             mousePoint.y() >= (bottomRight.y() - mouseHandler.y()))
    {

        setCursor(Qt::SizeFDiagCursor);
    }

    QGraphicsObject::hoverEnterEvent(event);
}

void ElementEllipse::readFromXml(const QXmlStreamAttributes &attributes) {

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

    if (attributes.hasAttribute("borderColor")) {
        borderColor = QColor(attributes.value("borderColor").toString());
    }

    if (attributes.hasAttribute("borderWidth")) {
        borderWidth = attributes.value("borderWidth").toString().toInt();
    }

    if (attributes.hasAttribute("elemAngle")) {
        setAngle(attributes.value("elemAngle").toString().toInt());
    }

    updateBoundingElement();
}

void ElementEllipse::readData(QDataStream &in) {
    QString id;
    qreal xpos;
    qreal ypos;
    qreal zvalue;
    int width;
    int height;
    QColor backColor;
    QColor borderColor;
    int borderWidth;
    qreal angle;

    in >> id
       >> xpos
       >> ypos
       >> zvalue
       >> width
       >> height
       >> backColor
       >> borderColor
       >> borderWidth
       >> angle;

    this->setElementId(id);
    this->setElementXPos(xpos);
    this->setElementYPos(ypos);
    this->setElementZValue(zvalue);
    this->setElementWidth(width);
    this->setElementHeight(height);
    this->backgroundColor = backColor;
    this->borderColor = borderColor;
    this->borderWidth = borderWidth;
    this->setAngle(angle);
    this->updateBoundingElement();
}

QDataStream &operator>>(QDataStream &in,ElementEllipse &rect) {

    QString id;
    qreal xpos;
    qreal ypos;
    qreal zvalue;
    int width;
    int height;
    QColor backColor;
    QColor borderColor;
    int borderWidth;
    qreal angle;

    in >> id
       >> xpos
       >> ypos
       >> zvalue
       >> width
       >> height
       >> backColor
       >> borderColor
       >> borderWidth
       >> angle;

    rect.setElementId(id);
    rect.setElementXPos(xpos);
    rect.setElementYPos(ypos);
    rect.setElementZValue(zvalue);
    rect.setElementWidth(width);
    rect.setElementHeight(height);
    rect.backgroundColor = backColor;
    rect.borderColor = borderColor;
    rect.borderWidth = borderWidth;
    rect.setAngle(angle);
    rect.updateBoundingElement();

    return in;
}
