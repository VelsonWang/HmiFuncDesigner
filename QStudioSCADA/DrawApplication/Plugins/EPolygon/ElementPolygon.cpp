#include "elementpolygon.h"
#include <QtDebug>

ElementPolygon::ElementPolygon()
{
    elementId = trUtf8("多边形");
    internalElementType = trUtf8("Polygon");
    elementIcon = QIcon(":/images/polygon.png");

    clickPoint = -1;

    init();
    createPropertyList();
    updatePropertyModel();
    createPoints();
}

void ElementPolygon::addNodePoint() {

    qreal minx, y = 0;
    bool first = true;

    foreach (QPointF point,points) {
        if (first) {
            minx = point.x();
            first = false;
        }
        else {
            if (point.x() < minx) {
                minx = point.x(); y = point.y();
            }
        }
    }

    points.append(QPointF(minx + 20,y + 20));
    updateBoundingElement();
}

void ElementPolygon::removeNodePoint() {

    QPointF minXPoint;
    bool first = true;

    if (points.size() < 4) {
        return;
    }

    foreach (QPointF point,points) {
        if (first) {
            minXPoint.setX(point.x());
            first = false;
        }
        else {
            if (point.x() < minXPoint.x()) {minXPoint = point;}
        }
    }

    points.remove(points.indexOf(minXPoint));
    createPath();
    update(boundingRect());
    scene()->update();
}

void ElementPolygon::createPoints() {

    points.push_back(QPointF(45,45));
    points.push_back(QPointF(150,75));
    points.push_back(QPointF(200,150));
    createPath();
}

void ElementPolygon::createPath() {

    polygon.clear();
    for (int i = 0; i < points.count(); i++) {
        polygon.append(points[i]);
    }
}

QRectF ElementPolygon::boundingRect() const {

    qreal extra = 5;
    qreal minx, maxx, miny, maxy;
    bool first = true;

    foreach (QPointF point, points) {
        if (first) {
            minx = point.x();
            miny = point.y();
            maxx = point.x();
            maxy = point.y();
            first = false;
        }
        else {
            if (point.x() < minx) minx = point.x();
            if (point.x() > maxx) maxx = point.x();
            if (point.y() < miny) miny = point.y();
            if (point.y() > maxy) maxy = point.y();
        }
    }

    return QRectF(minx,miny,maxx - minx, maxy - miny).normalized()
            .adjusted(-extra,-extra,extra,extra);
}

QPainterPath ElementPolygon::shape() const {
    QPainterPath path;
    path.addPolygon(polygon);

    if (isSelected()) {

        for (int i = 0; i < points.count(); i++) {
            path.addRect(QRectF(points[i] - QPointF(3,3),points[i] + QPointF(3,3)));
        }
    }

    return path;
}

void ElementPolygon::createPropertyList() {

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
}

void ElementPolygon::updateElementProperty(uint id, const QVariant &value) {

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
    }

    scene()->update();
    update();
}

void ElementPolygon::updatePropertyModel() {

    idProperty->setValue(elementId);
    xCoordProperty->setValue(elementXPos);
    yCoordProperty->setValue(elementYPos);
    zValueProperty->setValue(elementZValue);
    backColorProperty->setValue(backgroundColor);
    borderColorProperty->setValue(borderColor);
    borderWidthProperty->setValue(borderWidth);
    angleProperty->setValue(elemAngle);
}

void ElementPolygon::setClickPosition(QPointF position) {

    prepareGeometryChange();
    elementXPos = position.x();
    elementYPos = position.y();
    setX(elementXPos);
    setY(elementYPos);

    updatePropertyModel();
}

void ElementPolygon::updateBoundingElement() {

    prepareGeometryChange();
    createPath();
}

void ElementPolygon::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->setRenderHints(QPainter::HighQualityAntialiasing | QPainter::SmoothPixmapTransform);

    painter->setPen(QPen(borderColor,borderWidth));
    painter->setBrush(QBrush(backgroundColor));
    painter->drawPolygon(polygon);

    if (isSelected()) {

        painter->setPen(Qt::gray);
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(boundingRect());

        setCursor(Qt::SizeAllCursor);
        painter->setBrush(Qt::red);
        painter->setPen(Qt::red);

        for (int i = 0; i < points.count(); i++) {
            painter->drawRect(QRectF(points[i] - QPointF(3,3),points[i] + QPointF(3,3)));
        }
    }
}

void ElementPolygon::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{

    QPointF mousePoint = event->pos();

    if (resizing) {

        setCursor(Qt::SizeFDiagCursor);

        points.replace(clickPoint,mousePoint);
        createPath();
        update(boundingRect());
        scene()->update();
        return;
    }
    else {
        QGraphicsObject::mouseMoveEvent(event);
    }
}

void ElementPolygon::mousePressEvent(QGraphicsSceneMouseEvent *event) {

    QPointF mousePoint = event->pos();

    if (event->button() & Qt::LeftButton) {

        for (clickPoint = 0; clickPoint < points.count(); clickPoint++) {
            if (hasClickedOn(mousePoint,points.at(clickPoint))) break;
        }

        if (clickPoint == points.count())  {
            clickPoint = -1;
            resizing = false;
        }
        else {
            resizing = true;
            event->accept();
        }
    }

    oldPos = pos();
    oldWidth = elementWidth;
    oldHeight = elementHeight;

    QGraphicsObject::mousePressEvent(event);
}

bool ElementPolygon::hasClickedOn(QPointF pressPoint, QPointF point) const {
    return (
        pressPoint.x() >= point.x() - 3 &&
        pressPoint.x() <  point.x() + 3 &&
        pressPoint.y() >= point.y() - 3 &&
        pressPoint.y() <  point.y() + 3
    );
}

void ElementPolygon::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {

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

/*void ElementPolygon::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {

    QPointF mousePoint = event->pos();
    QPointF mouseHandler = QPointF(10,10);
    QPointF topLeft = boundingRect().topLeft();
    QPointF bottomRight = boundingRect().bottomRight();

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
}*/

QString ElementPolygon::createPointsXmlString() const {

    QString xmlString;

    foreach (QPointF point, points) {
        QString tempx = QString::number(point.x()) + ",";
        QString tempy = QString::number(point.y()) + ",";
        xmlString.append(tempx);
        xmlString.append(tempy);
    }

    return xmlString;
}

void ElementPolygon::writeAsXml(QXmlStreamWriter &writer) {

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
    writer.writeAttribute("points",createPointsXmlString());
    writer.writeEndElement();
}

void ElementPolygon::readFromXml(const QXmlStreamAttributes &attributes) {

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

    if (attributes.hasAttribute("points")) {
        QStringList list = attributes.value("points").toString().split(",",QString::SkipEmptyParts);
        QVector <QPointF> m_points;

        for (int i = 0; i < list.size() - 1; i += 2) {
            m_points.push_back(QPointF(list.at(i).toInt(),list.at(i+1).toInt()));
        }

        points.clear();
        points = m_points;
    }

    updateBoundingElement();
    updatePropertyModel();
}

void ElementPolygon::writeData(QDataStream &out) {

    out << this->elementId
        << this->x()
        << this->y()
        << this->zValue()
        << this->elementWidth
        << this->elementHeight
        << this->backgroundColor
        << this->borderColor
        << this->borderWidth
        << this->elemAngle
        << this->points.size();

    for (int i = 0; i < this->points.size(); i++) {
        out << this->points[i];
    }
}


void ElementPolygon::readData(QDataStream &in) {

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
    int pointsCount;
    QVector <QPointF> points;

    in >> id
       >> xpos
       >> ypos
       >> zvalue
       >> width
       >> height
       >> backColor
       >> borderColor
       >> borderWidth
       >> angle
       >> pointsCount;

    for (int i = 0; i < pointsCount; i++) {
        QPointF point;
        in >> point;
        points.push_back(point);
    }

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
    this->points.clear();
    this->points = points;
    this->updateBoundingElement();
    this->updatePropertyModel();
}


QDataStream &operator<<(QDataStream &out,const ElementPolygon &polygon)
{

    out << polygon.elementId
        << polygon.x()
        << polygon.y()
        << polygon.zValue()
        << polygon.elementWidth
        << polygon.elementHeight
        << polygon.backgroundColor
        << polygon.borderColor
        << polygon.borderWidth
        << polygon.elemAngle
        << polygon.points.size();

    for (int i = 0; i < polygon.points.size(); i++) {
        out << polygon.points[i];
    }

    return out;
}

QDataStream &operator>>(QDataStream &in,ElementPolygon &polygon)
{

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
    int pointsCount;
    QVector <QPointF> points;

    in >> id
       >> xpos
       >> ypos
       >> zvalue
       >> width
       >> height
       >> backColor
       >> borderColor
       >> borderWidth
       >> angle
       >> pointsCount;

    for (int i = 0; i < pointsCount; i++) {
        QPointF point;
        in >> point;
        points.push_back(point);
    }

    polygon.setElementId(id);
    polygon.setElementXPos(xpos);
    polygon.setElementYPos(ypos);
    polygon.setElementZValue(zvalue);
    polygon.setElementWidth(width);
    polygon.setElementHeight(height);
    polygon.backgroundColor = backColor;
    polygon.borderColor = borderColor;
    polygon.borderWidth = borderWidth;
    polygon.setAngle(angle);
    polygon.points.clear();
    polygon.points = points;
    polygon.updateBoundingElement();
    polygon.updatePropertyModel();

    return in;
}
