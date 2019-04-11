#include "elementtext.h"
#include "PubTool.h"
#include <QDebug>

ElementText::ElementText() {
    elementId = trUtf8("Text");
    internalElementType = trUtf8("Text");
    init();
}

QRectF ElementText::boundingRect() const {
    qreal extra = 5;
    QRectF rect(elementRect.toRect());
    return rect.normalized().adjusted(-extra,-extra,extra,extra);
}

QPainterPath ElementText::shape() const {
    QPainterPath path;
    path.addRect(elementRect);
    return path;
}

void ElementText::setClickPosition(QPointF position) {
    prepareGeometryChange();
    elementXPos = position.x();
    elementYPos = position.y();
    setX(elementXPos);
    setY(elementYPos);
    elementRect.setRect(0, 0, elementWidth, elementHeight);
}

void ElementText::updateBoundingElement() {
    elementRect.setRect(0, 0, elementWidth, elementHeight);
}

void ElementText::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option)
    Q_UNUSED(widget)

    if(!showOnInitial_) {
        return;
    }

    painter->setRenderHints(QPainter::HighQualityAntialiasing | QPainter::TextAntialiasing);

    // 背景色不透明显示
    if(!transparentBackground_) {
        QBrush brush(backgroundColor_);
        painter->fillRect(elementRect, brush);
    }

    // 绘制文本
    drawText(painter);

    // 绘制边框
    if(borderWidth_ > 0) {
        QRect rect(elementRect.x(), elementRect.y(), elementRect.width(), elementRect.height());
        for(int i=0; i<borderWidth_; i++) {
            PubTool::DrawFrameRect(painter, rect, borderColor_);
            rect.adjust(1, 1, -1, -1);
        }
    }
}

void ElementText::drawText(QPainter *painter) {
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

    QRectF rect(elementRect.toRect());
    QRectF textRect = rect.normalized().adjusted(borderWidth_, borderWidth_, -borderWidth_, -borderWidth_);

    painter->drawText(textRect, hFlags|vFlags, elementText);
}

void ElementText::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    Q_UNUSED(event)
}

void ElementText::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if(hideOnClick_) {
        this->hide();
    }

    QGraphicsObject::mousePressEvent(event);
}

void ElementText::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {

    QGraphicsObject::mouseReleaseEvent(event);
}

void ElementText::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {

    QGraphicsObject::hoverEnterEvent(event);
}


void ElementText::readFromXml(const QXmlStreamAttributes &attributes) {
    if (attributes.hasAttribute("elementId")) {
        QString szID = attributes.value("elementId").toString();
        setElementId(szID);
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
        this->setHAlignString(align);
    }

    if (attributes.hasAttribute("valign")) {
        QString align = attributes.value("valign").toString();
        this->setVAlignString(align);
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

    if (attributes.hasAttribute("hideOnClick")) {
        QString value = attributes.value("hideOnClick").toString();
        hideOnClick_ = false;
        if(value == "true") {
            hideOnClick_ = true;
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

    if(!showOnInitial_) {
        this->hide();
    }
}


void ElementText::readData(QDataStream &in) {
    QString id;
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
    bool hideOnClick;
    bool showOnInitial;
    qreal angle;

    in >> id
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
       >> hideOnClick
       >> showOnInitial
       >> angle;

    this->setElementId(id);
    this->setElementXPos(xpos);
    this->setElementYPos(ypos);
    this->setElementZValue(zvalue);
    this->setElementWidth(width);
    this->setElementHeight(height);
    this->elementText = text;
    this->setHAlignString(hAlign);
    this->setVAlignString(vAlign);
    this->backgroundColor_ = backgroundColor;
    this->transparentBackground_ = transparentBackground;
    this->textColor = textColor;
    this->font_ = font;
    this->borderWidth_ = borderWidth;
    this->borderColor_ = borderColor;
    this->hideOnClick_ = hideOnClick;
    this->showOnInitial_ = showOnInitial;
    this->setAngle(angle);
    this->updateBoundingElement();

    if(!showOnInitial_) {
        this->hide();
    }
}


/**
 * @brief ElementText::getHAlignString
 * @return 水平方向对齐方式
 */
QString ElementText::getHAlignString() const {
    if(szHAlign_ == tr("左对齐")) {
        return QString("left");
    } else if(szHAlign_ == tr("居中对齐")) {
        return QString("center");
    } else if(szHAlign_ == tr("右对齐")) {
        return QString("right");
    }
    return QString("");
}


/**
 * @brief ElementText::setHAlignString
 * @details 设置水平方向对齐方式
 * @param szAlign 水平方向对齐方式
 */
void ElementText::setHAlignString(const QString& szAlign) {
    if(szAlign == QString("left")) {
        szHAlign_ = tr("左对齐");
    } else if(szAlign == QString("center")) {
        szHAlign_ = tr("居中对齐");
    } else if(szAlign == QString("right")) {
        szHAlign_ = tr("右对齐");
    }
}


/**
 * @brief ElementText::getVAlignString
 * @return 垂直方向对齐方式
 */
QString ElementText::getVAlignString() const {
    if(szVAlign_ == tr("上对齐")) {
        return QString("top");
    } else if(szVAlign_ == tr("居中对齐")) {
        return QString("center");
    } else if(szVAlign_ == tr("下对齐")) {
        return QString("bottom");
    }
    return QString("");
}

/**
 * @brief ElementText::setVAlignString
 * @details 设置垂直方向对齐方式
 * @param szAlign 垂直方向对齐方式
 */
void ElementText::setVAlignString(const QString& szAlign) {
    if(szAlign == QString("top")) {
        szVAlign_ = tr("上对齐");
    } else if(szAlign == QString("center")) {
        szVAlign_ = tr("居中对齐");
    } else if(szAlign == QString("bottom")) {
        szVAlign_ = tr("下对齐");
    }
}


QDataStream &operator>>(QDataStream &in,ElementText &rect) {
    QString id;
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
    bool hideOnClick;
    bool showOnInitial;
    qreal angle;

    in >> id
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
       >> hideOnClick
       >> showOnInitial
       >> angle;

    rect.setElementId(id);
    rect.setElementXPos(xpos);
    rect.setElementYPos(ypos);
    rect.setElementZValue(zvalue);
    rect.setElementWidth(width);
    rect.setElementHeight(height);
    rect.elementText = text;
    rect.setHAlignString(hAlign);
    rect.setVAlignString(vAlign);
    rect.backgroundColor_ = backgroundColor;
    rect.transparentBackground_ = transparentBackground;
    rect.textColor = textColor;
    rect.font_ = font;
    rect.borderWidth_ = borderWidth;
    rect.borderColor_ = borderColor;
    rect.hideOnClick_ = hideOnClick;
    rect.showOnInitial_ = showOnInitial;
    rect.setAngle(angle);
    rect.updateBoundingElement();

    if(!rect.showOnInitial_) {
        rect.hide();
    }
    return in;
}

