#include "ElementTagTextList.h"
#include "Helper.h"
#include "XMLObject.h"
#include "TagManager.h"
#include "DrawListUtils.h"
#include "ElementIDHelper.h"
#include <QMessageBox>
#include <QDateTime>
#include <QDate>

int ElementTagTextList::iLastIndex_ = 1;

ElementTagTextList::ElementTagTextList(const QString &szProjPath, const QString &szProjName)
    : Element(szProjPath, szProjName)
{
    elementId = QString(tr("TagTextList_%1").arg(iLastIndex_, 4, 10, QChar('0')));
    iLastIndex_++;
    internalElementType = tr("TagTextList");
    elementIcon = QIcon(":/images/TagTextList.png");
    szHAlign_ = tr("左对齐");
    szVAlign_ = tr("居中对齐");
    backgroundColor_ = Qt::white;
    transparentBackground_ = true;
    borderWidth_ = 0;
    borderColor_ = Qt::black;
    showOnInitial_ = true;

    TagManager::setProjectPath(szProjectPath_);
    DrawListUtils::setProjectPath(szProjectPath_);
    ElementIDHelper::setProjectPath(szProjectPath_);

    init();
    createPropertyList();
    updatePropertyModel();
}

void ElementTagTextList::regenerateElementId()
{
    elementId = QString(tr("TagTextList_%1").arg(iLastIndex_ - 1, 4, 10, QChar('0')));
    this->updatePropertyModel();
}


/**
 * @brief ElementTagTextList::release
 * @details 释放占用的资源
 */
void ElementTagTextList::release()
{

}


QRectF ElementTagTextList::boundingRect() const
{
    qreal extra = 5;
    QRectF rect(elementRect_.toRect());
    return rect.normalized().adjusted(-extra,-extra,extra,extra);
}

QPainterPath ElementTagTextList::shape() const
{
    QPainterPath path;
    path.addRect(elementRect_);
    if (isSelected()) {
        path.addRect(QRectF(elementRect_.topLeft() - QPointF(3,3), elementRect_.topLeft() + QPointF(3,3)));
        path.addRect(QRectF(elementRect_.bottomRight() - QPointF(3,3), elementRect_.bottomRight() + QPointF(3,3)));
    }
    return path;
}

void ElementTagTextList::createPropertyList()
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

    // 水平对齐
    hAlignProperty_ = new ListProperty(tr("水平对齐"));
    hAlignProperty_->setId(EL_H_ALIGN);
    QStringList hAlignList;
    hAlignList << tr("左对齐") << tr("居中对齐") << tr("右对齐");
    hAlignProperty_->setList(hAlignList);
    propList.insert(propList.end(), hAlignProperty_);

    // 垂直对齐
    vAlignProperty_ = new ListProperty(tr("水平对齐"));
    vAlignProperty_->setId(EL_V_ALIGN);
    QStringList vAlignList;
    vAlignList << tr("上对齐") << tr("居中对齐") << tr("下对齐");
    vAlignProperty_->setList(vAlignList);
    propList.insert(propList.end(), vAlignProperty_);

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

void ElementTagTextList::updateElementProperty(uint id, const QVariant &value)
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
    case EL_H_ALIGN:
        szHAlign_ = value.toString();
        break;
    case EL_V_ALIGN:
        szVAlign_ = value.toString();
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

void ElementTagTextList::updatePropertyModel()
{
    idProperty_->setValue(elementId);
    tagSelectProperty_->setValue(szTagSelected_);
    tagTextListProperty_->setValue(tagTextList_);
    elementTextProperty_->setValue(elementText);
    hAlignProperty_->setValue(szHAlign_);
    vAlignProperty_->setValue(szVAlign_);
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

void ElementTagTextList::setClickPosition(QPointF position)
{
    prepareGeometryChange();
    elementXPos = static_cast<int>(position.x());
    elementYPos = static_cast<int>(position.y());
    setX(elementXPos);
    setY(elementYPos);
    elementRect_.setRect(0,0,elementWidth,elementHeight);
    updatePropertyModel();
}

void ElementTagTextList::updateBoundingElement()
{
    elementRect_.setRect(0, 0, elementWidth, elementHeight);
}

void ElementTagTextList::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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
    drawTagTextList(painter);

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

void ElementTagTextList::drawTagTextList(QPainter *painter)
{
    painter->setPen(textColor);
    painter->setBrush(Qt::NoBrush);
    painter->setFont(font_);

    int hFlags = Qt::AlignLeft;
    if(szHAlign_ == tr("左对齐")) {
        hFlags = Qt::AlignLeft;
    } else if(szHAlign_ == tr("居中对齐")) {
        hFlags = Qt::AlignHCenter;
    } else if(szHAlign_ == tr("右对齐")) {
        hFlags = Qt::AlignRight;
    }

    int vFlags = Qt::AlignVCenter;
    if(szVAlign_ == tr("上对齐")) {
        vFlags = Qt::AlignTop;
    } else if(szVAlign_ == tr("居中对齐")) {
        vFlags = Qt::AlignVCenter;
    } else if(szVAlign_ == tr("下对齐")) {
        vFlags = Qt::AlignBottom;
    }

    QRectF rect(elementRect_.toRect());
    QRectF textRect = rect.normalized().adjusted(borderWidth_, borderWidth_, -borderWidth_, -borderWidth_);

    painter->drawText(textRect, hFlags|vFlags, elementText);
}

void ElementTagTextList::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
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

void ElementTagTextList::mousePressEvent(QGraphicsSceneMouseEvent *event)
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

void ElementTagTextList::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
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

void ElementTagTextList::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
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


void ElementTagTextList::writeAsXml(QXmlStreamWriter &writer)
{
    writer.writeStartElement("element");
    writer.writeAttribute("internalType", internalElementType);
    writer.writeAttribute("elementId", elementId);
    writer.writeAttribute("tag", szTagSelected_);
    writer.writeAttribute("tagTextList", tagTextList_.join("|"));
    writer.writeAttribute("elementText", elementText);
    writer.writeAttribute("halign", getHAlignString(szHAlign_));
    writer.writeAttribute("valign", getVAlignString(szVAlign_));
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

void ElementTagTextList::readFromXml(const QXmlStreamAttributes &attributes)
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

    if (attributes.hasAttribute("halign")) {
        QString align = attributes.value("halign").toString();
        this->setHAlignString(align, szHAlign_);
    }

    if (attributes.hasAttribute("valign")) {
        QString align = attributes.value("valign").toString();
        this->setVAlignString(align, szVAlign_);
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

void ElementTagTextList::writeData(QDataStream &out)
{
    out << this->elementId
        << this->szTagSelected_
        << this->tagTextList_
        << this->elementText
        << this->getHAlignString(szHAlign_)
        << this->getVAlignString(szVAlign_)
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

void ElementTagTextList::readData(QDataStream &in)
{
    QString id;
    QString szTagSelected;
    QStringList tagTextList;
    QString text;
    QString hAlign;
    QString vAlign;
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
       >> hAlign
       >> vAlign
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
    this->setHAlignString(hAlign, szHAlign_);
    this->setVAlignString(vAlign, szVAlign_);
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


QDataStream &operator<<(QDataStream &out, const ElementTagTextList &ele)
{
    out << ele.elementId
        << ele.szTagSelected_
        << ele.tagTextList_
        << ele.elementText
        << ele.getHAlignString(ele.szHAlign_)
        << ele.getVAlignString(ele.szVAlign_)
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

QDataStream &operator>>(QDataStream &in, ElementTagTextList &ele)
{
    QString id;
    QString szTagSelected;
    QStringList tagTextList;
    QString text;
    QString hAlign;
    QString vAlign;
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
       >> hAlign
       >> vAlign
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
    ele.setHAlignString(hAlign, ele.szHAlign_);
    ele.setVAlignString(vAlign, ele.szVAlign_);
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

