#include "elementpolygon.h"
#include <QtDebug>

ElementPolygon::ElementPolygon()
{
    elementId = trUtf8("多边形");
    internalElementType = trUtf8("Polygon");
    clickPoint = -1;
    init();
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
    return path;
}

void ElementPolygon::setClickPosition(QPointF position) {

    prepareGeometryChange();
    elementXPos = position.x();
    elementYPos = position.y();
    setX(elementXPos);
    setY(elementYPos);
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
}

void ElementPolygon::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event)
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

    if (oldPos != pos()) {
        emit elementMoved(oldPos);
    }

    if (resizing) {
        emit elementResized(oldWidth,oldHeight,oldPos);
    }

    QGraphicsObject::mouseReleaseEvent(event);
}


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

    return in;
}
