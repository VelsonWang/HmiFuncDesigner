#include "elementpolygon.h"
#include <QtDebug>

ElementPolygon::ElementPolygon() {
    elementId = trUtf8("Polygon");
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

    foreach (QPointF point, points) {
        if (first) {
            minXPoint.setX(point.x());
            first = false;
        }
        else {
            if (point.x() < minXPoint.x()) {
                minXPoint = point;
            }
        }
    }

    points.remove(points.indexOf(minXPoint));
    createPath();
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
    elementXPos = position.x();
    elementYPos = position.y();
}

void ElementPolygon::updateBoundingElement() {
    createPath();
}


void ElementPolygon::paint(QPainter *painter) {
    if(!showOnInitial_) {
        return;
    }

    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    painter->translate(QPoint(elementXPos, elementYPos));
    painter->rotate(elemAngle);

    painter->setPen(QPen(borderColor_, borderWidth_));
    if(isFill_) {
        painter->setBrush(QBrush(fillColor_));
    } else {
        painter->setBrush(Qt::NoBrush);
    }
    painter->drawPolygon(polygon);
    painter->restore();
}

void ElementPolygon::mouseMoveEvent(QMouseEvent *event) {
    Q_UNUSED(event)
}

void ElementPolygon::mousePressEvent(QMouseEvent *event) {
    Q_UNUSED(event)
}


void ElementPolygon::mouseReleaseEvent(QMouseEvent *event) {
    Q_UNUSED(event)
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
        setElementZValue(attributes.value("z").toString().toInt());
    }

    if (attributes.hasAttribute("tag")) {
        szTagSelected_ = attributes.value("tag").toString();
    }

    if (attributes.hasAttribute("tagColorList")) {
        QString listString = attributes.value("tagColorList").toString();
        tagColorList_ = listString.split('|');
    }

    if (attributes.hasAttribute("fillColor")) {
        fillColor_ = QColor(attributes.value("fillColor").toString());
    }

    if (attributes.hasAttribute("isFill")) {
        QString value = attributes.value("isFill").toString();
        isFill_ = false;
        if(value == "true") {
            isFill_ = true;
        }
    }

    if (attributes.hasAttribute("borderWidth")) {
        borderWidth_ = attributes.value("borderWidth").toInt();
    }

    if (attributes.hasAttribute("borderColor")) {
        borderColor_ = QColor(attributes.value("borderColor").toString());
    }

    if (attributes.hasAttribute("showOnInitial")) {
        QString value = attributes.value("showOnInitial").toString();
        showOnInitial_ = false;
        if(value == "true") {
            showOnInitial_ = true;
        }
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
    QString szTagSelected;
    QStringList tagColorList;
    QString fillColor;
    bool isFill;
    int borderWidth;
    QColor borderColor;
    qreal angle;
    int pointsCount;
    bool showOnInitial;
    QVector <QPointF> points;

    in >> id
       >> xpos
       >> ypos
       >> zvalue
       >> szTagSelected
       >> tagColorList
       >> fillColor
       >> isFill
       >> borderWidth
       >> borderColor
       >> showOnInitial
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
    this->szTagSelected_ = szTagSelected;
    this->tagColorList_ = tagColorList;
    this->fillColor_ = fillColor;
    this->isFill_ = isFill;
    this->borderWidth_ = borderWidth;
    this->borderColor_ = borderColor;
    this->showOnInitial_ = showOnInitial;
    this->setAngle(angle);
    this->points.clear();
    this->points = points;
    this->updateBoundingElement();
}


QDataStream &operator>>(QDataStream &in,ElementPolygon &polygon) {
    QString id;
    qreal xpos;
    qreal ypos;
    qreal zvalue;
    QString szTagSelected;
    QStringList tagColorList;
    QString fillColor;
    bool isFill;
    int borderWidth;
    QColor borderColor;
    bool showOnInitial;
    qreal angle;
    int pointsCount;
    QVector <QPointF> points;

    in >> id
       >> xpos
       >> ypos
       >> zvalue
       >> szTagSelected
       >> tagColorList
       >> fillColor
       >> isFill
       >> borderWidth
       >> borderColor
       >> showOnInitial
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
    polygon.szTagSelected_ = szTagSelected;
    polygon.tagColorList_ = tagColorList;
    polygon.fillColor_ = fillColor;
    polygon.isFill_ = isFill;
    polygon.borderWidth_ = borderWidth;
    polygon.borderColor_ = borderColor;
    polygon.showOnInitial_ = showOnInitial;
    polygon.setAngle(angle);
    polygon.points.clear();
    polygon.points = points;
    polygon.updateBoundingElement();

    return in;
}
