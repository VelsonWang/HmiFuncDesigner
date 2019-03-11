#include "elementline.h"
#include <QtDebug>

ElementLine::ElementLine()
{
    elementId = trUtf8("直线");
    internalElementType = trUtf8("Line");
    elementIcon = QIcon(":/images/lineitem.png");

    init();
    createPropertyList();
    updatePropertyModel();
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

void ElementLine::createPropertyList() {

    idProperty = new TextProperty(trUtf8("ID"));
    idProperty->setId(EL_ID);
    propList.insert(propList.end(),idProperty);

    titleProperty = new EmptyProperty(trUtf8("标题"));
    propList.insert(propList.end(),titleProperty);

    xCoordProperty = new IntegerProperty(trUtf8("坐标 X"));
    xCoordProperty->setSettings(0,5000);
    xCoordProperty->setId(EL_X);
    propList.insert(propList.end(),xCoordProperty);

    yCoordProperty = new IntegerProperty(trUtf8("坐标 Y"));
    yCoordProperty->setId(EL_Y);
    yCoordProperty->setSettings(0,5000);
    propList.insert(propList.end(),yCoordProperty);

    zValueProperty = new IntegerProperty(trUtf8("Z 值"));
    zValueProperty->setId(EL_Z_VALUE);
    zValueProperty->setSettings(-1000,1000);
    propList.insert(propList.end(),zValueProperty);

    widthProperty = new IntegerProperty(trUtf8("宽度"));
    widthProperty->setId(EL_WIDTH);
    widthProperty->setSettings(0,5000);
    propList.insert(propList.end(),widthProperty);

    heightProperty = new IntegerProperty(trUtf8("高度"));
    heightProperty->setId(EL_HEIGHT);
    heightProperty->setSettings(0,5000);
    propList.insert(propList.end(),heightProperty);

    backColorProperty = new ColorProperty(trUtf8("背景颜色"));
    backColorProperty->setId(EL_BACKGROUND);
    propList.insert(propList.end(),backColorProperty);

    borderWidthProperty = new IntegerProperty(trUtf8("边框颜色"));
    borderWidthProperty->setId(EL_BORDER_WIDTH);
    borderWidthProperty->setSettings(0,5);
    propList.insert(propList.end(),borderWidthProperty);

    angleProperty = new IntegerProperty(trUtf8("角度"));
    angleProperty->setId(EL_ANGLE);
    angleProperty->setSettings(0,360);
    propList.insert(propList.end(),angleProperty);
}

void ElementLine::updateElementProperty(uint id, const QVariant &value) {

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

void ElementLine::updatePropertyModel() {

    idProperty->setValue(elementId);
    xCoordProperty->setValue(elementXPos);
    yCoordProperty->setValue(elementYPos);
    zValueProperty->setValue(elementZValue);
    widthProperty->setValue(elementWidth);
    heightProperty->setValue(elementHeight);
    backColorProperty->setValue(backgroundColor);
    borderWidthProperty->setValue(borderWidth);
    angleProperty->setValue(elemAngle);
}

void ElementLine::setClickPosition(QPointF position) {

    prepareGeometryChange();
    setElementXPos(position.x());
    setElementYPos(position.y());

    elementLine.setLine(0,0, elementWidth,elementHeight);
    updatePropertyModel();
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

    if (isSelected()) {
        setCursor(Qt::SizeAllCursor);
        painter->setBrush(Qt::red);
        painter->setPen(Qt::red);
        painter->drawRect(QRectF(elementLine.p1() - QPointF(3,3),elementLine.p1() + QPointF(3,3)));
        painter->drawRect(QRectF(elementLine.p2() - QPointF(3,3),elementLine.p2() + QPointF(3,3)));
    }
}

void ElementLine::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {

    if (resizing) {

        setCursor(Qt::SizeFDiagCursor);
        QPointF mousePoint = event->pos();

        switch (rd) {

        case RdBottomRight:
            elementLine.setP2(mousePoint);
            elementWidth = qAbs(elementLine.p1().x() - elementLine.p2().x());
            elementHeight = qAbs(elementLine.p1().y() - elementLine.p2().y());
            break;
        case RdTopLeft:
            elementLine.setP1(mousePoint);
            setElementXPos(mapToScene(elementLine.p1()).x());
            setElementYPos(mapToScene(elementLine.p1()).y());
            setElementWidth(qAbs(mapToScene(elementLine.p1()).x() - mapToScene(elementLine.p2()).x()));
            setElementHeight(qAbs(mapToScene(elementLine.p1()).y() - mapToScene(elementLine.p2()).y()));
            updateBoundingElement();
            break;
        case RdNone:
            QGraphicsObject::mouseMoveEvent(event);
            break;
        }

        scene()->update();
        return;
    }
    else {
        QGraphicsObject::mouseMoveEvent(event);
    }
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
    updatePropertyModel();

    if (oldPos != pos()) {
        emit elementMoved(oldPos);
    }

    if (resizing) {
        emit elementResized(oldWidth,oldHeight,oldPos);
    }

    QGraphicsObject::mouseReleaseEvent(event);
}

void ElementLine::writeAsXml(QXmlStreamWriter &writer) {

    writer.writeStartElement("element");
    writer.writeAttribute("internalType",internalElementType);
    writer.writeAttribute("elementId",elementId);
    writer.writeAttribute("x",QString::number(x()));
    writer.writeAttribute("y",QString::number(y()));
    writer.writeAttribute("z",QString::number(zValue()));
    writer.writeAttribute("width",QString::number(elementWidth));
    writer.writeAttribute("height",QString::number(elementHeight));
    writer.writeAttribute("background",backgroundColor.name());
    writer.writeAttribute("borderColor",borderColor.name());
    writer.writeAttribute("borderWidth",QString::number(borderWidth));
    writer.writeAttribute("elemAngle",QString::number(elemAngle));
    writer.writeEndElement();
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

    if (attributes.hasAttribute("block")) {
        setBlocked(attributes.value("block").toString().toInt());
    }

    updateBoundingElement();
    updatePropertyModel();
}

void ElementLine::writeData(QDataStream &out) {

    out << this->elementId
        << this->x()
        << this->y()
        << this->zValue()
        << this->elementWidth
        << this->elementHeight
        << this->backgroundColor
        << this->borderWidth
        << this->elemAngle;
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
    bool block;

    in >> id
       >> xpos
       >> ypos
       >> zvalue
       >> width
       >> height
       >> backColor
       >> borderWidth
       >> angle
       >> block;

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
    this->updatePropertyModel();
}

QDataStream &operator<<(QDataStream &out,const ElementLine &line) {

    out << line.elementId
        << line.x()
        << line.y()
        << line.zValue()
        << line.elementWidth
        << line.elementHeight
        << line.backgroundColor
        << line.borderWidth
        << line.elemAngle;
    return out;
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
    line.updatePropertyModel();

    return in;
}
