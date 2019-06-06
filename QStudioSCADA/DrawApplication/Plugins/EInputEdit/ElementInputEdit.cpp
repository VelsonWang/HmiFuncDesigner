#include "ElementInputEdit.h"
#include "TagManager.h"
#include <QDebug>

int ElementInputEdit::iLastIndex_ = 1;

ElementInputEdit::ElementInputEdit(const QString &projPath) :
    Element(projPath) {
    elementId = QString(tr("InputEdit_%1").arg(iLastIndex_, 4, 10, QChar('0')));
    iLastIndex_++;
    internalElementType = trUtf8("InputEdit");
    elementIcon = QIcon(":/images/InputEdit.png");
    szHAlign_ = tr("左对齐");
    szVAlign_ = tr("居中对齐");
    backgroundColor_ = Qt::white;
    transparentBackground_ = false;
    borderWidth_ = 2;
    borderColor_ = QColor(61, 123, 173);
    enableEdit_ = true;
    enableOnInitial_ = true;
    inputPassword_ = false;
    showOnInitial_ = true;
    TagManager::setProjectPath(projPath);
    init();
    elementWidth = 80;
    elementHeight = 26;
    elementText = trUtf8("输入编辑框");
    createPropertyList();
    updatePropertyModel();
}

void ElementInputEdit::regenerateElementId() {
    elementId = QString(tr("InputEdit_%1").arg(iLastIndex_ - 1, 4, 10, QChar('0')));
    this->updatePropertyModel();
}

/**
 * @brief ElementInputEdit::release
 * @details 释放占用的资源
 */
void ElementInputEdit::release()
{

}

QRectF ElementInputEdit::boundingRect() const {
    qreal extra = 5;
    QRectF rect(elementRect.toRect());
    return rect.normalized().adjusted(-extra, -extra, extra, extra);
}

QPainterPath ElementInputEdit::shape() const {
    QPainterPath path;
    path.addRect(elementRect);

    if (isSelected()) {
        path.addRect(QRectF(elementRect.topLeft() - QPointF(3,3),elementRect.topLeft() + QPointF(3,3)));
        path.addRect(QRectF(elementRect.bottomRight() - QPointF(3,3),elementRect.bottomRight() + QPointF(3,3)));
    }

    return path;
}

void ElementInputEdit::createPropertyList() {
    // ID
    idProperty = new TextProperty(trUtf8("ID"));
    idProperty->setId(EL_ID);
    idProperty->setReadOnly(true);
    propList.insert(propList.end(),idProperty);

    // 标题
    titleProperty = new EmptyProperty(trUtf8("标题"));
    propList.insert(propList.end(),titleProperty);

    // 允许编辑输入
    enableEditProperty_ = new BoolProperty(tr("允许编辑输入"));
    enableEditProperty_->setId(EL_ENABLE_EDIT);
    enableEditProperty_->setTrueText(tr("是"));
    enableEditProperty_->setFalseText(tr("否"));
    enableEditProperty_->setValue(enableEdit_);
    propList.insert(propList.end(), enableEditProperty_);

    // 选择变量
    tagSelectProperty_ = new ListProperty(tr("选择变量"));
    tagSelectProperty_->setId(EL_TAG);
    QStringList varList;
    TagManager::getAllTagName(TagManager::getProjectPath(), varList);
    tagSelectProperty_->setList(varList);
    propList.insert(propList.end(), tagSelectProperty_);

    // X坐标
    xCoordProperty = new IntegerProperty(trUtf8("坐标 X"));
    xCoordProperty->setSettings(0,5000);
    xCoordProperty->setId(EL_X);
    propList.insert(propList.end(),xCoordProperty);

    // Y坐标
    yCoordProperty = new IntegerProperty(trUtf8("坐标 Y"));
    yCoordProperty->setId(EL_Y);
    yCoordProperty->setSettings(0,5000);
    propList.insert(propList.end(),yCoordProperty);

    // Z坐标
    zValueProperty = new IntegerProperty(trUtf8("Z 值"));
    zValueProperty->setId(EL_Z_VALUE);
    zValueProperty->setSettings(-1000,1000);
    propList.insert(propList.end(),zValueProperty);

    // 宽度
    widthProperty = new IntegerProperty(trUtf8("宽度"));
    widthProperty->setId(EL_WIDTH);
    widthProperty->setSettings(0,5000);
    propList.insert(propList.end(),widthProperty);

    // 高度
    heightProperty = new IntegerProperty(trUtf8("高度"));
    heightProperty->setId(EL_HEIGHT);
    heightProperty->setSettings(0,5000);
    propList.insert(propList.end(),heightProperty);

    // 文本
    elementTextProperty = new TextProperty(trUtf8("文本"));
    elementTextProperty->setId(EL_TEXT);
    propList.insert(propList.end(),elementTextProperty);

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
    textColorProperty = new ColorProperty(trUtf8("文本颜色"));
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

    // 密码输入
    inputPasswordProperty_ = new BoolProperty(tr("密码输入"));
    inputPasswordProperty_->setId(EL_INPUT_PASSWORD);
    inputPasswordProperty_->setTrueText(tr("是"));
    inputPasswordProperty_->setFalseText(tr("否"));
    inputPasswordProperty_->setValue(inputPassword_);
    propList.insert(propList.end(), inputPasswordProperty_);

    // 初始有效性
    enableOnInitialProperty_ = new BoolProperty(tr("初始有效性"));
    enableOnInitialProperty_->setId(EL_ENABLE_ON_INITIAL);
    enableOnInitialProperty_->setTrueText(tr("有效"));
    enableOnInitialProperty_->setFalseText(tr("失效"));
    enableOnInitialProperty_->setValue(enableOnInitial_);
    propList.insert(propList.end(), enableOnInitialProperty_);

    // 初始可见性
    showOnInitialProperty_ = new BoolProperty(tr("初始可见性"));
    showOnInitialProperty_->setId(EL_SHOW_ON_INITIAL);
    showOnInitialProperty_->setTrueText(tr("显示"));
    showOnInitialProperty_->setFalseText(tr("不显示"));
    showOnInitialProperty_->setValue(showOnInitial_);
    propList.insert(propList.end(), showOnInitialProperty_);

    // 旋转角度
    angleProperty = new IntegerProperty(trUtf8("角度"));
    angleProperty->setId(EL_ANGLE);
    angleProperty->setSettings(0, 360);
    propList.insert(propList.end(), angleProperty);
}

void ElementInputEdit::updateElementProperty(uint id, const QVariant &value) {
    switch (id) {
    case EL_ID:
        elementId = value.toString();
        break;
    case EL_ENABLE_EDIT:
        enableEdit_ = value.toBool();
        break;
    case EL_TAG:
        szTagSelected_ = value.toString();
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
    case EL_INPUT_PASSWORD:
        inputPassword_ = value.toBool();
        break;
    case EL_ENABLE_ON_INITIAL:
        enableOnInitial_ = value.toBool();
        break;
    case EL_SHOW_ON_INITIAL:
        showOnInitial_ = value.toBool();
        break;
    case EL_ANGLE:
        elemAngle = value.toInt();
        setAngle(elemAngle);
        break;
    }

    update();
    scene()->update();
}

void ElementInputEdit::updatePropertyModel() {
    idProperty->setValue(elementId);
    enableEditProperty_->setValue(enableEdit_);
    tagSelectProperty_->setValue(szTagSelected_);
    xCoordProperty->setValue(elementXPos);
    yCoordProperty->setValue(elementYPos);
    zValueProperty->setValue(elementZValue);
    widthProperty->setValue(elementWidth);
    heightProperty->setValue(elementHeight);
    elementTextProperty->setValue(elementText);
    hAlignProperty_->setValue(szHAlign_);
    vAlignProperty_->setValue(szVAlign_);
    backgroundColorProperty_->setValue(backgroundColor_);
    transparentBackgroundProperty_->setValue(transparentBackground_);
    fontProperty_->setValue(font_);
    textColorProperty->setValue(textColor);
    borderWidthProperty_->setValue(borderWidth_);
    borderColorProperty_->setValue(borderColor_);
    inputPasswordProperty_->setValue(inputPassword_);
    enableOnInitialProperty_->setValue(enableOnInitial_);
    showOnInitialProperty_->setValue(showOnInitial_);
    angleProperty->setValue(elemAngle);
}

void ElementInputEdit::setClickPosition(QPointF position) {
    prepareGeometryChange();
    elementXPos = position.x();
    elementYPos = position.y();
    setX(elementXPos);
    setY(elementYPos);
    elementRect.setRect(0, 0, elementWidth, elementHeight);
    updatePropertyModel();
}

void ElementInputEdit::updateBoundingElement() {
    elementRect.setRect(0, 0, elementWidth, elementHeight);
}

void ElementInputEdit::paint(QPainter *painter,
                             const QStyleOptionGraphicsItem *option,
                             QWidget *widget) {
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->setRenderHints(QPainter::HighQualityAntialiasing | QPainter::TextAntialiasing);

    drawInputEdit(painter);

    if (isSelected()) {
        painter->setPen(QPen(borderColor));
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(boundingRect());

        setCursor(Qt::SizeAllCursor);
        painter->setBrush(Qt::red);
        painter->setPen(Qt::red);
        painter->drawRect(QRectF(elementRect.topLeft() - QPointF(3,3),elementRect.topLeft() + QPointF(3,3)));
        painter->drawRect(QRectF(elementRect.bottomRight() - QPointF(3,3),elementRect.bottomRight() + QPointF(3,3)));
    }
}

void ElementInputEdit::drawInputEdit(QPainter *painter) {
    // 绘制边框
    if(borderWidth_ > 0) {
        painter->setPen(QPen(borderColor_, borderWidth_, Qt::SolidLine));
    } else {
        painter->setPen(QPen(borderColor_, 1, Qt::DashLine));
    }
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(elementRect);

    // 背景色不透明显示
    if(!transparentBackground_) {
        QRectF rect(elementRect.toRect());
        QRectF fillRect = rect.normalized().adjusted(borderWidth_, borderWidth_, -borderWidth_, -borderWidth_);
        QBrush brush(backgroundColor_);
        painter->fillRect(fillRect, brush);
    }

    // 绘制文本
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

    QRectF rect1(elementRect.toRect());
    QRectF textRect = rect1.normalized().adjusted(borderWidth_, borderWidth_, -borderWidth_, -borderWidth_);
    QString szDrawText = elementText;
    if(inputPassword_) {
        int iLen = elementText.length();
        szDrawText = "";
        for(int i=0; i<iLen; i++) {
            szDrawText.append(QChar('*'));
        }
    }
    painter->drawText(textRect, hFlags|vFlags, szDrawText);
}

void ElementInputEdit::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
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
        QPointF pos = scenePos();

        if(pos.x() < 0) {
            this->setX(0);
        }
        if(pos.x() > iGraphPageWidth_ - getElementWidth()) {
            this->setX(iGraphPageWidth_ - getElementWidth());
        }

        if(pos.y() < 0) {
            this->setY(0);
        }
        if(pos.y() > iGraphPageHeight_ - getElementHeight()) {
            this->setY(iGraphPageHeight_ - getElementHeight());
        }
    }
}

void ElementInputEdit::mousePressEvent(QGraphicsSceneMouseEvent *event) {
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

void ElementInputEdit::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
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

void ElementInputEdit::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
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

void ElementInputEdit::writeAsXml(QXmlStreamWriter &writer) {
    writer.writeStartElement("element");
    writer.writeAttribute("internalType",internalElementType);
    writer.writeAttribute("elementId",elementId);
    writer.writeAttribute("enableEdit", enableEdit_?"true":"false");
    writer.writeAttribute("tag", szTagSelected_);
    writer.writeAttribute("x",QString::number(x()));
    writer.writeAttribute("y",QString::number(y()));
    writer.writeAttribute("z",QString::number(zValue()));
    writer.writeAttribute("width",QString::number(elementWidth));
    writer.writeAttribute("height",QString::number(elementHeight));
    writer.writeAttribute("elementtext",elementText);
    writer.writeAttribute("halign", getHAlignString(szHAlign_));
    writer.writeAttribute("valign", getVAlignString(szVAlign_));
    writer.writeAttribute("backgroundColor", backgroundColor_.name());
    writer.writeAttribute("transparentBackground", transparentBackground_?"true":"false");
    writer.writeAttribute("textcolor", textColor.name());
    writer.writeAttribute("font", font_.toString());
    writer.writeAttribute("borderWidth", QString::number(borderWidth_));
    writer.writeAttribute("borderColor", borderColor_.name());
    writer.writeAttribute("inputPassword", inputPassword_?"true":"false");
    writer.writeAttribute("enableOnInitial", enableOnInitial_?"true":"false");
    writer.writeAttribute("showOnInitial", showOnInitial_?"true":"false");
    writer.writeAttribute("elemAngle", QString::number(elemAngle));
    writer.writeEndElement();
}

void ElementInputEdit::readFromXml(const QXmlStreamAttributes &attributes) {
    if (attributes.hasAttribute("elementId")) {
        QString szID = attributes.value("elementId").toString();
        setElementId(szID);
        int index = getIndexFromIDString(szID);
        if(iLastIndex_ < index) {
            iLastIndex_ = index;
        }
    }

    if (attributes.hasAttribute("enableEdit")) {
        QString value = attributes.value("enableEdit").toString();
        enableEdit_ = false;
        if(value == "true") {
            enableEdit_ = true;
        }
    }

    if (attributes.hasAttribute("tag")) {
        szTagSelected_ = attributes.value("tag").toString();
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

    if (attributes.hasAttribute("elementtext")) {
        elementText = attributes.value("elementtext").toString();
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

    if (attributes.hasAttribute("borderWidth")) {
        borderWidth_ = attributes.value("borderWidth").toInt();
    }

    if (attributes.hasAttribute("borderColor")) {
        borderColor_ = QColor(attributes.value("borderColor").toString());
    }

    if (attributes.hasAttribute("inputPassword")) {
        QString value = attributes.value("inputPassword").toString();
        inputPassword_ = false;
        if(value == "true") {
            inputPassword_ = true;
        }
    }

    if (attributes.hasAttribute("enableOnInitial")) {
        QString value = attributes.value("enableOnInitial").toString();
        enableOnInitial_ = false;
        if(value == "true") {
            enableOnInitial_ = true;
        }
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

void ElementInputEdit::writeData(QDataStream &out) {
    out << this->elementId
        << this->enableEdit_
        << this->szTagSelected_
        << this->x()
        << this->y()
        << this->zValue()
        << this->elementWidth
        << this->elementHeight
        << this->elementText
        << this->getHAlignString(szHAlign_)
        << this->getVAlignString(szVAlign_)
        << this->backgroundColor_
        << this->transparentBackground_
        << this->textColor
        << this->font_.toString()
        << this->borderWidth_
        << this->borderColor_
        << this->inputPassword_
        << this->enableOnInitial_
        << this->showOnInitial_
        << this->elemAngle;
}

void ElementInputEdit::readData(QDataStream &in) {
    QString id;
    bool enableEdit;
    QString szTagSelected;
    qreal xpos;
    qreal ypos;
    qreal zvalue;
    int width;
    int height;
    QString text;
    QString hAlign;
    QString vAlign;
    QColor backgroundColor;
    bool transparentBackground;
    QColor textColor;
    QString font;
    int borderWidth;
    QColor borderColor;
    bool inputPassword;
    bool enableOnInitial;
    bool showOnInitial;
    qreal angle;

    in >> id
       >> enableEdit
       >> szTagSelected
       >> xpos
       >> ypos
       >> zvalue
       >> width
       >> height
       >> text
       >> hAlign
       >> vAlign
       >> backgroundColor
       >> transparentBackground
       >> textColor
       >> font
       >> borderWidth
       >> borderColor
       >> inputPassword
       >> enableOnInitial
       >> showOnInitial
       >> angle;

    this->setElementId(id);
    int index = getIndexFromIDString(id);
    if(iLastIndex_ < index) {
        iLastIndex_ = index;
    }
    this->enableEdit_ = enableEdit;
    this->szTagSelected_ = szTagSelected;
    this->setElementXPos(xpos);
    this->setElementYPos(ypos);
    this->setElementZValue(zvalue);
    this->setElementWidth(width);
    this->setElementHeight(height);
    this->elementText = text;
    this->setHAlignString(hAlign, szHAlign_);
    this->setVAlignString(vAlign, szVAlign_);
    this->backgroundColor_ = backgroundColor;
    this->transparentBackground_ = transparentBackground;
    this->textColor = textColor;
    this->font_ = font;
    this->borderWidth_ = borderWidth;
    this->borderColor_ = borderColor;
    this->inputPassword_ = inputPassword;
    this->enableOnInitial_ = enableOnInitial;
    this->showOnInitial_ = showOnInitial;
    this->setAngle(angle);
    this->updateBoundingElement();
    this->updatePropertyModel();
}

QDataStream &operator<<(QDataStream &out, const ElementInputEdit &edit) {
    out << edit.elementId
        << edit.enableEdit_
        << edit.szTagSelected_
        << edit.x()
        << edit.y()
        << edit.zValue()
        << edit.elementWidth
        << edit.elementHeight
        << edit.elementText
        << edit.getHAlignString(edit.szHAlign_)
        << edit.getVAlignString(edit.szVAlign_)
        << edit.backgroundColor_
        << edit.transparentBackground_
        << edit.textColor
        << edit.font_.toString()
        << edit.borderWidth_
        << edit.borderColor_
        << edit.inputPassword_
        << edit.enableOnInitial_
        << edit.showOnInitial_
        << edit.elemAngle;
    return out;
}

QDataStream &operator>>(QDataStream &in, ElementInputEdit &edit) {
    QString id;
    bool enableEdit;
    QString szTagSelected;
    qreal xpos;
    qreal ypos;
    qreal zvalue;
    int width;
    int height;
    QString text;
    QString hAlign;
    QString vAlign;
    QColor backgroundColor;
    bool transparentBackground;
    QColor textColor;
    QString font;
    int borderWidth;
    QColor borderColor;
    bool inputPassword;
    bool enableOnInitial;
    bool showOnInitial;
    qreal angle;

    in >> id
       >> enableEdit
       >> szTagSelected
       >> xpos
       >> ypos
       >> zvalue
       >> width
       >> height
       >> text
       >> hAlign
       >> vAlign
       >> backgroundColor
       >> transparentBackground
       >> textColor
       >> font
       >> borderWidth
       >> borderColor
       >> inputPassword
       >> enableOnInitial
       >> showOnInitial
       >> angle;

    edit.setElementId(id);
    int index = edit.getIndexFromIDString(id);
    if(edit.iLastIndex_ < index) {
        edit.iLastIndex_ = index;
    }
    edit.enableEdit_ = enableEdit;
    edit.szTagSelected_ = szTagSelected;
    edit.setElementXPos(xpos);
    edit.setElementYPos(ypos);
    edit.setElementZValue(zvalue);
    edit.setElementWidth(width);
    edit.setElementHeight(height);
    edit.elementText = text;
    edit.setHAlignString(hAlign, edit.szHAlign_);
    edit.setVAlignString(vAlign, edit.szVAlign_);
    edit.backgroundColor_ = backgroundColor;
    edit.transparentBackground_ = transparentBackground;
    edit.textColor = textColor;
    edit.font_ = font;
    edit.borderWidth_ = borderWidth;
    edit.borderColor_ = borderColor;
    edit.inputPassword_ = inputPassword;
    edit.enableOnInitial_ = enableOnInitial;
    edit.showOnInitial_ = showOnInitial;
    edit.setAngle(angle);
    edit.updateBoundingElement();
    edit.updatePropertyModel();

    return in;
}

