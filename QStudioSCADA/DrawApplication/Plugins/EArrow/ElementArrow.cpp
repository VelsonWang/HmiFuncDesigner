#include "ElementArrow.h"
#include "math.h"
#include <QDebug>

static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;
int ElementArrow::iLastIndex_ = 1;

ElementArrow::ElementArrow(const QString &szProjPath, const QString &szProjName)
    : Element(szProjPath, szProjName),
      arrowSize(10)
{
    elementId = QString(tr("Arrow_%1").arg(iLastIndex_ - 1, 4, 10, QChar('0')));
    iLastIndex_++;
    internalElementType = tr("Arrow");
    elementIcon = QIcon(":/images/arrowitem.png");
    borderWidth_ = 1;
    borderColor_ = Qt::black;
    init();
    createPropertyList();
    updatePropertyModel();
}


void ElementArrow::regenerateElementId()
{
    elementId = QString(tr("Arrow_%1").arg(iLastIndex_, 4, 10, QChar('0')));
    this->updatePropertyModel();
}

/**
 * @brief ElementArrow::release
 * @details 释放占用的资源
 */
void ElementArrow::release()
{

}

QRectF ElementArrow::boundingRect() const
{
    qreal extra = 5;
    const qreal x1 = elementLine.p1().x();
    const qreal x2 = elementLine.p2().x();
    const qreal y1 = elementLine.p1().y();
    const qreal y2 = elementLine.p2().y();
    qreal lx = qMin(x1, x2);
    qreal rx = qMax(x1, x2);
    qreal ty = qMin(y1, y2);
    qreal by = qMax(y1, y2);

    return QRectF(lx, ty, rx - lx, by - ty).normalized()
            .adjusted(-extra, -extra, extra, extra);
}

void ElementArrow::createPropertyList()
{
    idProperty = new TextProperty(tr("ID"));
    idProperty->setId(EL_ID);
    idProperty->setReadOnly(true);
    propList.insert(propList.end(), idProperty);

    titleProperty = new EmptyProperty(tr("标题"));
    propList.insert(propList.end(), titleProperty);

    // 线条宽度
    borderWidthProperty_ = new IntegerProperty(tr("线条宽度"));
    borderWidthProperty_->setId(EL_BORDER_WIDTH);
    borderWidthProperty_->setSettings(0, 1000);
    borderWidthProperty_->setValue(borderWidth_);
    propList.insert(propList.end(), borderWidthProperty_);

    // 线条颜色
    borderColorProperty_ = new ColorProperty(tr("线条颜色"));
    borderColorProperty_->setId(EL_BORDER_COLOR);
    borderColorProperty_->setValue(borderColor_);
    propList.insert(propList.end(), borderColorProperty_);

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

void ElementArrow::updateElementProperty(uint id, const QVariant &value)
{
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
    case EL_BORDER_WIDTH:
        borderWidth_ = value.toInt();
        break;
    case EL_BORDER_COLOR:
        borderColor_ = value.value<QColor>();
        break;
    case EL_ANGLE:
        elemAngle = value.toInt();
        setAngle(elemAngle);
        break;
    }

    scene()->update();
    update();
}

void ElementArrow::updatePropertyModel()
{
    idProperty->setValue(elementId);
    xCoordProperty->setValue(elementXPos);
    yCoordProperty->setValue(elementYPos);
    zValueProperty->setValue(elementZValue);
    widthProperty_->setValue(elementWidth);
    heightProperty_->setValue(elementHeight);
    borderWidthProperty_->setValue(borderWidth_);
    borderColorProperty_->setValue(borderColor_);
    angleProperty->setValue(elemAngle);
}

void ElementArrow::setClickPosition(QPointF position)
{
    prepareGeometryChange();
    setElementXPos(static_cast<int>(position.x()));
    setElementYPos(static_cast<int>(position.y()));
    elementLine.setLine(0, 0, elementWidth, elementHeight);
    updatePropertyModel();
}

void ElementArrow::updateBoundingElement()
{
    elementLine.setLine(0, 0, elementWidth, elementHeight);
}

void ElementArrow::paint(QPainter *painter,
                         const QStyleOptionGraphicsItem *option,
                         QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->setRenderHints(QPainter::HighQualityAntialiasing | QPainter::TextAntialiasing);
    painter->setPen(QPen(borderColor_, borderWidth_));
    painter->drawLine(elementLine);

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

void ElementArrow::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (resizing) {
        setCursor(Qt::SizeFDiagCursor);
        QPointF mousePoint = event->pos();

        switch (rd) {
        case RdBottomRight:
            elementLine.setP2(mousePoint);
            elementWidth = static_cast<int>(qAbs(elementLine.p1().x() - elementLine.p2().x()));
            elementHeight = static_cast<int>(qAbs(elementLine.p1().y() - elementLine.p2().y()));
            break;
        case RdTopLeft:
            elementLine.setP1(mousePoint);
            setElementXPos(static_cast<int>(mapToScene(elementLine.p1()).x()));
            setElementYPos(static_cast<int>(mapToScene(elementLine.p1()).y()));
            setElementWidth(static_cast<int>(qAbs(mapToScene(elementLine.p1()).x() - mapToScene(elementLine.p2()).x())));
            setElementHeight(static_cast<int>(qAbs(mapToScene(elementLine.p1()).y() - mapToScene(elementLine.p2()).y())));
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
        // 限制矩形区域
        RestrictedRectangularRegion();
    }
}

void ElementArrow::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF mousePoint = event->pos();
    QPointF mouseHandler = QPointF(3,3);
    QPointF pp1 = elementLine.p1();
    QPointF pp2 = elementLine.p2();

    if (mousePoint.x() <= (pp1.x() + mouseHandler.x()) &&
        mousePoint.x() >= (pp1.x() - mouseHandler.x()) &&
        mousePoint.y() <= (pp1.y() + mouseHandler.y()) &&
        mousePoint.y() >= (pp1.y() - mouseHandler.y())) {
        rd = RdTopLeft;
        resizing = true;
        setCursor(Qt::SizeFDiagCursor);
    } else if (mousePoint.x() <= (pp2.x() + mouseHandler.x()) &&
             mousePoint.x() >= (pp2.x() - mouseHandler.x()) &&
             mousePoint.y() <= (pp2.y() + mouseHandler.y()) &&
             mousePoint.y() >= (pp2.y() - mouseHandler.y())) {
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

void ElementArrow::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    setCursor(Qt::ArrowCursor);
    elementXPos = static_cast<int>(pos().x());
    elementYPos = static_cast<int>(pos().y());
    updatePropertyModel();

    if (oldPos != pos()) {
        emit elementMoved(oldPos);
    }

    if (resizing) {
        emit elementResized(oldWidth,oldHeight,oldPos);
    }

    QGraphicsObject::mouseReleaseEvent(event);
}

void ElementArrow::writeAsXml(QXmlStreamWriter &writer)
{
    writer.writeStartElement("element");
    writer.writeAttribute("internalType",internalElementType);
    writer.writeAttribute("elementId",elementId);
    writer.writeAttribute("x",QString::number(x()));
    writer.writeAttribute("y",QString::number(y()));
    writer.writeAttribute("z",QString::number(zValue()));
    writer.writeAttribute("width",QString::number(elementWidth));
    writer.writeAttribute("height",QString::number(elementHeight));
    writer.writeAttribute("borderWidth", QString::number(borderWidth_));
    writer.writeAttribute("borderColor", borderColor_.name());
    writer.writeAttribute("elemAngle", QString::number(elemAngle));
    writer.writeEndElement();
}

void ElementArrow::readFromXml(const QXmlStreamAttributes &attributes)
{
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

    if (attributes.hasAttribute("borderWidth")) {
        borderWidth_ = attributes.value("borderWidth").toInt();
    }

    if (attributes.hasAttribute("borderColor")) {
        borderColor_ = QColor(attributes.value("borderColor").toString());
    }

    if (attributes.hasAttribute("elemAngle")) {
        setAngle(attributes.value("elemAngle").toString().toInt());
    }

    updateBoundingElement();
    updatePropertyModel();
}

void ElementArrow::writeData(QDataStream &out)
{
    out << this->elementId
        << this->x()
        << this->y()
        << this->zValue()
        << this->elementWidth
        << this->elementHeight
        << this->borderWidth_
        << this->borderColor_
        << this->elemAngle;
}

void ElementArrow::readData(QDataStream &in)
{
    QString id;
    qreal xpos;
    qreal ypos;
    qreal zvalue;
    int width;
    int height;
    int borderWidth;
    QColor borderColor;
    qreal angle;

    in >> id
       >> xpos
       >> ypos
       >> zvalue
       >> width
       >> height
       >> borderWidth
       >> borderColor
       >> angle;

    this->setElementId(id);
    int index = getIndexFromIDString(id);
    if(iLastIndex_ < index) {
        iLastIndex_ = index;
    }
    this->setElementXPos(static_cast<int>(xpos));
    this->setElementYPos(static_cast<int>(ypos));
    this->setElementZValue(static_cast<int>(zvalue));
    this->setElementWidth(width);
    this->setElementHeight(height);
    this->borderWidth_ = borderWidth;
    this->borderColor_ = borderColor;
    this->setAngle(angle);
    this->updateBoundingElement();
    this->updatePropertyModel();
}

QDataStream &operator<<(QDataStream &out,const ElementArrow &arrow)
{
    out << arrow.elementId
        << arrow.x()
        << arrow.y()
        << arrow.zValue()
        << arrow.elementWidth
        << arrow.elementHeight
        << arrow.borderWidth_
        << arrow.borderColor_
        << arrow.elemAngle;

    return out;
}

QDataStream &operator>>(QDataStream &in, ElementArrow &arrow)
{
    QString id;
    qreal xpos;
    qreal ypos;
    qreal zvalue;
    int width;
    int height;
    int borderWidth;
    QColor borderColor;
    qreal angle;

    in >> id
       >> xpos
       >> ypos
       >> zvalue
       >> width
       >> height
       >> borderWidth
       >> borderColor
       >> angle;

    arrow.setElementId(id);
    int index = arrow.getIndexFromIDString(id);
    if(arrow.iLastIndex_ < index) {
        arrow.iLastIndex_ = index;
    }
    arrow.setElementXPos(static_cast<int>(xpos));
    arrow.setElementYPos(static_cast<int>(ypos));
    arrow.setElementZValue(static_cast<int>(zvalue));
    arrow.setElementWidth(width);
    arrow.setElementHeight(height);
    arrow.borderWidth_ = borderWidth;
    arrow.borderColor_ = borderColor;
    arrow.setAngle(angle);
    arrow.updateBoundingElement();
    arrow.updatePropertyModel();

    return in;
}
