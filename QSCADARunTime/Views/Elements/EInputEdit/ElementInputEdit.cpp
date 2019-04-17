#include "ElementInputEdit.h"
#include "InputMethodNumber.h"
#include "InputMethodAlphabet.h"
#include <QDebug>

ElementInputEdit::ElementInputEdit() {
    elementId = QString(tr("InputEdit"));
    internalElementType = trUtf8("InputEdit");
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
    elementWidth = 80;
    elementHeight = 26;
    elementText = trUtf8("输入编辑框");
    inputLineEdit_ = new InputLineEdit();
}

ElementInputEdit::~ElementInputEdit() {
    if(inputLineEdit_ != nullptr) {
        delete inputLineEdit_;
        inputLineEdit_ = nullptr;
    }
}

QRectF ElementInputEdit::boundingRect() const {
    qreal extra = 5;
    QRectF rect(elementRect.toRect());
    return rect.normalized().adjusted(-extra, -extra, extra, extra);
}

QPainterPath ElementInputEdit::shape() const {
    QPainterPath path;
    path.addRect(elementRect);
    return path;
}

void ElementInputEdit::setClickPosition(QPointF position) {
    prepareGeometryChange();
    elementXPos = position.x();
    elementYPos = position.y();
    setX(elementXPos);
    setY(elementYPos);
    elementRect.setRect(0, 0, elementWidth, elementHeight);
}

void ElementInputEdit::updateBoundingElement() {
    elementRect.setRect(0, 0, elementWidth, elementHeight);
}

void ElementInputEdit::paint(QPainter *painter,
                             const QStyleOptionGraphicsItem *option,
                             QWidget *widget) {
    Q_UNUSED(option)
    Q_UNUSED(widget)

    if(!showOnInitial_) {
        return;
    }

    painter->setRenderHints(QPainter::HighQualityAntialiasing | QPainter::TextAntialiasing);
    drawInputEdit(painter);
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
    Q_UNUSED(event)
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

    //inputLineEdit_->move(x(), x());
    //inputLineEdit_->resize(elementWidth, elementHeight);
    //inputLineEdit_->show();

    QGraphicsObject::mousePressEvent(event);
}

void ElementInputEdit::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
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
    }
    else if (mousePoint.x() <= (bottomRight.x() + mouseHandler.x()) &&
             mousePoint.x() >= (bottomRight.x() - mouseHandler.x()) &&
             mousePoint.y() <= (bottomRight.y() + mouseHandler.y()) &&
             mousePoint.y() >= (bottomRight.y() - mouseHandler.y())) {
        setCursor(Qt::SizeFDiagCursor);
    }

    QGraphicsObject::hoverEnterEvent(event);
}



void ElementInputEdit::readFromXml(const QXmlStreamAttributes &attributes) {
    if (attributes.hasAttribute("elementId")) {
        QString szID = attributes.value("elementId").toString();
        setElementId(szID);
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

    if(enableOnInitial_) {
        this->setEnabled(enableOnInitial_);
    }

    if(!showOnInitial_) {
        this->hide();
    }

    if(enableEdit_) {
        if(inputPassword_) {
            InputMethodAlphabet::instance()->init("control", "", font_.pixelSize(), font_.pixelSize());
        } else {
            InputMethodNumber::instance()->init("", font_.pixelSize());
        }
    }
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

    if(enableOnInitial_) {
        this->setEnabled(enableOnInitial_);
    }

    if(!showOnInitial_) {
        this->hide();
    }

    if(enableEdit_) {
        if(inputPassword_) {
            InputMethodAlphabet::instance()->init("control", "", font_.pixelSize(), font_.pixelSize());
        } else {
            InputMethodNumber::instance()->init("", font_.pixelSize());
        }
    }
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

    if(edit.enableOnInitial_) {
        edit.setEnabled(edit.enableOnInitial_);
    }

    if(!edit.showOnInitial_) {
        edit.hide();
    }

    if(edit.enableEdit_) {
        if(edit.inputPassword_) {
            InputMethodAlphabet::instance()->init("control", "", edit.font_.pixelSize(), edit.font_.pixelSize());
        } else {
            InputMethodNumber::instance()->init("", edit.font_.pixelSize());
        }
    }

    return in;
}

