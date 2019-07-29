#include "ElementMovingText.h"
#include "Helper.h"
#include "XMLObject.h"
#include "TagManager.h"
#include "DrawListUtils.h"
#include "ElementIDHelper.h"
#include <QMessageBox>
#include <QDateTime>
#include <QDate>

int ElementMovingText::iLastIndex_ = 1;

ElementMovingText::ElementMovingText(const QString &szProjPath, const QString &szProjName)
    : Element(szProjPath, szProjName)
{
    elementId = QString(tr("MovingText_%1").arg(iLastIndex_, 4, 10, QChar('0')));
    iLastIndex_++;
    internalElementType = tr("MovingText");
    elementIcon = QIcon(":/images/MovingText.png");
    backgroundColor_ = Qt::white;
    transparentBackground_ = true;
    borderWidth_ = 0;
    borderColor_ = Qt::black;
    period_ = 1.0;
    showOnInitial_ = true;
    szMoveDir_ = tr("从右到左");
    iMoveCharNum_ = 1;

    TagManager::setProjectPath(szProjectPath_);
    DrawListUtils::setProjectPath(szProjectPath_);
    ElementIDHelper::setProjectPath(szProjectPath_);

    init();
    createPropertyList();
    updatePropertyModel();
}

void ElementMovingText::regenerateElementId()
{
    elementId = QString(tr("MovingText_%1").arg(iLastIndex_ - 1, 4, 10, QChar('0')));
    this->updatePropertyModel();
}


/**
 * @brief ElementMovingText::release
 * @details 释放占用的资源
 */
void ElementMovingText::release()
{

}


QRectF ElementMovingText::boundingRect() const
{
    qreal extra = 5;
    QRectF rect(elementRect_.toRect());
    return rect.normalized().adjusted(-extra,-extra,extra,extra);
}

QPainterPath ElementMovingText::shape() const
{
    QPainterPath path;
    path.addRect(elementRect_);
    if (isSelected()) {
        path.addRect(QRectF(elementRect_.topLeft() - QPointF(3,3), elementRect_.topLeft() + QPointF(3,3)));
        path.addRect(QRectF(elementRect_.bottomRight() - QPointF(3,3), elementRect_.bottomRight() + QPointF(3,3)));
    }
    return path;
}

void ElementMovingText::createPropertyList()
{
    // ID
    idProperty_ = new TextProperty(tr("ID"));
    idProperty_->setId(EL_ID);
    idProperty_->setReadOnly(true);
    propList.insert(propList.end(), idProperty_);

    // 标题
    titleProperty_ = new EmptyProperty(tr("标题"));
    propList.insert(propList.end(), titleProperty_);

    // 选择变量
    tagSelectProperty_ = new ListProperty(tr("选择变量"));
    tagSelectProperty_->setId(EL_TAG);
    QStringList varList;
    TagManager::getAllTagName(TagManager::getProjectPath(), varList);
    tagSelectProperty_->setList(varList);
    propList.insert(propList.end(), tagSelectProperty_);

    // 变量文本列表
    tagTextListProperty_ = new TagTextListProperty(tr("变量文本列表"));
    tagTextListProperty_->setId(EL_TAG_TEXT_LIST);
    tagTextListProperty_->setValue(tagTextList_);
    propList.insert(propList.end(), tagTextListProperty_);

    // 文本
    elementTextProperty_ = new TextProperty(tr("文本"));
    elementTextProperty_->setId(EL_TEXT);
    elementTextProperty_->setValue(elementText);
    propList.insert(propList.end(), elementTextProperty_);

    // 移动方向
    moveDirProperty_ = new ListProperty(tr("移动方向"));
    moveDirProperty_->setId(EL_TEXT_MOVE_DIR);
    QStringList moveDirList;
    moveDirList << tr("从左到右") << tr("从右到左");
    moveDirProperty_->setList(moveDirList);
    moveDirProperty_->setValue(szMoveDir_);
    propList.insert(propList.end(), moveDirProperty_);

    // 移动字符数
    moveCharNumProperty_ = new IntegerProperty(tr("移动字符数"));
    moveCharNumProperty_->setId(EL_TEXT_MOVE_CHAR_NUM);
    moveCharNumProperty_->setSettings(0, 5000);
    moveCharNumProperty_->setValue(iMoveCharNum_);
    propList.insert(propList.end(), moveCharNumProperty_);

    // 移动间隔
    periodProperty_ = new DoubleProperty(tr("移动间隔"));
    periodProperty_->setId(EL_PERIOD);
    periodProperty_->setSettings(0, 6000, 5);
    periodProperty_->setValue(period_);
    propList.insert(propList.end(), periodProperty_);

    // 背景颜色
    backgroundColorProperty_ = new ColorProperty(tr("背景颜色"));
    backgroundColorProperty_->setId(EL_BACKGROUND);
    backgroundColorProperty_->setValue(backgroundColor_);
    propList.insert(propList.end(), backgroundColorProperty_);

    // 透明背景颜色
    transparentBackgroundProperty_ = new BoolProperty(tr("透明背景颜色"));
    transparentBackgroundProperty_->setId(EL_TRANSPARENT_BACKGROUND);
    transparentBackgroundProperty_->setTrueText(tr("透明"));
    transparentBackgroundProperty_->setFalseText(tr("不透明"));
    transparentBackgroundProperty_->setValue(transparentBackground_);
    propList.insert(propList.end(), transparentBackgroundProperty_);

    // 字体
    fontProperty_ = new FontProperty(tr("字体"));
    fontProperty_->setId(EL_FONT);
    fontProperty_->setValue(QFont("Arial Black", 12));
    propList.insert(propList.end(), fontProperty_);

    // 文本颜色
    textColorProperty = new ColorProperty(tr("文本颜色"));
    textColorProperty->setId(EL_FONT_COLOR);
    propList.insert(propList.end(),textColorProperty);

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

    xCoordProperty_ = new IntegerProperty(tr("坐标 X"));
    xCoordProperty_->setSettings(0, 5000);
    xCoordProperty_->setId(EL_X);
    propList.insert(propList.end(), xCoordProperty_);

    yCoordProperty_ = new IntegerProperty(tr("坐标 Y"));
    yCoordProperty_->setId(EL_Y);
    yCoordProperty_->setSettings(0, 5000);
    propList.insert(propList.end(), yCoordProperty_);

    zValueProperty_ = new IntegerProperty(tr("Z 值"));
    zValueProperty_->setId(EL_Z_VALUE);
    zValueProperty_->setSettings(-1000, 1000);
    propList.insert(propList.end(), zValueProperty_);

    widthProperty_ = new IntegerProperty(tr("宽度"));
    widthProperty_->setId(EL_WIDTH);
    widthProperty_->setSettings(0, 5000);
    propList.insert(propList.end(), widthProperty_);

    heightProperty_ = new IntegerProperty(tr("高度"));
    heightProperty_->setId(EL_HEIGHT);
    heightProperty_->setSettings(0, 5000);
    propList.insert(propList.end(), heightProperty_);
}

void ElementMovingText::updateElementProperty(uint id, const QVariant &value)
{
    switch (id) {
    case EL_ID:
        elementId = value.toString();
        break;
    case EL_TAG:
        szTagSelected_ = value.toString();
        break;
    case EL_TAG_TEXT_LIST:
        tagTextList_ = value.toStringList();
        break;
    case EL_TEXT:
        elementText = value.toString();
        break;
    case EL_TEXT_MOVE_DIR:
        szMoveDir_ = value.toString();
        break;
    case EL_TEXT_MOVE_CHAR_NUM:
        iMoveCharNum_ = value.toInt();
        break;
    case EL_PERIOD:
        period_ = value.toDouble();
		break;
    case EL_BACKGROUND:
        backgroundColor_ = value.value<QColor>();
        break;
    case EL_TRANSPARENT_BACKGROUND:
        transparentBackground_ = value.toBool();
        break;
    case EL_FONT:
        font_ = value.value<QFont>();
        break;
    case EL_FONT_COLOR:
        textColor = value.value<QColor>();
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
    }

    update();
    scene()->update();
}

void ElementMovingText::updatePropertyModel()
{
    idProperty_->setValue(elementId);
    tagSelectProperty_->setValue(szTagSelected_);
    tagTextListProperty_->setValue(tagTextList_);
    elementTextProperty_->setValue(elementText);
    moveDirProperty_->setValue(szMoveDir_);
    moveCharNumProperty_->setValue(iMoveCharNum_);
    periodProperty_->setValue(period_);
    backgroundColorProperty_->setValue(backgroundColor_);
    transparentBackgroundProperty_->setValue(transparentBackground_);
    fontProperty_->setValue(font_);
    textColorProperty->setValue(textColor);
    borderWidthProperty_->setValue(borderWidth_);
    borderColorProperty_->setValue(borderColor_);
    showOnInitialProperty_->setValue(showOnInitial_);
    xCoordProperty_->setValue(elementXPos);
    yCoordProperty_->setValue(elementYPos);
    zValueProperty_->setValue(elementZValue);
    widthProperty_->setValue(elementWidth);
    heightProperty_->setValue(elementHeight);
}

void ElementMovingText::setClickPosition(QPointF position)
{
    prepareGeometryChange();
    elementXPos = static_cast<int>(position.x());
    elementYPos = static_cast<int>(position.y());
    setX(elementXPos);
    setY(elementYPos);
    elementRect_.setRect(0,0,elementWidth,elementHeight);
    updatePropertyModel();
}

void ElementMovingText::updateBoundingElement()
{
    elementRect_.setRect(0, 0, elementWidth, elementHeight);
}

void ElementMovingText::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->setRenderHints(QPainter::HighQualityAntialiasing | QPainter::TextAntialiasing);

    // 背景色不透明显示
    if(!transparentBackground_) {
        QBrush brush(backgroundColor_);
        painter->fillRect(elementRect_, brush);
    }

    // 绘制文本
    drawMovingText(painter);

    // 绘制边框
    painter->setPen(QPen(borderColor_, borderWidth_));
    painter->setBrush(Qt::NoBrush);
    if(borderWidth_ > 0)
        painter->drawRect(elementRect_);

    if (isSelected()) {
        painter->setPen(QPen(borderColor));
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(boundingRect());

        setCursor(Qt::SizeAllCursor);
        painter->setBrush(Qt::red);
        painter->setPen(Qt::red);
        painter->drawRect(QRectF(elementRect_.topLeft() - QPointF(3,3), elementRect_.topLeft() + QPointF(3,3)));
        painter->drawRect(QRectF(elementRect_.bottomRight() - QPointF(3,3), elementRect_.bottomRight() + QPointF(3,3)));
    }
}

void ElementMovingText::drawMovingText(QPainter *painter)
{
    painter->setPen(textColor);
    painter->setBrush(Qt::NoBrush);
    painter->setFont(font_);

    QRectF rect(elementRect_.toRect());
    QRectF textRect = rect.normalized().adjusted(borderWidth_, borderWidth_, -borderWidth_, -borderWidth_);

    painter->drawText(textRect, Qt::AlignCenter, elementText);
}

void ElementMovingText::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF mousePoint = event->pos();

    if (resizing) {
        setCursor(Qt::SizeFDiagCursor);

        switch (rd) {
        case RdBottomRight:
            elementRect_.setBottomRight(mousePoint);
            elementWidth = static_cast<int>(qAbs(elementRect_.topLeft().x() - elementRect_.bottomRight().x()));
            elementHeight = static_cast<int>(qAbs(elementRect_.topLeft().y() - elementRect_.bottomRight().y()));
            break;
        case RdTopLeft:
            elementRect_.setTopLeft(mousePoint);
            setElementXPos(static_cast<int>(mapToScene(elementRect_.topLeft()).x()));
            setElementYPos(static_cast<int>(mapToScene(elementRect_.topLeft()).y()));
            setElementWidth(static_cast<int>(qAbs(mapToScene(elementRect_.topLeft()).x() - mapToScene(elementRect_.bottomRight()).x())));
            setElementHeight(static_cast<int>(qAbs(mapToScene(elementRect_.topLeft()).y() - mapToScene(elementRect_.bottomRight()).y())));
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

void ElementMovingText::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF mousePoint = event->pos();
    QPointF mouseHandler = QPointF(3,3);
    QPointF topLeft = elementRect_.topLeft();
    QPointF bottomRight = elementRect_.bottomRight();

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

void ElementMovingText::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
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

void ElementMovingText::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QPointF mousePoint = event->pos();
    QPointF mouseHandler = QPointF(3,3);
    QPointF topLeft = elementRect_.topLeft();
    QPointF bottomRight = elementRect_.bottomRight();

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


/**
 * @brief ElementMovingText::getMoveDirString
 * @param szAlign 文本移动方向
 * @return 文本移动方向
 */
QString ElementMovingText::getMoveDirString(const QString& szAlign) const
{
    if(szAlign == tr("从左到右")) {
        return QString("LeftToRight");
    } else if(szAlign == tr("从右到左")) {
        return QString("RightToLeft");
    }
    return QString("");
}


/**
 * @brief ElementMovingText::setMoveDirString
 * @details 设置文本移动方向
 * @param szAlign 文本移动方向
 * @param szAlignSet 待设置文本移动方向
 */
void ElementMovingText::setMoveDirString(const QString& szAlign, QString& szAlignSet)
{
    if(szAlign == QString("LeftToRight")) {
        szAlignSet = tr("从左到右");
    } else if(szAlign == QString("RightToLeft")) {
        szAlignSet = tr("从右到左");
    }
}


void ElementMovingText::writeAsXml(QXmlStreamWriter &writer)
{
    writer.writeStartElement("element");
    writer.writeAttribute("internalType", internalElementType);
    writer.writeAttribute("elementId", elementId);
    writer.writeAttribute("tag", szTagSelected_);
    writer.writeAttribute("tagTextList", tagTextList_.join("|"));
    writer.writeAttribute("elementText", elementText);
    writer.writeAttribute("moveDir", getMoveDirString(szMoveDir_));
    writer.writeAttribute("moveCharNum", QString::number(iMoveCharNum_));
    writer.writeAttribute("period", QString::number(period_));
    writer.writeAttribute("backgroundColor", backgroundColor_.name());
    writer.writeAttribute("transparentBackground", transparentBackground_?"true":"false");
    writer.writeAttribute("textcolor", textColor.name());
    writer.writeAttribute("font", font_.toString());
    writer.writeAttribute("borderColor", borderColor_.name());
    writer.writeAttribute("borderWidth", QString::number(borderWidth_));
    writer.writeAttribute("showOnInitial", showOnInitial_?"true":"false");
    writer.writeAttribute("x", QString::number(x()));
    writer.writeAttribute("y", QString::number(y()));
    writer.writeAttribute("z", QString::number(zValue()));
    writer.writeAttribute("width", QString::number(elementWidth));
    writer.writeAttribute("height", QString::number(elementHeight));
    writer.writeEndElement();
}

void ElementMovingText::readFromXml(const QXmlStreamAttributes &attributes)
{
    if (attributes.hasAttribute("elementId")) {
        QString szID = attributes.value("elementId").toString();
        setElementId(szID);
        int index = getIndexFromIDString(szID);
        if(iLastIndex_ < index) {
            iLastIndex_ = index;
        }
    }

    if (attributes.hasAttribute("tag")) {
        szTagSelected_ = attributes.value("tag").toString();
    }

    if (attributes.hasAttribute("tagTextList")) {
        QString listString = attributes.value("tagTextList").toString();
        tagTextList_ = listString.split('|');
    }

    if (attributes.hasAttribute("elementText")) {
        elementText = attributes.value("elementText").toString();
    }

    if (attributes.hasAttribute("moveDir")) {
        QString szMoveDir = attributes.value("moveDir").toString();
        this->setMoveDirString(szMoveDir, szMoveDir_);
    }

    if (attributes.hasAttribute("moveCharNum")) {
        iMoveCharNum_ = attributes.value("moveCharNum").toString().toInt();
    }



    if (attributes.hasAttribute("period")) {
        period_ = attributes.value("period").toString().toDouble();
    }

    if (attributes.hasAttribute("backgroundColor")) {
        backgroundColor_ = QColor(attributes.value("backgroundColor").toString());
    }

    if (attributes.hasAttribute("transparentBackground")) {
        QString value = attributes.value("transparentBackground").toString();
        transparentBackground_ = false;
        if(value == "true") {
            transparentBackground_ = true;
        }
    }

    if (attributes.hasAttribute("textcolor")) {
        textColor = QColor(attributes.value("textcolor").toString());
    }

    if (attributes.hasAttribute("font")) {
        QString szFont = attributes.value("font").toString();
        font_.fromString(szFont);
    }

    if (attributes.hasAttribute("borderColor")) {
        borderColor_ = QColor(attributes.value("borderColor").toString());
    }

    if (attributes.hasAttribute("borderWidth")) {
        borderWidth_ = attributes.value("borderWidth").toInt();
    }

    if (attributes.hasAttribute("showOnInitial")) {
        QString value = attributes.value("showOnInitial").toString();
        showOnInitial_ = false;
        if(value == "true") {
            showOnInitial_ = true;
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

    updateBoundingElement();
    updatePropertyModel();
}

void ElementMovingText::writeData(QDataStream &out)
{
    out << this->elementId
        << this->szTagSelected_
        << this->tagTextList_
        << this->elementText
        << this->getMoveDirString(szMoveDir_)
        << this->iMoveCharNum_
        << this->period_
        << this->backgroundColor_
        << this->transparentBackground_
        << this->textColor
        << this->font_.toString()
        << this->borderColor_
        << this->borderWidth_
        << this->showOnInitial_
        << this->x()
        << this->y()
        << this->zValue()
        << this->elementWidth
        << this->elementHeight;
}

void ElementMovingText::readData(QDataStream &in)
{
    QString id;
    QString szTagSelected;
    QStringList tagTextList;
    QString text;
    QString szMoveDir;
    int iMoveCharNum;
    double period;
    QColor backgroundColor;
    bool transparentBackground;
    QColor textColor;
    QString font;
    QColor borderColor;
    int borderWidth;
    bool showOnInitial;
    qreal xpos;
    qreal ypos;
    qreal zvalue;
    int width;
    int height;

    in >> id
       >> szTagSelected
       >> tagTextList
       >> text
       >> szMoveDir
       >> iMoveCharNum
       >> period
       >> backgroundColor
       >> transparentBackground
       >> textColor
       >> font
       >> borderColor
       >> borderWidth
       >> showOnInitial
       >> xpos
       >> ypos
       >> zvalue
       >> width
       >> height;

    this->setElementId(id);
    int index = getIndexFromIDString(id);
    if(iLastIndex_ < index) {
        iLastIndex_ = index;
    }
    this->szTagSelected_ = szTagSelected;
    this->tagTextList_ = tagTextList;
    this->elementText = text;
    this->setMoveDirString(szMoveDir, szMoveDir_);
    this->iMoveCharNum_ = iMoveCharNum;

    this->period_ = period;
    this->backgroundColor_ = backgroundColor;
    this->transparentBackground_ = transparentBackground;
    this->textColor = textColor;
    this->font_ = font;
    this->borderColor_ = borderColor;
    this->borderWidth_ = borderWidth;
    this->showOnInitial_ = showOnInitial;
    this->setElementXPos(static_cast<int>(xpos));
    this->setElementYPos(static_cast<int>(ypos));
    this->setElementZValue(static_cast<int>(zvalue));
    this->setElementWidth(width);
    this->setElementHeight(height);
    this->updateBoundingElement();
    this->updatePropertyModel();
}


QDataStream &operator<<(QDataStream &out,const ElementMovingText &ele)
{
    out << ele.elementId
        << ele.szTagSelected_
        << ele.tagTextList_
        << ele.elementText
        << ele.getMoveDirString(ele.szMoveDir_)
        << ele.iMoveCharNum_
        << ele.period_
        << ele.backgroundColor_
        << ele.transparentBackground_
        << ele.textColor
        << ele.font_.toString()
        << ele.borderColor_
        << ele.borderWidth_
        << ele.showOnInitial_
        << ele.x()
        << ele.y()
        << ele.zValue()
        << ele.elementWidth
        << ele.elementHeight;

    return out;
}

QDataStream &operator>>(QDataStream &in, ElementMovingText &ele)
{
    QString id;
    QString szTagSelected;
    QStringList tagTextList;
    QString text;
    QString szMoveDir;
    int iMoveCharNum;
    double period;
    QColor backgroundColor;
    bool transparentBackground;
    QColor textColor;
    QString font;
    QColor borderColor;
    int borderWidth;
    bool showOnInitial;
    qreal xpos;
    qreal ypos;
    qreal zvalue;
    int width;
    int height;

    in >> id
       >> szTagSelected
       >> tagTextList
       >> text
       >> szMoveDir
       >> iMoveCharNum
       >> period
       >> backgroundColor
       >> transparentBackground
       >> textColor
       >> font
       >> borderColor
       >> borderWidth
       >> showOnInitial
       >> xpos
       >> ypos
       >> zvalue
       >> width
       >> height;

    ele.setElementId(id);
    int index = ele.getIndexFromIDString(id);
    if(ele.iLastIndex_ < index) {
        ele.iLastIndex_ = index;
    }
    ele.szTagSelected_ = szTagSelected;
    ele.tagTextList_ = tagTextList;
    ele.elementText = text;
    ele.setMoveDirString(szMoveDir, ele.szMoveDir_);
    ele.iMoveCharNum_ = iMoveCharNum;

    ele.period_ = period;
    ele.backgroundColor_ = backgroundColor;
    ele.transparentBackground_ = transparentBackground;
    ele.textColor = textColor;
    ele.font_ = font;
    ele.borderColor_ = borderColor;
    ele.borderWidth_ = borderWidth;
    ele.showOnInitial_ = showOnInitial;
    ele.setElementXPos(static_cast<int>(xpos));
    ele.setElementYPos(static_cast<int>(ypos));
    ele.setElementZValue(static_cast<int>(zvalue));
    ele.setElementWidth(width);
    ele.setElementHeight(height);
    ele.updateBoundingElement();
    ele.updatePropertyModel();

    return in;
}

