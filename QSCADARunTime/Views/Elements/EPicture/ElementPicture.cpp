#include "elementPicture.h"
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QDebug>

ElementPicture::ElementPicture() {
    elementId = trUtf8("图片");
    internalElementType = trUtf8("Picture");
    filePicture_ = QString();
    showNoScale_ = false;
    borderWidth_ = 1;
    borderColor_ = Qt::black;
    showOnInitial_ = true;
    init();
}

QRectF ElementPicture::boundingRect() const {
    qreal extra = 5;
    QRectF rect(elementRect.toRect());
    return rect.normalized().adjusted(-extra, -extra, extra, extra);
}

QPainterPath ElementPicture::shape() const {
    QPainterPath path;
    path.addRect(elementRect);
    return path;
}


void ElementPicture::setClickPosition(QPointF position) {
    elementXPos = position.x();
    elementYPos = position.y();
    elementRect.setRect(0, 0, elementWidth, elementHeight);
}

void ElementPicture::updateBoundingElement() {
    elementRect.setRect(0, 0, elementWidth, elementHeight);
}

void ElementPicture::paint(QPainter *painter) {
    if(!showOnInitial_) {
        return;
    }

    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    painter->translate(QPoint(elementXPos, elementYPos));
    painter->rotate(elemAngle);

    if(filePicture_ != QString()) {
        QString picture = getProjectPath() + "/Pictures/" + filePicture_;
        if(QFile::exists(picture)) {
            QImage image(getProjectPath() + "/Pictures/" + filePicture_);
            QImage scaleImage;
            if(showNoScale_) {
                scaleImage = image;
            } else {
                scaleImage = image.scaled((int)elementRect.width(), (int)elementRect.height(), Qt::IgnoreAspectRatio);
            }
            painter->drawImage(elementRect, scaleImage);
        }
    }

    if(borderWidth_ < 1) {
        borderColor = Qt::gray;
        painter->setPen(QPen(borderColor, 1, Qt::DotLine));
    } else {
        painter->setPen(QPen(borderColor_, borderWidth_, Qt::SolidLine));
    }
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(elementRect);
    painter->restore();
}

void ElementPicture::mouseMoveEvent(QMouseEvent *event) {
    Q_UNUSED(event)
}

void ElementPicture::mousePressEvent(QMouseEvent *event) {
    Q_UNUSED(event)
}

void ElementPicture::mouseReleaseEvent(QMouseEvent *event) {
    Q_UNUSED(event)
}

void ElementPicture::readFromXml(const QXmlStreamAttributes &attributes) {
    if (attributes.hasAttribute("elementId")) {
        setElementId(attributes.value("elementId").toString());
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

    if (attributes.hasAttribute("picture")) {
        filePicture_ = attributes.value("picture").toString();
    }

    if (attributes.hasAttribute("showNoScale")) {
        QString value = attributes.value("showNoScale").toString();
        showNoScale_ = false;
        if(value == "true") {
            showNoScale_ = true;
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
}

void ElementPicture::readData(QDataStream &in) {
    QString id;
    qreal xpos;
    qreal ypos;
    qreal zvalue;
    int width;
    int height;
    QString pic;
    bool showNoScale;
    int borderWidth;
    bool showOnInitial;
    qreal angle;

    in >> id
       >> xpos
       >> ypos
       >> zvalue
       >> width
       >> height
       >> pic
       >> showNoScale
       >> borderWidth
       >> borderColor
       >> showOnInitial
       >> angle;

    this->setElementId(id);
    this->setElementXPos(xpos);
    this->setElementYPos(ypos);
    this->setElementZValue(zvalue);
    this->setElementWidth(width);
    this->setElementHeight(height);
    this->filePicture_ = pic;
    this->showNoScale_ = showNoScale;
    this->borderWidth_ = borderWidth;
    this->borderColor_ = borderColor;
    this->showOnInitial_ = showOnInitial;
    this->setAngle(angle);
    this->updateBoundingElement();
}


QDataStream &operator>>(QDataStream &in,ElementPicture &picture) {
    QString id;
    qreal xpos;
    qreal ypos;
    qreal zvalue;
    int width;
    int height;
    QString pic;
    bool showNoScale;
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
       >> pic
       >> showNoScale
       >> borderWidth
       >> borderColor
       >> showOnInitial
       >> angle;

    picture.setElementId(id);
    picture.setElementXPos(xpos);
    picture.setElementYPos(ypos);
    picture.setElementZValue(zvalue);
    picture.setElementWidth(width);
    picture.setElementHeight(height);
    picture.filePicture_ = pic;
    picture.showNoScale_ = showNoScale;
    picture.borderWidth_ = borderWidth;
    picture.borderColor_ = borderColor;
    picture.showOnInitial_ = showOnInitial;
    picture.setAngle(angle);
    picture.updateBoundingElement();

    return in;
}
