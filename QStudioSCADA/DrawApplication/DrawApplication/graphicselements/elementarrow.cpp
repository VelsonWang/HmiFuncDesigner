#include "elementarrow.h"
#include "math.h"
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
    createPropertyList();
    updatePropertyModel();
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

void ElementArrow::createPropertyList() {

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

    borderWidthProperty = new IntegerProperty(trUtf8("边框宽度"));
    borderWidthProperty->setId(EL_BORDER_WIDTH);
    borderWidthProperty->setSettings(0,5);
    propList.insert(propList.end(),borderWidthProperty);

    angleProperty = new IntegerProperty(trUtf8("角度"));
    angleProperty->setId(EL_ANGLE);
    angleProperty->setSettings(0,360);
    propList.insert(propList.end(),angleProperty);

    blockedProperty = new BoolProperty(trUtf8("块"));
    blockedProperty->setId(EL_BLOCK);
    propList.insert(propList.end(),blockedProperty);

    serviceProperty = new EmptyProperty(trUtf8("服务"));
    propList.insert(propList.end(),serviceProperty);

    messageTypeProperty = new TextProperty(trUtf8("消息类型"));
    messageTypeProperty->setId(EL_MESSAGE_TYPE);
    propList.insert(propList.end(),messageTypeProperty);
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
       case EL_BLOCK:
           block = value.toBool();
           setBlocked(block);
           break;
       case EL_MESSAGE_TYPE:
            messageType = value.toString();
            setMessageType(messageType);
            break;
       }


    update();
    scene()->update();
}

void ElementArrow::updatePropertyModel() {

    idProperty->setValue(elementId);
    xCoordProperty->setValue(elementXPos);
    yCoordProperty->setValue(elementYPos);
    zValueProperty->setValue(elementZValue);
    widthProperty->setValue(elementWidth);
    heightProperty->setValue(elementHeight);
    backColorProperty->setValue(backgroundColor);
    borderWidthProperty->setValue(borderWidth);
    angleProperty->setValue(elemAngle);
    blockedProperty->setValue(block);
    messageTypeProperty->setValue(messageType);
}

void ElementArrow::setClickPosition(QPointF position) {

    prepareGeometryChange();
    setElementXPos(position.x());
    setElementYPos(position.y());

    elementLine.setLine(0,0, elementWidth,elementHeight);
    updatePropertyModel();
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
    updatePropertyModel();

    if (oldPos != pos()) {
        emit elementMoved(oldPos);
    }

    if (resizing) {
        emit elementResized(oldWidth,oldHeight,oldPos);
    }

    QGraphicsObject::mouseReleaseEvent(event);
}

void ElementArrow::writeAsXml(QXmlStreamWriter &writer) {

    writer.writeStartElement("element");
    writer.writeAttribute("internalType",internalElementType);
    writer.writeAttribute("elementId",elementId);
    writer.writeAttribute("x",QString::number(x()));
    writer.writeAttribute("y",QString::number(y()));
    writer.writeAttribute("z",QString::number(zValue()));
    writer.writeAttribute("width",QString::number(elementWidth));
    writer.writeAttribute("height",QString::number(elementHeight));
    writer.writeAttribute("background",backgroundColor.name());
    writer.writeAttribute("borderWidth",QString::number(borderWidth));
    writer.writeAttribute("elemAngle",QString::number(elemAngle));
    writer.writeAttribute("block",QString(QVariant(block).toString()));
    writer.writeAttribute("indicationrule",indicationRule);
    writer.writeAttribute("linkingType",linkingType);
    writer.writeAttribute("deviceLink",deviceLink);
    writer.writeAttribute("signalLink",signalLink);
    writer.writeAttribute("messageType",messageType);
    writer.writeEndElement();
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

    if (attributes.hasAttribute("indicationrule")) {
        setIndicationRule(attributes.value("indicationrule").toString());
    }

    if (attributes.hasAttribute("linkingType")) {
        setLinkingType(attributes.value("linkingType").toString());
    }

    if (attributes.hasAttribute("deviceLink")) {
        setDeviceLink(attributes.value("deviceLink").toString());
    }

    if (attributes.hasAttribute("signalLink")) {
        setSignalLink(attributes.value("signalLink").toString());
    }

    if (attributes.hasAttribute("messageType")) {
        setMessageType(attributes.value("messageType").toString());
    }

    updateBoundingElement();
    updatePropertyModel();
}

/** Дополнить сериализацию новыми свойствами*/
QDataStream &operator<<(QDataStream &out,const ElementArrow &line) {

    out << line.elementId << line.x() << line.y() << line.zValue()
        << line.elementWidth << line.elementHeight << line.backgroundColor
        << line.borderWidth << line.elemAngle << line.block << line.indicationRule;
    return out;
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
    bool block;
    QString rule;

    in >> id >> xpos >> ypos >> zvalue >> width
       >> height >> backColor >> borderWidth
       >> angle >> block >> rule;

    line.setElementId(id);
    line.setElementXPos(xpos);
    line.setElementYPos(ypos);
    line.setElementZValue(zvalue);
    line.setElementWidth(width);
    line.setElementHeight(height);
    line.backgroundColor = backColor;
    line.borderWidth = borderWidth;
    line.setAngle(angle);
    line.block = block;
    line.setIndicationRule(rule);
    line.updateBoundingElement();
    line.updatePropertyModel();

    return in;
}
