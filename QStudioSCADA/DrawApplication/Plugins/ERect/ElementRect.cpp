#include "elementrect.h"
#include "TagManager.h"
#include <QDebug>

int ElementRect::iLastIndex_ = 1;

ElementRect::ElementRect(const QString &szProjPath, const QString &szProjName) :
    Element(szProjPath, szProjName)
{
    elementId = QString(tr("Rect_%1").arg(iLastIndex_, 4, 10, QChar('0')));
    iLastIndex_++;
    internalElementType = tr("Rect");
    elementIcon = QIcon(":/images/rectitem.png");
    fillColor_ = Qt::white;
    isFill_ = true;
    borderWidth_ = 1;
    borderColor_ = Qt::black;
    showOnInitial_ = true;
    TagManager::setProjectPath(szProjectPath_);
    init();
    createPropertyList();
    updatePropertyModel();
}

void ElementRect::regenerateElementId() {
    elementId = QString(tr("Rect_%1").arg(iLastIndex_ - 1, 4, 10, QChar('0')));
    this->updatePropertyModel();
}


/**
 * @brief ElementRect::release
 * @details 释放占用的资源
 */
void ElementRect::release()
{

}

QRectF ElementRect::boundingRect() const {
    qreal extra = 5;
    QRectF rect(elementRect.toRect());
    return rect.normalized().adjusted(-extra, -extra, extra, extra);
}

QPainterPath ElementRect::shape() const {
    QPainterPath path;
    path.addRect(elementRect);

    if (isSelected()) {
        path.addRect(QRectF(elementRect.topLeft() - QPointF(3,3),elementRect.topLeft() + QPointF(3,3)));
        path.addRect(QRectF(elementRect.bottomRight() - QPointF(3,3),elementRect.bottomRight() + QPointF(3,3)));
    }

    return path;
}

void ElementRect::createPropertyList() {
    idProperty = new TextProperty(tr("ID"));
    idProperty->setId(EL_ID);
    idProperty->setReadOnly(true);
    propList.insert(propList.end(), idProperty);

    titleProperty = new EmptyProperty(tr("标题"));
    propList.insert(propList.end(), titleProperty);

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

    xCoordProperty = new IntegerProperty(tr("坐标 X"));
    xCoordProperty->setSettings(0, 5000);
    xCoordProperty->setId(EL_X);
    propList.insert(propList.end(), xCoordProperty);

    yCoordProperty = new IntegerProperty(tr("坐标 Y"));
    yCoordProperty->setId(EL_Y);
    yCoordProperty->setSettings(0, 5000);
    propList.insert(propList.end(), yCoordProperty);

    zValueProperty = new IntegerProperty(tr("Z 值"));
    zValueProperty->setId(EL_Z_VALUE);
    zValueProperty->setSettings(-1000, 1000);
    propList.insert(propList.end(), zValueProperty);

    // 宽度
    widthProperty_ = new IntegerProperty(tr("宽度"));
    widthProperty_->setId(EL_WIDTH);
    widthProperty_->setSettings(0, 5000);
    propList.insert(propList.end(), widthProperty_);

    // 高度
    heightProperty_ = new IntegerProperty(tr("高度"));
    heightProperty_->setId(EL_HEIGHT);
    heightProperty_->setSettings(0, 5000);
    propList.insert(propList.end(), heightProperty_);

    // 旋转角度
    angleProperty = new IntegerProperty(tr("角度"));
    angleProperty->setId(EL_ANGLE);
    angleProperty->setSettings(0,360);
    propList.insert(propList.end(),angleProperty);
}

void ElementRect::updateElementProperty(uint id, const QVariant &value) {
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

void ElementRect::updatePropertyModel() {
    idProperty->setValue(elementId);
    xCoordProperty->setValue(elementXPos);
    yCoordProperty->setValue(elementYPos);
    zValueProperty->setValue(elementZValue);
    widthProperty_->setValue(elementWidth);
    heightProperty_->setValue(elementHeight);
    tagSelectProperty_->setValue(szTagSelected_);
    tagColorListProperty_->setValue(tagColorList_);
    fillColorProperty_->setValue(fillColor_);
    isFillProperty_->setValue(isFill_);
    borderWidthProperty_->setValue(borderWidth_);
    borderColorProperty_->setValue(borderColor_);
    showOnInitialProperty_->setValue(showOnInitial_);
    angleProperty->setValue(elemAngle);
}

void ElementRect::setClickPosition(QPointF position) {
    prepareGeometryChange();
    elementXPos = position.x();
    elementYPos = position.y();
    setX(elementXPos);
    setY(elementYPos);
    elementRect.setRect(0, 0, elementWidth, elementHeight);
    updatePropertyModel();
}

void ElementRect::updateBoundingElement() {
    elementRect.setRect(0, 0, elementWidth, elementHeight);
}

void ElementRect::paint(QPainter *painter,
                        const QStyleOptionGraphicsItem *option,
                        QWidget *widget) {
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->setRenderHints(QPainter::HighQualityAntialiasing | QPainter::TextAntialiasing);
    painter->setPen(QPen(borderColor_, borderWidth_));
    if(isFill_) {
        painter->setBrush(QBrush(fillColor_));
    } else {
        painter->setBrush(Qt::NoBrush);
    }
    painter->drawRect(elementRect);

    if (isSelected()) {
        setCursor(Qt::SizeAllCursor);
        painter->setBrush(Qt::red);
        painter->setPen(Qt::red);
        painter->drawRect(QRectF(elementRect.topLeft() - QPointF(3,3),elementRect.topLeft() + QPointF(3,3)));
        painter->drawRect(QRectF(elementRect.bottomRight() - QPointF(3,3),elementRect.bottomRight() + QPointF(3,3)));
    }
}

void ElementRect::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
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
    } else {
        QGraphicsObject::mouseMoveEvent(event);
        QPointF pos_ = scenePos();

        if(pos_.x() < 0) {
            this->setX(0);
        }
        if(pos_.x() > iGraphPageWidth_ - getElementWidth()) {
            this->setX(iGraphPageWidth_ - getElementWidth());
        }

        if(pos_.y() < 0) {
            this->setY(0);
        }
        if(pos_.y() > iGraphPageHeight_ - getElementHeight()) {
            this->setY(iGraphPageHeight_ - getElementHeight());
        }
    }
}

void ElementRect::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QPointF mousePoint = event->pos();
    QPointF mouseHandler = QPointF(3,3);
    QPointF topLeft = elementRect.topLeft();
    QPointF bottomRight = elementRect.bottomRight();

    if (mousePoint.x() <= (topLeft.x() + mouseHandler.x()) &&
        mousePoint.x() >= (topLeft.x() - mouseHandler.x()) &&
        mousePoint.y() <= (topLeft.y() + mouseHandler.y()) &&
        mousePoint.y() >= (topLeft.y() - mouseHandler.y())) {
        rd = RdTopLeft;
        resizing = true;
        setCursor(Qt::SizeFDiagCursor);
    } else if (mousePoint.x() <= (bottomRight.x() + mouseHandler.x()) &&
             mousePoint.x() >= (bottomRight.x() - mouseHandler.x()) &&
             mousePoint.y() <= (bottomRight.y() + mouseHandler.y()) &&
             mousePoint.y() >= (bottomRight.y() - mouseHandler.y())) {
        rd = RdBottomRight;
        resizing = true;
        setCursor(Qt::SizeFDiagCursor);
    } else {
        resizing = false;
        rd = RdNone;
    }

    oldPos = pos();
    oldWidth = elementWidth;
    oldHeight = elementHeight;

    QGraphicsObject::mousePressEvent(event);
}

void ElementRect::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
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

void ElementRect::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    QPointF mousePoint = event->pos();
    QPointF mouseHandler = QPointF(3,3);
    QPointF topLeft = elementRect.topLeft();
    QPointF bottomRight = elementRect.bottomRight();

    if (mousePoint.x() <= (topLeft.x() + mouseHandler.x()) &&
        mousePoint.x() >= (topLeft.x() - mouseHandler.x()) &&
        mousePoint.y() <= (topLeft.y() + mouseHandler.y()) &&
        mousePoint.y() >= (topLeft.y() - mouseHandler.y())) {

        setCursor(Qt::SizeFDiagCursor);
    } else if (mousePoint.x() <= (bottomRight.x() + mouseHandler.x()) &&
             mousePoint.x() >= (bottomRight.x() - mouseHandler.x()) &&
             mousePoint.y() <= (bottomRight.y() + mouseHandler.y()) &&
             mousePoint.y() >= (bottomRight.y() - mouseHandler.y())) {

        setCursor(Qt::SizeFDiagCursor);
    }

    QGraphicsObject::hoverEnterEvent(event);
}

void ElementRect::writeAsXml(QXmlStreamWriter &writer) {
    writer.writeStartElement("element");
    writer.writeAttribute("internalType",internalElementType);
    writer.writeAttribute("elementId",elementId);
    writer.writeAttribute("x",QString::number(x()));
    writer.writeAttribute("y",QString::number(y()));
    writer.writeAttribute("z",QString::number(zValue()));
    writer.writeAttribute("width",QString::number(elementWidth));
    writer.writeAttribute("height",QString::number(elementHeight));
    writer.writeAttribute("tag", szTagSelected_);
    writer.writeAttribute("tagColorList", tagColorList_.join("|"));
    writer.writeAttribute("fillColor", fillColor_.name());
    writer.writeAttribute("isFill", isFill_?"true":"false");
    writer.writeAttribute("borderWidth", QString::number(borderWidth_));
    writer.writeAttribute("borderColor", borderColor_.name());
    writer.writeAttribute("showOnInitial", showOnInitial_?"true":"false");
    writer.writeAttribute("elemAngle", QString::number(elemAngle));
    writer.writeEndElement();
}

void ElementRect::readFromXml(const QXmlStreamAttributes &attributes) {
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

    if (attributes.hasAttribute("width")) {
        setElementWidth(attributes.value("width").toString().toInt());
    }

    if (attributes.hasAttribute("height")) {
        setElementHeight(attributes.value("height").toString().toInt());
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

    updateBoundingElement();
    updatePropertyModel();
}

void ElementRect::writeData(QDataStream &out) {
    out << this->elementId
        << this->x()
        << this->y()
        << this->zValue()
        << this->elementWidth
        << this->elementHeight
        << this->szTagSelected_
        << this->tagColorList_
        << this->fillColor_
        << this->isFill_
        << this->borderWidth_
        << this->borderColor_
        << this->showOnInitial_
        << this->elemAngle;
}

void ElementRect::readData(QDataStream &in) {
    QString id;
    qreal xpos;
    qreal ypos;
    qreal zvalue;
    int width;
    int height;
    QString szTagSelected;
    QStringList tagColorList;
    QColor fillColor;
    bool isFill;
    int borderWidth;
    QColor borderColor;
    qreal angle;
    bool showOnInitial;

    in >> id
       >> xpos
       >> ypos
       >> zvalue
       >> width
       >> height
       >> szTagSelected
       >> tagColorList
       >> fillColor
       >> isFill
       >> borderWidth
       >> borderColor
       >> showOnInitial
       >> angle;

    this->setElementId(id);
    int index = getIndexFromIDString(id);
    if(iLastIndex_ < index) {
        iLastIndex_ = index;
    }
    this->setElementXPos(xpos);
    this->setElementYPos(ypos);
    this->setElementZValue(zvalue);
    this->setElementWidth(width);
    this->setElementHeight(height);
    this->szTagSelected_ = szTagSelected;
    this->tagColorList_ = tagColorList;
    this->fillColor_ = fillColor;
    this->isFill_ = isFill;
    this->borderWidth_ = borderWidth;
    this->borderColor_ = borderColor;
    this->showOnInitial_ = showOnInitial;
    this->setAngle(angle);
    this->updateBoundingElement();
    this->updatePropertyModel();
}

QDataStream &operator<<(QDataStream &out, const ElementRect &rect) {
    out << rect.elementId
        << rect.x()
        << rect.y()
        << rect.zValue()
        << rect.elementWidth
        << rect.elementHeight
        << rect.szTagSelected_
        << rect.tagColorList_
        << rect.fillColor_
        << rect.isFill_
        << rect.borderWidth_
        << rect.borderColor_
        << rect.showOnInitial_
        << rect.elemAngle;

    return out;
}

QDataStream &operator>>(QDataStream &in, ElementRect &rect) {
    QString id;
    qreal xpos;
    qreal ypos;
    qreal zvalue;
    int width;
    int height;
    QString szTagSelected;
    QStringList tagColorList;
    QString fillColor;
    bool isFill;
    int borderWidth;
    QColor borderColor;
    bool showOnInitial;
    qreal angle;

    in >> id
       >> xpos
       >> ypos
       >> zvalue
       >> width
       >> height
       >> szTagSelected
       >> tagColorList
       >> fillColor
       >> isFill
       >> borderWidth
       >> borderColor
       >> showOnInitial
       >> angle;

    rect.setElementId(id);
    int index = rect.getIndexFromIDString(id);
    if(rect.iLastIndex_ < index) {
        rect.iLastIndex_ = index;
    }
    rect.setElementXPos(xpos);
    rect.setElementYPos(ypos);
    rect.setElementZValue(zvalue);
    rect.setElementWidth(width);
    rect.setElementHeight(height);
    rect.szTagSelected_ = szTagSelected;
    rect.tagColorList_ = tagColorList;
    rect.fillColor_ = fillColor;
    rect.isFill_ = isFill;
    rect.borderWidth_ = borderWidth;
    rect.borderColor_ = borderColor;
    rect.showOnInitial_ = showOnInitial;
    rect.setAngle(angle);
    rect.updateBoundingElement();
    rect.updatePropertyModel();

    return in;
}
