﻿#include "ElementPushButton.h"
#include "../../Public/PubTool.h"
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QDebug>

ElementPushButton::ElementPushButton()
{
    internalElementType = tr("PushButton");
    showContent_ = tr("文本");
    bShowContentText_ = true;
    szHAlign_ = QString("center");
    szVAlign_ = QString("center");
    font_ = QFont("宋体", 12);
    init();
    elementWidth = 100;
    elementHeight = 40;
    backgroundColor_ = QColor(240, 240, 240);
    signBackgroundColor = QColor(Qt::black);
    borderWidth = 4;
    borderColor = QColor(112, 112, 112);
    elementText = tr("弹出按钮");
    enableOnInitial_ = true;
    showOnInitial_ = true;
    isSelected_ = false;
}

QRectF ElementPushButton::boundingRect() const
{
    qreal extra = 6;
    QRectF rect(elementRect.toRect());
    return rect.normalized().adjusted(extra, extra, -extra, -extra);
}

QPainterPath ElementPushButton::shape() const
{
    QPainterPath path;
    path.addRect(elementRect);
    return path;
}

void ElementPushButton::setClickPosition(QPointF position)
{
    elementXPos = static_cast<int>(position.x());
    elementYPos = static_cast<int>(position.y());
    elementRect.setRect(0, 0, elementWidth, elementHeight);
}

void ElementPushButton::updateBoundingElement()
{
    elementRect.setRect(0, 0, elementWidth, elementHeight);
}

void ElementPushButton::paint(QPainter *painter)
{
    if(!showOnInitial_ || !bShow_) {
        return;
    }

    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    painter->translate(QPoint(elementXPos, elementYPos));
    painter->rotate(elemAngle);

    drawPushButton(painter);

    if (isSelected_) {
        painter->setPen(QPen(Qt::gray, 2, Qt::DashLine));
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(boundingRect());
    }
    painter->restore();
}

void ElementPushButton::drawPushButton(QPainter *painter)
{
    QRect rect(static_cast<int>(elementRect.x()),
               static_cast<int>(elementRect.y()),
               static_cast<int>(elementRect.width()),
               static_cast<int>(elementRect.height()));

    if(transparent_) {
        painter->setPen(QPen(Qt::gray, 1, Qt::DashLine));
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(rect);
    } else {
        for(int i=0; i<borderWidth; i++) {
            PubTool::DrawFrameRect(painter, rect, borderColor);
            if(i<borderWidth/2) rect.adjust(1, 1, -1, -1);
            else rect.adjust(1, 1, 0, 0);
        }

        PubTool::DrawFrameRect(painter, rect, QColor(252, 252, 252));
        rect.adjust(1, 1, -1, -1);
        rect.adjust(-1, -1, 0, 0);

        if(bShowContentText_) { // 文本+背景
            PubTool::FillFullRect(painter, rect, backgroundColor_);
            painter->setPen(textColor);
            painter->setBrush(Qt::NoBrush);
            painter->setFont(font_);

            int hFlags = Qt::AlignLeft;
            if(szHAlign_ == QString("left")) {
                hFlags = Qt::AlignLeft;
            } else if(szHAlign_ == QString("center")) {
                hFlags = Qt::AlignHCenter;
            } else if(szHAlign_ == QString("right")) {
                hFlags = Qt::AlignRight;
            }

            int vFlags = Qt::AlignVCenter;
            if(szVAlign_ == QString("top")) {
                vFlags = Qt::AlignTop;
            } else if(szVAlign_ == QString("center")) {
                vFlags = Qt::AlignVCenter;
            } else if(szVAlign_ == QString("bottom")) {
                vFlags = Qt::AlignBottom;
            }

            QRectF rect(elementRect.toRect());
            QRectF textRect = rect.normalized().adjusted(borderWidth, borderWidth, -borderWidth, -borderWidth);
            painter->drawText(textRect, hFlags|vFlags, elementText);

        } else { // 图片按钮
            if(filePicture_ != QString()) {
                QString picture = getProjectPath() + "/Pictures/" + filePicture_;
                if(QFile::exists(picture)) {
                    QImage image(getProjectPath() + "/Pictures/" + filePicture_);
                    QImage scaleImage = image.scaled(static_cast<int>(elementRect.width()),
                                                     static_cast<int>(elementRect.height()),
                                                     Qt::IgnoreAspectRatio);
                    painter->setRenderHints(QPainter::HighQualityAntialiasing | QPainter::TextAntialiasing);
                    painter->drawImage(elementRect, scaleImage);
                }
            }
        }
    }
}

void ElementPushButton::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
}

void ElementPushButton::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    if(!enableOnInitial_ || !bShow_ || !bEnable_) {
        return;
    }

    QPointF mousePoint = event->pos();
    if (mousePoint.x() <= (elementXPos + elementWidth) &&
            mousePoint.x() >= (elementXPos) &&
            mousePoint.y() <= (elementYPos + elementHeight) &&
            mousePoint.y() >= (elementYPos)) {
        isSelected_ = true;
        // 处理"按下事件"功能
        emit notifyExecScriptFunction(funcs_, tr("PressDown"));
        // 执行"按下事件"脚本
        emit notifyExecScriptText(script_, tr("PressDown"));
    } else {
        isSelected_ = false;
    }

    QWidget *pOwner = getOwnerWidget();
    if(pOwner != nullptr) {
        pOwner->update();
    }
}

void ElementPushButton::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    if(!enableOnInitial_ || !bShow_ || !bEnable_) {
        return;
    }

    QPointF mousePoint = event->pos();
    if (mousePoint.x() <= (elementXPos + elementWidth) &&
            mousePoint.x() >= (elementXPos) &&
            mousePoint.y() <= (elementYPos + elementHeight) &&
            mousePoint.y() >= (elementYPos)) {
        isSelected_ = false;
        // 处理"抬起事件"功能
        emit notifyExecScriptFunction(funcs_, tr("ReleaseUp"));
        // 执行"抬起事件"脚本
        emit notifyExecScriptText(script_, tr("ReleaseUp"));
    }

    QWidget *pOwner = getOwnerWidget();
    if(pOwner != nullptr) {
        pOwner->update();
    }
}


void ElementPushButton::readFromXml(const QXmlStreamAttributes &attributes)
{
    if (attributes.hasAttribute("elementId")) {
        QString szTagID = attributes.value("elementId").toString();
        setElementId(szTagID);
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

    if (attributes.hasAttribute("width")) {
        setElementWidth(attributes.value("width").toString().toInt());
    }

    if (attributes.hasAttribute("height")) {
        setElementHeight(attributes.value("height").toString().toInt());
    }

    if (attributes.hasAttribute("showContent")) {
        showContent_ = attributes.value("showContent").toString();
    }

    if (attributes.hasAttribute("picture")) {
        filePicture_ = attributes.value("picture").toString();
    }

    if (attributes.hasAttribute("elementtext")) {
        elementText = attributes.value("elementtext").toString();
    }

    if (attributes.hasAttribute("halign")) {
        QString align = attributes.value("halign").toString();
        this->szHAlign_ = align;
    }

    if (attributes.hasAttribute("valign")) {
        QString align = attributes.value("valign").toString();
        this->szVAlign_ = align;
    }

    if (attributes.hasAttribute("backgroundColor")) {
        backgroundColor_ = QColor(attributes.value("backgroundColor").toString());
    }

    if (attributes.hasAttribute("transparent")) {
        QString value = attributes.value("transparent").toString();
        transparent_ = false;
        if(value == "true") {
            transparent_ = true;
        }
    }

    if (attributes.hasAttribute("font")) {
        QString szFont = attributes.value("font").toString();
        font_.fromString(szFont);
    }

    if (attributes.hasAttribute("textcolor")) {
        textColor = QColor(attributes.value("textcolor").toString());
    }

    if (attributes.hasAttribute("fontsize")) {
        fontSize = attributes.value("fontsize").toString().toInt();
    }

    if (attributes.hasAttribute("elemAngle")) {
        setAngle(attributes.value("elemAngle").toString().toInt());
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

    if (attributes.hasAttribute("functions")) {
        QString listString = attributes.value("functions").toString();
        funcs_ = listString.split('|');
    }

    if (attributes.hasAttribute("script")) {
        script_ = attributes.value("script").toString();
    }

    updateBoundingElement();
}

void ElementPushButton::readData(QDataStream &in)
{
    QString id;
    qreal xpos;
    qreal ypos;
    qreal zvalue;
    int width;
    int height;
    QString showContent;
    QString pic;
    QString text;
    QString hAlign;
    QString vAlign;
    QColor backgroundColor;
    bool transparent;
    QString font;
    QColor textColor;
    int fontSize;
    qreal angle;
    bool enableOnInitial;
    bool showOnInitial;
    QStringList funcs;
    QString script;

    in >> id
       >> xpos
       >> ypos
       >> zvalue
       >> width
       >> height
       >> showContent
       >> pic
       >> text
       >> hAlign
       >> vAlign
       >> backgroundColor
       >> transparent
       >> font
       >> textColor
       >> fontSize
       >> angle
       >> enableOnInitial
       >> showOnInitial
       >> funcs
       >> script;

    this->setElementId(id);
    this->setElementXPos(static_cast<int>(xpos));
    this->setElementYPos(static_cast<int>(ypos));
    this->setElementZValue(static_cast<int>(zvalue));
    this->setElementWidth(width);
    this->setElementHeight(height);
    this->showContent_ = showContent;
    this->filePicture_ = pic;
    this->elementText = text;
    this->szHAlign_ = hAlign;
    this->szVAlign_ = vAlign;
    this->backgroundColor_ = backgroundColor;
    this->transparent_ = transparent;
    this->textColor = textColor;
    this->fontSize = fontSize;
    this->setAngle(angle);
    this->enableOnInitial_ = enableOnInitial;
    this->showOnInitial_ = showOnInitial;
    this->funcs_ = funcs;
    this->script_ = script;

    this->updateBoundingElement();
}


QDataStream &operator>>(QDataStream &in,ElementPushButton &ele)
{
    QString id;
    qreal xpos;
    qreal ypos;
    qreal zvalue;
    int width;
    int height;
    QString showContent;
    QString pic;
    QColor backgroundColor;
    bool transparent;
    QString text;
    QString hAlign;
    QString vAlign;
    QString font;
    QColor textColor;
    int fontSize;
    qreal angle;
    bool enableOnInitial;
    bool showOnInitial;
    QStringList funcs;
    QString script;

    in >> id
       >> xpos
       >> ypos
       >> zvalue
       >> width
       >> height
       >> showContent
       >> pic
       >> text
       >> hAlign
       >> vAlign
       >> backgroundColor
       >> transparent
       >> font
       >> textColor
       >> fontSize
       >> angle
       >> enableOnInitial
       >> showOnInitial
       >> funcs
       >> script;

    ele.setElementId(id);
    ele.setElementXPos(static_cast<int>(xpos));
    ele.setElementYPos(static_cast<int>(ypos));
    ele.setElementZValue(static_cast<int>(zvalue));
    ele.setElementWidth(width);
    ele.setElementHeight(height);
    ele.showContent_ = showContent;
    ele.filePicture_ = pic;
    ele.elementText = text;
    ele.szHAlign_ = hAlign;
    ele.szVAlign_ = vAlign;
    ele.backgroundColor_ = backgroundColor;
    ele.transparent_ = transparent;
    ele.font_ = font;
    ele.textColor = textColor;
    ele.fontSize = fontSize;
    ele.setAngle(angle);
    ele.enableOnInitial_ = enableOnInitial;
    ele.showOnInitial_ = showOnInitial;
    ele.funcs_ = funcs;
    ele.script_ = script;

    ele.updateBoundingElement();

    return in;
}

