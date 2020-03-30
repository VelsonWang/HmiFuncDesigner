#include "ElementMovingText.h"
#include "../../Public/PubTool.h"
#include "../../Public/RealTimeDB.h"
#include <QDateTime>
#include <QDate>
#include <QFontMetrics>
#include <QDebug>


ElementMovingText::ElementMovingText()
    : Element()
{
    elementId = tr("MovingText");
    internalElementType = tr("MovingText");
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


    QRectF rect(elementRect_.toRect());
    QRectF eleRect = rect.normalized().adjusted(borderWidth_, borderWidth_, -borderWidth_, -borderWidth_);
    int iEleRectWidth = static_cast<int>(eleRect.width());
    int iEleRectHeight = static_cast<int>(eleRect.height());

    QColor getBackgroundColor;
    QColor getTextColor;

    // 未关联变量, 使用属性：移动文本
    szText = elementText;
    getBackgroundColor = backgroundColor_;
    getTextColor = textColor;

    // 已关联变量, 使用属性：变量文本列表
    if(szTagSelected_ != "") {
        QString szTagValue = "#";
        QString szTagID = pRtdbObj_->getIdByTagName(szTagSelected_);
        if (szTagID != "") {
            szTagValue = pRtdbObj_->GetDataString(szTagID);
        } else {
            szTagValue = "#";
        }

        for(int i=0; i<tagTextList_.size(); i++) {
            QString szValueColor = tagTextList_.at(i);
            QStringList listValueColor = szValueColor.split(":");
            if(listValueColor.size() == 4) {
                if(szTagValue != "#") {
                    if(szTagValue == listValueColor.at(0)) {
                        szText = listValueColor.at(1);
                        getBackgroundColor = QColor(listValueColor.at(2));
                        getTextColor = QColor(listValueColor.at(3));
                    }
                }
            }
        }
    }

    // 背景色不透明显示
    if(!transparentBackground_) {
        QBrush brush(getBackgroundColor);
        painter->fillRect(elementRect_, brush);
    }

    painter->setPen(getTextColor);
    painter->setBrush(Qt::NoBrush);
    painter->setFont(font_);

    QFontMetrics fontMetrics(font_);
    textSize = fontMetrics.size(Qt::TextSingleLine, elementText);

    if((QDateTime::currentMSecsSinceEpoch() - iLastMSecs_) > period_ * 1000) {
        iLastMSecs_ = QDateTime::currentMSecsSinceEpoch();

        textRect_.setTop((iEleRectHeight - textSize.height()) / 2);
        textRect_.setHeight(textSize.height());

        if(szText.indexOf(szMovingText_) == -1) {
            szMovingText_ = szText;
        }

        if(szMoveDir_ == "LeftToRight") { // 从左到右
            iOneCharWidth_ = textSize.width() / elementText.length();
            iPos_ += (iOneCharWidth_ * iMoveCharNum_);

            if(iPos_ >= (0 - iOneCharWidth_ * szText.length()) && iPos_ < textSize.width()) {
                if(iPos_ < 0) {
                    textRect_.setLeft(0);
                } else {
                    textRect_.setLeft(iPos_);
                }
                QString szInsert = szText.mid(szText.length() - szMovingText_.length() - iMoveCharNum_, iMoveCharNum_);
                szMovingText_.insert(0, szInsert);
                textRect_.setWidth(szMovingText_.length() * iOneCharWidth_);
            } else if(iPos_ >= textSize.width() && iPos_ <= (iEleRectWidth - textSize.width())) {
                textRect_.setLeft(iPos_);
                textRect_.setWidth(textSize.width());
            } else if(iPos_ > (iEleRectWidth - textSize.width()) && iPos_ <= iEleRectWidth) {
                textRect_.setLeft(iPos_);
                szMovingText_ = szMovingText_.remove(szMovingText_.length() - iMoveCharNum_, iMoveCharNum_);
                textRect_.setWidth(szMovingText_.length() * iOneCharWidth_);
            } else {
                iPos_ = 0 - iOneCharWidth_ * szText.length();
                szMovingText_ = "";
                textRect_.setWidth(0);
            }
        } else if(szMoveDir_ == "RightToLeft") { // 从右到左
            iOneCharWidth_ = textSize.width() / elementText.length();
            int iEleRectWidthAdjusted = 0;

            iPos_ -= (iOneCharWidth_ * iMoveCharNum_);

            if(iPos_ <= iEleRectWidth && iPos_ > (iEleRectWidth - textSize.width())) {
                textRect_.setLeft(iPos_);
                textRect_.setWidth(iEleRectWidth - iPos_);
            } else if(iPos_ <= (iEleRectWidth - textSize.width()) && iPos_ >= 0) {
                textRect_.setLeft(iPos_);
                textRect_.setWidth(textSize.width());
            } else if(iPos_ < 0 && iPos_ > -textSize.width()) {
                szMovingText_ = szMovingText_.remove(0, iMoveCharNum_);
                textRect_.setWidth(szMovingText_.length() * iOneCharWidth_);
            } else {
                int iMod = iEleRectWidth % (iOneCharWidth_ * iMoveCharNum_);
                iEleRectWidthAdjusted = (iMod > 0) ? (iEleRectWidth - iMod) : iEleRectWidth;
                iPos_ = iEleRectWidthAdjusted + iOneCharWidth_ * iMoveCharNum_;
                szMovingText_ = szText;
                textRect_.setWidth(0);
            }
        }
    }

    painter->drawText(textRect_, Qt::AlignLeft, szMovingText_);
}


void ElementMovingText::mouseMoveEvent(QMouseEvent *event)
{
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
        if(szMoveDir_ == "LeftToRight") { // 从左到右
            iPos_ = -32767;
        } else if(szMoveDir_ == "RightToLeft") { // 从右到左
            iPos_ = -32767;
        }
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
    if(szMoveDir_ == "LeftToRight") { // 从左到右
        iPos_ = -32767;
    } else if(szMoveDir_ == "RightToLeft") { // 从右到左
        iPos_ = -32767;
    }
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
    if(ele.szMoveDir_ == "LeftToRight") { // 从左到右
        ele.iPos_ = -32767;
    } else if(ele.szMoveDir_ == "RightToLeft") { // 从右到左
        ele.iPos_ = -32767;
    }
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


