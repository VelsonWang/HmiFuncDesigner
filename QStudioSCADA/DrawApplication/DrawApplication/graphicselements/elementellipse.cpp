#include "elementellipse.h"
#include <QtDebug>

ElementEllipse::ElementEllipse()
{
    elementId = trUtf8("椭圆");
    internalElementType = trUtf8("Ellipse");
    elementIcon = QIcon(":/images/ellipseitem.png");

    init();
    createPropertyList();
    updatePropertyModel();
}

QRectF ElementEllipse::boundingRect() const {

    qreal extra = 5;

    QRectF rect = elementRect.toRect();
    return rect.normalized().adjusted(-extra,-extra,extra,extra);
}

QPainterPath ElementEllipse::shape() const {
    QPainterPath path;
    path.addEllipse(elementRect);

    if (isSelected()) {
        path.addRect(QRectF(elementRect.topLeft() - QPointF(3,3),elementRect.topLeft() + QPointF(3,3)));
        path.addRect(QRectF(elementRect.bottomRight() - QPointF(3,3),elementRect.bottomRight() + QPointF(3,3)));
    }

    return path;
}

void ElementEllipse::createPropertyList() {

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

    borderColorProperty = new ColorProperty(trUtf8("边框颜色"));
    borderColorProperty->setId(EL_BORDER_COLOR);
    propList.insert(propList.end(),borderColorProperty);

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

void ElementEllipse::updateElementProperty(uint id, const QVariant &value) {

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
    case EL_BORDER_COLOR:
        borderColor = value.value<QColor>();
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

void ElementEllipse::updatePropertyModel() {

    idProperty->setValue(elementId);
    xCoordProperty->setValue(elementXPos);
    yCoordProperty->setValue(elementYPos);
    zValueProperty->setValue(elementZValue);
    widthProperty->setValue(elementWidth);
    heightProperty->setValue(elementHeight);
    backColorProperty->setValue(backgroundColor);
    borderColorProperty->setValue(borderColor);
    borderWidthProperty->setValue(borderWidth);
    angleProperty->setValue(elemAngle);
    blockedProperty->setValue(block);
    messageTypeProperty->setValue(messageType);
}

void ElementEllipse::setClickPosition(QPointF position) {

    prepareGeometryChange();
    elementXPos = position.x();
    elementYPos = position.y();
    setX(elementXPos);
    setY(elementYPos);

    elementRect.setRect(0,0,elementWidth,elementHeight);
    updatePropertyModel();
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

    if (isSelected()) {
        setCursor(Qt::SizeAllCursor);
        painter->setBrush(Qt::red);
        painter->setPen(Qt::red);
        painter->drawRect(QRectF(elementRect.topLeft() - QPointF(3,3),elementRect.topLeft() + QPointF(3,3)));
        painter->drawRect(QRectF(elementRect.bottomRight() - QPointF(3,3),elementRect.bottomRight() + QPointF(3,3)));
    }
}

void ElementEllipse::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {

    QPointF mousePoint = event->pos();

    if (resizing) {

        setCursor(Qt::SizeFDiagCursor);

        switch (rd) {

        case RdBottomRight:
            elementRect.setBottomRight(mousePoint);
            elementWidth = qAbs(elementRect.topLeft().x() - elementRect.bottomRight().x());
            elementHeight = qAbs(elementRect.topLeft().y() - elementRect.bottomRight().y());
            break;
        case RdTopLeft:
            elementRect.setTopLeft(mousePoint);
            setElementXPos(mapToScene(elementRect.topLeft()).x());
            setElementYPos(mapToScene(elementRect.topLeft()).y());
            setElementWidth(qAbs(mapToScene(elementRect.topLeft()).x() - mapToScene(elementRect.bottomRight()).x()));
            setElementHeight(qAbs(mapToScene(elementRect.topLeft()).y() - mapToScene(elementRect.bottomRight()).y()));
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
    updatePropertyModel();

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

void ElementEllipse::writeAsXml(QXmlStreamWriter &writer) {

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
    writer.writeAttribute("block",QString(QVariant(block).toString()));
    writer.writeAttribute("indicationrule",indicationRule);
    writer.writeAttribute("linkingType",linkingType);
    writer.writeAttribute("deviceLink",deviceLink);
    writer.writeAttribute("signalLink",signalLink);
    writer.writeAttribute("messageType",messageType);
    writer.writeEndElement();
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

QDataStream &operator<<(QDataStream &out,const ElementEllipse &rect) {

    out << rect.elementId
        << rect.x() << rect.y() << rect.zValue()
        << rect.elementWidth << rect.elementHeight << rect.backgroundColor << rect.borderColor
        << rect.borderWidth  << rect.elemAngle << rect.block << rect.indicationRule;

    return out;
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
    bool block;
    QString rule;

    in >> id >> xpos >> ypos >> zvalue >> width >> height >> backColor
       >> borderColor >> borderWidth >> angle >> block >> rule;

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
    rect.block = block;
    rect.setIndicationRule(rule);
    rect.updateBoundingElement();
    rect.updatePropertyModel();

    return in;
}
