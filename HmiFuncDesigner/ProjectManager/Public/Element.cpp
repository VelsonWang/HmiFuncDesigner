#include "Element.h"
#include <QGraphicsSceneHoverEvent>
#include <QPainter>

Element::Element(const QString &szProjPath,
                 const QString &szProjName,
                 QtVariantPropertyManager *propertyMgr)
    : elementId(""),
      szProjectPath_(szProjPath),
      szProjectName_(szProjName),
      variantPropertyManager_(propertyMgr)
{
}

Element::~Element()
{

}

void Element::setElementId(const QString &id)
{
    elementId = id;
}

QString Element::getElementId() const
{
    return elementId;
}

QString Element::getInternalElementType() const
{
    return internalElementType;
}

QIcon Element::getElementIcon() const
{
    return elementIcon;
}

void Element::setElementWidth(int width)
{
    elementWidth = width;
    updatePropertyModel();
}

int Element::getElementWidth() const
{
    return elementWidth;
}

void Element::setElementHeight(int height)
{
    elementHeight = height;
    updatePropertyModel();
}

int Element::getElementHeight() const
{
    return elementHeight;
}

void Element::setElementXPos(int xpos)
{
    elementXPos = xpos;
    setX(elementXPos);
    updatePropertyModel();
}

int Element::getElementXPos() const
{
    return elementXPos;
}

void Element::setElementYPos(int ypos)
{
    elementYPos = ypos;
    setY(elementYPos);
    updatePropertyModel();
}

int Element::getElementYPos() const
{
    return elementYPos;
}

void Element::setElementZValue(int zval)
{
    elementZValue = zval;
    setZValue(elementZValue);
    updatePropertyModel();
}

int Element::getElementZValue() const
{
    return elementZValue;
}

QList<QtProperty*> Element::getPropertyList() const
{
    return propList;
}

void Element::moveTo(int x, int y)
{
    elementXPos = elementXPos + x;
    elementYPos = elementYPos + y;
    setPos(elementXPos, elementYPos);
    updatePropertyModel();
}

void Element::setAngle(qreal angle_)
{
    QTransform transform;
    transform.rotate(angle_);
    elemAngle = angle_;
    setTransform(transform);
}

qreal Element::angle() const
{
    return elemAngle;
}

void Element::init()
{
    setFlags(QGraphicsItem::ItemIsSelectable |
             QGraphicsItem::ItemIsMovable |
             QGraphicsItem::ItemIsFocusable);
    setAcceptHoverEvents(true);

    resizing = false;

    elementWidth = 80;
    elementHeight = 32;
    elementZValue = zValue();
    backgroundColor = QColor(Qt::white);
    signBackgroundColor = QColor(Qt::black);
    borderWidth = 2;
    borderColor = QColor(Qt::black);
    elementText = tr("文本");
    fontSize = 10;

    elemAngle = 0;
    rotationCount = 0;

    setSelected(true);

    rd = ResizeNone;
}

void Element::setBlocked(bool blocked)
{
    setFlag(QGraphicsItem::ItemIsMovable, !blocked);
}


/**
 * @brief Element::setProjectPath
 * @details 设置工程路径
 * @param path 工程路径
 */
void Element::setProjectPath(const QString &path)
{
    if(szProjectPath_ != path) {
        szProjectPath_ = path;
    }
}


/**
 * @brief Element::getProjectPath
 * @details 获取工程路径
 * @return 工程路径
 */
QString Element::getProjectPath() const
{
    return szProjectPath_;
}


/**
 * @brief Element::setProjectName
 * @details 设置工程名称
 * @param name 工程名称
 */
void Element::setProjectName(const QString &name)
{
    if(szProjectName_ != name) {
        szProjectName_ = name;
    }
}


/**
 * @brief Element::getProjectName
 * @details 获取工程名称
 * @return 工程名称(不包含后缀)
 */
QString Element::getProjectName() const
{
    return szProjectName_;
}


/**
 * @brief Element::getIndexFromIDString
 * @details 控件唯一标识字符串，形如："PushButton_0001"
 * @param szID 控件唯一标识
 * @return 分配的索引值
 */
int Element::getIndexFromIDString(const QString &szID)
{
    int pos = szID.indexOf("_");
    if(pos > -1) {
        QString szIndex = szID.right(4);
        bool ok = false;
        int iRet = szIndex.toInt(&ok);
        if(!ok) {
            return 0;
        }
        return iRet;
    }
    return 0;
}

/**
 * @brief Element::getHAlignString
 * @param szAlign 对齐方式
 * @return 水平方向对齐方式
 */
QString Element::getHAlignString(const QString& szAlign) const
{
    if(szAlign == tr("左对齐")) {
        return QString("left");
    } else if(szAlign == tr("居中对齐")) {
        return QString("center");
    } else if(szAlign == tr("右对齐")) {
        return QString("right");
    }
    return QString("");
}


/**
 * @brief Element::setHAlignString
 * @details 设置水平方向对齐方式
 * @param szAlign 水平方向对齐方式
 * @param szAlignSet 待设置垂直方向对齐方式
 */
void Element::setHAlignString(const QString& szAlign, QString& szAlignSet)
{
    if(szAlign == QString("left")) {
        szAlignSet = tr("左对齐");
    } else if(szAlign == QString("center")) {
        szAlignSet = tr("居中对齐");
    } else if(szAlign == QString("right")) {
        szAlignSet = tr("右对齐");
    }
}


/**
 * @brief Element::getVAlignString
 * @param szAlign 对齐方式
 * @return 垂直方向对齐方式
 */
QString Element::getVAlignString(const QString& szAlign) const
{
    if(szAlign == tr("上对齐")) {
        return QString("top");
    } else if(szAlign == tr("居中对齐")) {
        return QString("center");
    } else if(szAlign == tr("下对齐")) {
        return QString("bottom");
    }
    return QString("");
}

/**
 * @brief Element::setVAlignString
 * @details 设置垂直方向对齐方式
 * @param szAlign 垂直方向对齐方式
 * @param szAlignSet 待设置垂直方向对齐方式
 */
void Element::setVAlignString(const QString& szAlign, QString& szAlignSet)
{
    if(szAlign == QString("top")) {
        szAlignSet = tr("上对齐");
    } else if(szAlign == QString("center")) {
        szAlignSet = tr("居中对齐");
    } else if(szAlign == QString("bottom")) {
        szAlignSet = tr("下对齐");
    }
}


/**
 * @brief Element::setGraphPageSize
 * @details 设置所属画面大小
 * @param width 画面宽度
 * @param height 画面高度
 */
void Element::setGraphPageSize(int width, int height)
{
    this->iGraphPageWidth_ = width;
    this->iGraphPageHeight_ = height;
}


/**
 * @brief Element::RestrictedRectangularRegion
 * @details 限制矩形区域
 */
void Element::RestrictedRectangularRegion()
{
    QPointF pos = scenePos();

    if(pos.x() < 0) {
        this->setX(0);
    }
    else if(pos.x() > iGraphPageWidth_ - getElementWidth()) {
        this->setX(iGraphPageWidth_ - getElementWidth());
    }

    if(pos.y() < 0) {
        this->setY(0);
    }
    else if(pos.y() > iGraphPageHeight_ - getElementHeight()) {
        this->setY(iGraphPageHeight_ - getElementHeight());
    }

    scene()->update();
}

void Element::addProperty(QtVariantProperty *property, const QString &id, bool bAddToList)
{
    if(bAddToList) {
        propList.append(property);
    }
    propertyToId_[property] = id;
    idToProperty_[id] = property;
}


void Element::clearProperties()
{
    QMap<QtProperty *, QString>::ConstIterator itProp = propertyToId_.constBegin();
    while (itProp != propertyToId_.constEnd()) {
        delete itProp.key();
        itProp++;
    }
    propertyToId_.clear();
    idToProperty_.clear();
}


void Element::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    update();
    QGraphicsObject::hoverEnterEvent(event);
}


void Element::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    setCursor(QCursor(Qt::ArrowCursor));
    scene()->update(sceneBoundingRect());
    update();
    QGraphicsObject::hoverLeaveEvent(event);
}


void Element::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    if(this->isSelected()) {
        QPointF mousePoint = event->pos();
        QPointF mouseHandler = QPointF(5, 5);
        QPointF topLeft = elementRect.topLeft();
        QPointF topRight = elementRect.topRight();
        QPointF bottomLeft = elementRect.bottomLeft();
        QPointF bottomRight = elementRect.bottomRight();

        // resize top
        if (mousePoint.x() >= (topLeft.x() + mouseHandler.x()) &&
                mousePoint.x() <= (topRight.x() - mouseHandler.x()) &&
                mousePoint.y() >= (topLeft.y() - mouseHandler.y()) &&
                mousePoint.y() <= (topLeft.y() + mouseHandler.y())) {
            setCursor(Qt::SizeVerCursor);
        }
        // resize bottom
        else if (mousePoint.x() >= (bottomLeft.x() + mouseHandler.x()) &&
                 mousePoint.x() <= (bottomRight.x() - mouseHandler.x()) &&
                 mousePoint.y() >= (bottomLeft.y() - mouseHandler.y()) &&
                 mousePoint.y() <= (bottomLeft.y() + mouseHandler.y())) {
            setCursor(Qt::SizeVerCursor);
        }
        // resize left
        else if (mousePoint.x() >= (topLeft.x() - mouseHandler.x()) &&
                 mousePoint.x() <= (topLeft.x() + mouseHandler.x()) &&
                 mousePoint.y() >= (topLeft.y() + mouseHandler.y()) &&
                 mousePoint.y() <= (bottomLeft.y() - mouseHandler.y())) {
            setCursor(Qt::SizeHorCursor);
        }
        // resize right
        else if (mousePoint.x() >= (topRight.x() - mouseHandler.x()) &&
                 mousePoint.x() <= (topRight.x() + mouseHandler.x()) &&
                 mousePoint.y() >= (topRight.y() + mouseHandler.y()) &&
                 mousePoint.y() <= (bottomRight.y() - mouseHandler.y())) {
            setCursor(Qt::SizeHorCursor);
        }
        // resize top left
        else if (mousePoint.x() <= (topLeft.x() + mouseHandler.x()) &&
                 mousePoint.x() >= (topLeft.x() - mouseHandler.x()) &&
                 mousePoint.y() <= (topLeft.y() + mouseHandler.y()) &&
                 mousePoint.y() >= (topLeft.y() - mouseHandler.y())) {
            setCursor(Qt::SizeFDiagCursor);
        }
        // resize bottom right
        else if (mousePoint.x() <= (bottomRight.x() + mouseHandler.x()) &&
                 mousePoint.x() >= (bottomRight.x() - mouseHandler.x()) &&
                 mousePoint.y() <= (bottomRight.y() + mouseHandler.y()) &&
                 mousePoint.y() >= (bottomRight.y() - mouseHandler.y())) {
            setCursor(Qt::SizeFDiagCursor);
        }
        // resize bottom left
        else if (mousePoint.x() <= (bottomLeft.x() + mouseHandler.x()) &&
                 mousePoint.x() >= (bottomLeft.x() - mouseHandler.x()) &&
                 mousePoint.y() >= (bottomLeft.y() - mouseHandler.y()) &&
                 mousePoint.y() <= (bottomLeft.y() + mouseHandler.y())) {
            setCursor(Qt::SizeBDiagCursor);
        }
        // resize top right
        else if (mousePoint.x() <= (topRight.x() + mouseHandler.x()) &&
                 mousePoint.x() >= (topRight.x() - mouseHandler.x()) &&
                 mousePoint.y() >= (topRight.y() - mouseHandler.y()) &&
                 mousePoint.y() <= (topRight.y() + mouseHandler.y())) {
            setCursor(Qt::SizeBDiagCursor);
        }
        else {
            setCursor(Qt::ArrowCursor);
        }
    }
    QGraphicsObject::hoverMoveEvent(event);
}

void Element::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (!isSelected()){
        QGraphicsItem::mouseMoveEvent(event);
        return;
    }

    QPointF mousePoint = event->pos();

    if (resizing) {
        if(rd & ResizeLeft) {
            elementRect.setLeft(mousePoint.x());
            setElementXPos(static_cast<int>(mapToScene(elementRect.topLeft()).x()));
            setElementWidth(static_cast<int>(qAbs(mapToScene(elementRect.topLeft()).x() - mapToScene(elementRect.topRight()).x())));
        }

        if(rd & ResizeRight) {
            elementRect.setRight(mousePoint.x());
            setElementWidth(static_cast<int>(qAbs(mapToScene(elementRect.topLeft()).x() - mapToScene(elementRect.topRight()).x())));
        }

        if(rd & ResizeTop) {
            elementRect.setTop(mousePoint.y());
            setElementYPos(static_cast<int>(mapToScene(elementRect.topLeft()).y()));
            setElementHeight(static_cast<int>(qAbs(mapToScene(elementRect.topLeft()).y() - mapToScene(elementRect.bottomLeft()).y())));
        }

        if(rd & ResizeBottom) {
            elementRect.setBottom(mousePoint.y());
            setElementHeight(static_cast<int>(qAbs(elementRect.topLeft().y() - elementRect.bottomLeft().y())));
        }

        scene()->update();
        return;
    } else {
        QGraphicsObject::mouseMoveEvent(event);
        // 限制矩形区域
        RestrictedRectangularRegion();
    }
}

void Element::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF mousePoint = event->pos();
    QPointF mouseHandler = QPointF(5, 5);
    QPointF topLeft = elementRect.topLeft();
    QPointF topRight = elementRect.topRight();
    QPointF bottomLeft = elementRect.bottomLeft();
    QPointF bottomRight = elementRect.bottomRight();

    rd = ResizeNone;
    resizing = false;

    // resize top
    if (mousePoint.x() >= (topLeft.x() + 0) &&
            mousePoint.x() <= (topRight.x() - 0) &&
            mousePoint.y() >= (topLeft.y() - mouseHandler.y()) &&
            mousePoint.y() <= (topLeft.y() + mouseHandler.y())) {
        rd |= ResizeTop;
        resizing = true;
    }

    // resize bottom
    if (mousePoint.x() >= (bottomLeft.x() + 0) &&
             mousePoint.x() <= (bottomRight.x() - 0) &&
             mousePoint.y() >= (bottomLeft.y() - mouseHandler.y()) &&
             mousePoint.y() <= (bottomLeft.y() + mouseHandler.y())) {
        rd |= ResizeBottom;
        resizing = true;
    }

    // resize left
    if (mousePoint.x() >= (topLeft.x() - mouseHandler.x()) &&
             mousePoint.x() <= (topLeft.x() + mouseHandler.x()) &&
             mousePoint.y() >= (topLeft.y() + 0) &&
             mousePoint.y() <= (bottomLeft.y() - 0)) {
        rd |= ResizeLeft;
        resizing = true;
    }

    // resize right
    if (mousePoint.x() >= (topRight.x() - mouseHandler.x()) &&
             mousePoint.x() <= (topRight.x() + mouseHandler.x()) &&
             mousePoint.y() >= (topRight.y() + 0) &&
             mousePoint.y() <= (bottomRight.y() - 0)) {
        rd |= ResizeRight;
        resizing = true;
    }

    switch (rd) {
    case ResizeRight:
    case ResizeLeft:
        setCursor(Qt::SizeHorCursor);
        break;
    case ResizeBottom:
    case ResizeTop:
        setCursor(Qt::SizeVerCursor);
        break;
    case ResizeRight | ResizeBottom:
    case ResizeLeft  | ResizeTop:
        setCursor(Qt::SizeFDiagCursor);
        break;
    case ResizeLeft  | ResizeBottom:
    case ResizeRight | ResizeTop:
        setCursor(Qt::SizeBDiagCursor);
        break;
    default:
        setCursor(Qt::ArrowCursor);
        break;
    }

    oldPos = pos();
    oldWidth = elementWidth;
    oldHeight = elementHeight;

    QGraphicsObject::mousePressEvent(event);
}


void Element::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsObject::mouseDoubleClickEvent(event);
}



void Element::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    setCursor(Qt::ArrowCursor);
    elementXPos = static_cast<int>(pos().x());
    elementYPos = static_cast<int>(pos().y());
    updatePropertyModel();

    if (oldPos != pos()) {
        emit elementMoved(oldPos);
    }

    if (resizing) {
        emit elementResized(oldWidth, oldHeight, oldPos);
    }
    resizing = false;
    rd = ResizeNone;

    QGraphicsObject::mouseReleaseEvent(event);
}

/**
 * @brief Element::paintSelected
 * @details 绘制选中状态
 * @param painter
 * @param iLineWidth 线宽
 */
void Element::paintSelected(QPainter *painter, int iLineWidth)
{
    if (isSelected()) {
        painter->save();

        painter->setBrush(Qt::NoBrush);

        QPen pen(Qt::red, iLineWidth, Qt::DashLine);
        painter->setPen(pen);
        QRectF rect = elementRect.adjusted(-1, -1, 1, 1);
        painter->drawRect(rect);

        QPen pen2(Qt::red, iLineWidth, Qt::SolidLine);
        painter->setPen(pen2);
        QPointF pt = QPointF(3, 3);

        painter->drawRect(QRectF(rect.topLeft() - pt, rect.topLeft() + pt));
        QPointF ptMid = QPointF(rect.left() + rect.width() / 2, rect.top());
        painter->drawRect(QRectF(ptMid - pt, ptMid + pt));
        painter->drawRect(QRectF(rect.topRight() - pt, rect.topRight() + pt));

        ptMid = QPointF(rect.left(), rect.top() + rect.height() / 2);
        painter->drawRect(QRectF(ptMid - pt, ptMid + pt));
        ptMid = QPointF(rect.right(), rect.top() + rect.height() / 2);
        painter->drawRect(QRectF(ptMid - pt, ptMid + pt));

        painter->drawRect(QRectF(rect.bottomLeft() - pt, rect.bottomLeft() + pt));
        ptMid = QPointF(rect.left() + rect.width() / 2, rect.bottom());
        painter->drawRect(QRectF(ptMid - pt, ptMid + pt));
        painter->drawRect(QRectF(rect.bottomRight() - pt, rect.bottomRight() + pt));

        painter->restore();
    }
}


