#include "ElementMovingText.h"
#include "PubTool.h"
#include "RealTimeDB.h"
#include "SCADARunTime.h"
#include <QDateTime>
#include <QDate>
#include <QFontMetrics>
#include <QDebug>


ElementMovingText::ElementMovingText()
    : Element()
{
    elementId = tr("Clock");
    internalElementType = tr("Clock");
    init();
    iLastMSecs_ = QDateTime::currentMSecsSinceEpoch();
    iPos_ = 0;
    iOneCharWidth_ = 0;
}

QRectF ElementMovingText::boundingRect() const
{
    qreal extra = 5;
    QRectF rect(elementRect_.toRect());
    return rect.normalized().adjusted(-extra, -extra, extra, extra);
}

QPainterPath ElementMovingText::shape() const
{
    QPainterPath path;
    path.addRect(elementRect_);
    return path;
}

void ElementMovingText::setClickPosition(QPointF position)
{
    elementXPos = static_cast<int>(position.x());
    elementYPos = static_cast<int>(position.y());
    elementRect_.setRect(0, 0, elementWidth, elementHeight);
}

void ElementMovingText::updateBoundingElement()
{
    elementRect_.setRect(0, 0, elementWidth, elementHeight);
}

void ElementMovingText::paint(QPainter *painter)
{
    if(!showOnInitial_) {
        return;
    }

    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    painter->translate(QPoint(elementXPos, elementYPos));
    painter->rotate(elemAngle);

    // 背景色不透明显示
    if(!transparentBackground_) {
        QBrush brush(backgroundColor_);
        painter->fillRect(elementRect_, brush);
    }

    // 绘制移动文本
    drawMovingText(painter);

    // 绘制边框
    painter->setPen(QPen(borderColor_, borderWidth_));
    painter->setBrush(Qt::NoBrush);
    if(borderWidth_ > 0)
        painter->drawRect(elementRect_);

    painter->restore();
}


void ElementMovingText::drawMovingText(QPainter *painter)
{
    if(period_ < 0.5) {
        period_ = 0.5;
    }

    QString szText = "";
    QSize textSize;

    painter->setPen(textColor);
    painter->setBrush(Qt::NoBrush);
    painter->setFont(font_);

    QRectF rect(elementRect_.toRect());
    QRectF eleRect = rect.normalized().adjusted(borderWidth_, borderWidth_, -borderWidth_, -borderWidth_);
    int iEleRectWidth = static_cast<int>(eleRect.width());
    int iEleRectHeight = static_cast<int>(eleRect.height());

    if(szTagSelected_ == "") { // 未关联变量, 使用属性：移动文本
        szText = elementText;

    } else { // 已关联变量, 使用属性：变量文本列表

        QString szTagID = RealTimeDB::getIdByTagName(szTagSelected_);
        if (szTagID != "") {
            elementText = RealTimeDB::GetDataString(szTagID);
        } else {
            elementText = "#";
        }

    }

    QFontMetrics fontMetrics(font_);
    textSize = fontMetrics.size(Qt::TextSingleLine, elementText);

    if((QDateTime::currentMSecsSinceEpoch() - iLastMSecs_) > period_ * 1000) {
        iLastMSecs_ = QDateTime::currentMSecsSinceEpoch();

        textRect_.setTop((iEleRectHeight - textSize.height()) / 2);
        textRect_.setHeight(textSize.height());

        if(szMoveDir_ == "LeftToRight") { // 从左到右

        } else if(szMoveDir_ == "RightToLeft") { // 从右到左

            iOneCharWidth_ = textSize.width() / elementText.length();
            int iEleRectWidthAdjusted = 0;

            iPos_ -= (iOneCharWidth_ * iMoveCharNum_);
            if(iPos_ < 0) {
                int iMod = iEleRectWidth % (iOneCharWidth_ * iMoveCharNum_);
                iEleRectWidthAdjusted = (iMod > 0) ? (iEleRectWidth - iMod) : iEleRectWidth;
                iPos_ = iEleRectWidthAdjusted;
                szMovingText_ = szText;
            }

            textRect_.setLeft(iPos_);
            if(iPos_ <= iEleRectWidth && iPos_ > (iEleRectWidth - textSize.width())) {
                textRect_.setWidth(iEleRectWidth - iPos_);
            } else if(iPos_ <= (iEleRectWidth - textSize.width()) && iPos_ >= textSize.width()) {
                textRect_.setWidth(textSize.width());
            } else {
                qDebug() <<__FILE__ << __LINE__ <<__FUNCTION__
                        << " iPos_: " << iPos_
                        << " iOneCharWidth_ : " << iOneCharWidth_
                        << " iMoveCharNum_: " << iMoveCharNum_
                        << " iOneCharWidth_ * iMoveCharNum_: " << iOneCharWidth_ * iMoveCharNum_;
                if(iPos_ >= iOneCharWidth_ * iMoveCharNum_) {
                    textRect_.setLeft(iPos_);
                    textRect_.setWidth(textSize.width());
                } else {
                    if(iPos_ <= iOneCharWidth_ / 2) {
                        textRect_.setLeft(0);
                        if(szMovingText_.length() == szText.length()) {
                            textRect_.setLeft(iPos_);
                        } else {
                            szMovingText_ = szMovingText_.remove(0, iMoveCharNum_);
                        }
                    }
                    textRect_.setWidth(szMovingText_.length() * iOneCharWidth_);
                    if(szMovingText_.length() > 0) {
                        iPos_ = iOneCharWidth_ * iMoveCharNum_ + iOneCharWidth_ / 2;
                    }
                }

            }




/*
            int iSub = iEleRectWidth - iPos_;
            if(iSub < 0) {
                iPos_ = 0;
                textRect_.setLeft(iEleRectWidth);
                textRect_.setWidth(0);
            } else {
                if(iSub > textSize.width()) {
                    textRect_.setLeft(iEleRectWidth - textSize.width() - iPos_);
                    textRect_.setWidth(textSize.width());
                } else {
                    if(iPos_ > textSize.width()) {

                    } else {

                    }

                    szMovingText_ = szMovingText_.remove(0, iMoveCharNum_);
                    textRect_.setLeft(0);
                    textRect_.setWidth(szMovingText_.length() * iOneCharWidth_);
                }
            }
            */
        }

        if((szText.length() - szMovingText_.length()) == szText.length() ||
                szText.length() == szMovingText_.length()) {
            szMovingText_ = szText;
        }
    }

    painter->fillRect(textRect_, Qt::yellow);
    painter->drawText(textRect_, Qt::AlignLeft, szMovingText_);
}


void ElementMovingText::mouseMoveEvent(QMouseEvent *event) {
    Q_UNUSED(event)
}

void ElementMovingText::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
}

void ElementMovingText::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
}


void ElementMovingText::readFromXml(const QXmlStreamAttributes &attributes)
{
    if (attributes.hasAttribute("elementId")) {
        QString szID = attributes.value("elementId").toString();
        setElementId(szID);
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
        szMovingText_ = elementText;
    }

    if (attributes.hasAttribute("moveDir")) {
        szMoveDir_ = attributes.value("moveDir").toString();
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
        setElementZValue(attributes.value("z").toString().toInt());
    }

    if (attributes.hasAttribute("width")) {
        setElementWidth(attributes.value("width").toString().toInt());
    }

    if (attributes.hasAttribute("height")) {
        setElementHeight(attributes.value("height").toString().toInt());
    }

    updateBoundingElement();
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
    this->szTagSelected_ = szTagSelected;
    this->tagTextList_ = tagTextList;
    this->elementText = text;
    szMovingText_ = elementText;
    this->szMoveDir_ = szMoveDir;
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
    ele.szTagSelected_ = szTagSelected;
    ele.tagTextList_ = tagTextList;
    ele.elementText = text;
    ele.szMovingText_ = ele.elementText;
    ele.szMoveDir_ = szMoveDir;
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

    return in;
}


