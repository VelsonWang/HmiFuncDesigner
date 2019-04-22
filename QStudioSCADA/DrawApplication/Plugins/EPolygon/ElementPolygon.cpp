#include "elementpolygon.h"
#include "TagManager.h"
#include <QtDebug>

int ElementPolygon::iLastIndex_ = 1;

ElementPolygon::ElementPolygon(const QString &projPath) :
    Element(projPath) {
    elementId = QString(tr("Polygon_%1").arg(iLastIndex_, 4, 10, QChar('0')));
    iLastIndex_++;
    internalElementType = trUtf8("Polygon");
    elementIcon = QIcon(":/images/polygon.png");
    fillColor_ = Qt::white;
    isFill_ = true;
    borderWidth_ = 1;
    borderColor_ = Qt::black;
    showOnInitial_ = true;

    clickPoint = -1;

    TagManager::setProjectPath(projPath);

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
    idProperty->setReadOnly(true);
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

    // 选择变量
    tagSelectProperty_ = new ListProperty(tr("选择变量"));
    tagSelectProperty_->setId(EL_TAG);
    QStringList varList;
    TagManager::getAllTagName(TagManager::getProjectPath(), varList);
    tagSelectProperty_->setList(varList);
    propList.insert(propList.end(), tagSelectProperty_);

    // 填充颜色列表
    tagColorListProperty_ = new TagColorListProperty(tr("填充颜色列表"));
    tagColorListProperty_->setId(EL_TAG_COLOR_LIST);
    tagColorListProperty_->setValue(tagColorList_);
    propList.insert(propList.end(), tagColorListProperty_);

    // 填充颜色
    fillColorProperty_ = new ColorProperty(tr("填充颜色"));
    fillColorProperty_->setId(EL_FILL_COLOR);
    fillColorProperty_->setValue(fillColor_);
    propList.insert(propList.end(), fillColorProperty_);

    // 是否填充颜色
    isFillProperty_ = new BoolProperty(tr("填充"));
    isFillProperty_->setId(EL_IS_FILL_COLOR);
    isFillProperty_->setTrueText(tr("填充"));
    isFillProperty_->setFalseText(tr("不填充"));
    isFillProperty_->setValue(isFill_);
    propList.insert(propList.end(), isFillProperty_);

    // 边框宽度
    borderWidthProperty_ = new IntegerProperty(tr("边框宽度"));
    borderWidthProperty_->setId(EL_BORDER_WIDTH);
    borderWidthProperty_->setSettings(0, 1000);
    borderWidthProperty_->setValue(borderWidth_);
    propList.insert(propList.end(), borderWidthProperty_);

    // 边框颜色
    borderColorProperty_ = new ColorProperty(tr("边框颜色"));
    borderColorProperty_->setId(EL_BORDER_COLOR);
    borderColorProperty_->setValue(borderColor_);
    propList.insert(propList.end(), borderColorProperty_);

    // 初始可见性
    showOnInitialProperty_ = new BoolProperty(tr("初始可见性"));
    showOnInitialProperty_->setId(EL_SHOW_ON_INITIAL);
    showOnInitialProperty_->setTrueText(tr("显示"));
    showOnInitialProperty_->setFalseText(tr("不显示"));
    showOnInitialProperty_->setValue(showOnInitial_);
    propList.insert(propList.end(), showOnInitialProperty_);

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
    case EL_TAG:
        szTagSelected_ = value.toString();
        break;
    case EL_TAG_COLOR_LIST:
        tagColorList_ = value.toStringList();
        break;
    case EL_FILL_COLOR:
        fillColor_ = value.value<QColor>();
        break;
    case EL_IS_FILL_COLOR:
        isFill_ = value.toBool();
        break;
    case EL_BORDER_WIDTH:
        borderWidth_ = value.toInt();
        break;
    case EL_BORDER_COLOR:
        borderColor_ = value.value<QColor>();
        break;
    case EL_SHOW_ON_INITIAL:
        showOnInitial_ = value.toBool();
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
    tagSelectProperty_->setValue(szTagSelected_);
    tagColorListProperty_->setValue(tagColorList_);
    fillColorProperty_->setValue(fillColor_);
    isFillProperty_->setValue(isFill_);
    borderWidthProperty_->setValue(borderWidth_);
    borderColorProperty_->setValue(borderColor_);
    showOnInitialProperty_->setValue(showOnInitial_);
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

void ElementPolygon::paint(QPainter *painter,
                           const QStyleOptionGraphicsItem *option,
                           QWidget *widget) {
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->setRenderHints(QPainter::HighQualityAntialiasing | QPainter::SmoothPixmapTransform);

    painter->setPen(QPen(borderColor_, borderWidth_));
    if(isFill_) {
        painter->setBrush(QBrush(fillColor_));
    } else {
        painter->setBrush(Qt::NoBrush);
    }
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

void ElementPolygon::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    QPointF mousePoint = event->pos();

    if (resizing) {
        setCursor(Qt::SizeFDiagCursor);
        points.replace(clickPoint,mousePoint);
        createPath();
        update(boundingRect());
        scene()->update();
        return;
    } else {
        QGraphicsObject::mouseMoveEvent(event);
        QPointF pos = scenePos();

        if(pos.x() < 0) {
            this->setX(0);
        }
        if(pos.x() > iGraphPageWidth_ - boundingRect().width()) {
            this->setX(iGraphPageWidth_ - boundingRect().width());
        }

        if(pos.y() < 0) {
            this->setY(0);
        }
        if(pos.y() > iGraphPageHeight_ - boundingRect().height()) {
            this->setY(iGraphPageHeight_ - boundingRect().height());
        }
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
    writer.writeAttribute("internalType", internalElementType);
    writer.writeAttribute("elementId", elementId);
    writer.writeAttribute("x", QString::number(x()));
    writer.writeAttribute("y", QString::number(y()));
    writer.writeAttribute("z", QString::number(zValue()));
    writer.writeAttribute("tag", szTagSelected_);
    writer.writeAttribute("tagColorList", tagColorList_.join("|"));
    writer.writeAttribute("fillColor", fillColor_.name());
    writer.writeAttribute("isFill", isFill_?"true":"false");
    writer.writeAttribute("borderWidth", QString::number(borderWidth_));
    writer.writeAttribute("borderColor", borderColor_.name());
    writer.writeAttribute("showOnInitial", showOnInitial_?"true":"false");
    writer.writeAttribute("elemAngle", QString::number(elemAngle));
    writer.writeAttribute("points", createPointsXmlString());
    writer.writeEndElement();
}

void ElementPolygon::readFromXml(const QXmlStreamAttributes &attributes) {
    if (attributes.hasAttribute("elementId")) {
        QString szID = attributes.value("elementId").toString();
        setElementId(szID);
        int index = getIndexFromIDString(szID);
        if(iLastIndex_ < index) {
            iLastIndex_ = index;
        }
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
    updatePropertyModel();
}

void ElementPolygon::writeData(QDataStream &out) {
    out << this->elementId
        << this->x()
        << this->y()
        << this->zValue()
        << this->szTagSelected_
        << this->tagColorList_
        << this->fillColor_
        << this->isFill_
        << this->borderWidth_
        << this->borderColor_
        << this->showOnInitial_
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
    int index = getIndexFromIDString(id);
    if(iLastIndex_ < index) {
        iLastIndex_ = index;
    }
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
    this->updatePropertyModel();
}


QDataStream &operator<<(QDataStream &out, const ElementPolygon &polygon) {
    out << polygon.elementId
        << polygon.x()
        << polygon.y()
        << polygon.zValue()
        << polygon.szTagSelected_
        << polygon.tagColorList_
        << polygon.fillColor_
        << polygon.isFill_
        << polygon.borderWidth_
        << polygon.borderColor_
        << polygon.showOnInitial_
        << polygon.elemAngle
        << polygon.points.size();

    for (int i = 0; i < polygon.points.size(); i++) {
        out << polygon.points[i];
    }

    return out;
}

QDataStream &operator>>(QDataStream &in, ElementPolygon &polygon) {
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
    int index = polygon.getIndexFromIDString(id);
    if(polygon.iLastIndex_ < index) {
        polygon.iLastIndex_ = index;
    }
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
    polygon.updatePropertyModel();

    return in;  
}
