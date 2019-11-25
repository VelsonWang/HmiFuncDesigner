#include "ElementComboBox.h"
#include "PubTool.h"
#include "RealTimeDB.h"
#include "HmiRunTime.h"
#include <QDateTime>
#include <QDate>
#include <QFontMetrics>
#include <QDebug>


ElementComboBox::ElementComboBox()
    : Element()
{
    elementId = tr("ComboBox");
    internalElementType = tr("ComboBox");
    init();
}

QRectF ElementComboBox::boundingRect() const
{
    qreal extra = 5;
    QRectF rect(elementRect_.toRect());
    return rect.normalized().adjusted(-extra, -extra, extra, extra);
}

QPainterPath ElementComboBox::shape() const
{
    QPainterPath path;
    path.addRect(elementRect_);
    return path;
}

void ElementComboBox::setClickPosition(QPointF position)
{
    elementXPos = static_cast<int>(position.x());
    elementYPos = static_cast<int>(position.y());
    elementRect_.setRect(0, 0, elementWidth, elementHeight);
}

void ElementComboBox::updateBoundingElement()
{
    elementRect_.setRect(0, 0, elementWidth, elementHeight);
}

void ElementComboBox::paint(QPainter *painter)
{
    if(!showOnInitial_) {
        return;
    }

    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    painter->translate(QPoint(elementXPos, elementYPos));
    painter->rotate(elemAngle);
    // 绘制变量文本
    drawComboBox(painter);

    // 绘制边框
    painter->setPen(QPen(borderColor_, borderWidth_));
    painter->setBrush(Qt::NoBrush);
    if(borderWidth_ > 0)
        painter->drawRect(elementRect_);

    painter->restore();
}


void ElementComboBox::drawComboBox(QPainter *painter)
{
    //QString szText = "";
    QColor getBackgroundColor;
    QColor getTextColor;

    // 未关联变量, 使用属性：移动文本
    //szText = elementText;
    getBackgroundColor = backgroundColor_;
    getTextColor = textColor;
    //int maxPage=5;//页最大数
    //int iPage=4;//当前页
    // 已关联变量, 使用属性：下拉菜单列表


    // 背景色不透明显示
    if(!transparentBackground_) {
        QBrush brush(getBackgroundColor);
        painter->fillRect(elementRect_, brush);
    }

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
    QRectF rect(elementRect_.toRect());
    if(szTagSelected_ != "") {
        QString szTagValue = "#";
        QString szTagID = RealTimeDB::getIdByTagName(szTagSelected_);
        if (szTagID != "") {
            szTagValue = RealTimeDB::GetDataString(szTagID);
        } else {
            szTagValue = "#";
        }

        for(int i=0; i<comboBox_.size(); i++) {
            QString szValueColor = comboBox_.at(i);
            QStringList listValueColor = szValueColor.split(":");
            if(listValueColor.size() == 4) {
                if(szTagValue != "#") {
                    getBackgroundColor = QColor(listValueColor.at(2));
                    getTextColor = QColor(listValueColor.at(3));
                    painter->setFont(font_);
                    if(szTagValue == listValueColor.at(0)) { // 绘制本栏目
                        //szText = listValueColor.at(1);

                        painter->setPen(getTextColor);
                        painter->setBrush(Qt::NoBrush);

                        QRectF textRect = rect.normalized().adjusted(borderWidth_, borderWidth_, -borderWidth_, -borderWidth_);
                        painter->drawText(textRect, hFlags|vFlags, listValueColor.at(1));
                        if(!isSelected_)break;
                    }
                    if (isSelected_&&i<iPage_*maxPage_&&i<(iPage_+1)*maxPage_) { // 绘制下拉栏目
                        QRectF textRect = rect.normalized().adjusted(0, rect.height()*((i%maxPage_)+1), 0, rect.height()*((i%maxPage_)+1));

                        // 绘制边框
                        painter->setPen(QPen(borderColor_, borderWidth_));
                        painter->setBrush(Qt::NoBrush);
                        painter->drawRect(textRect);
                        QBrush brush(getBackgroundColor);
                        if(i%maxPage_==0)
                        for(int j=0; j<maxPage_; j++) {//先清除页
                           QRectF textRectClear = rect.normalized().adjusted(0, rect.height()*((j%maxPage_)+1), 0, rect.height()*((j%maxPage_)+1));
                           painter->fillRect(textRectClear, brush);
                        }
                        // 背景色不透明显示
                        if(!transparentBackground_) {
                            painter->fillRect(textRect, brush);
                        }
                        painter->drawText(textRect, hFlags|vFlags, listValueColor.at(1));
                    }
                }
            }
        }
    }

    //开始绘制箭头
    int harrowWidth = elementRect_.width()/3;
    harrowWidth = harrowWidth>150?150:harrowWidth;
    harrowWidth = harrowWidth<50?50:harrowWidth;
    painter->setBrush(Qt::gray);
    painter->setPen(QPen(Qt::black, 2));

    int tmpPage = 1 + (comboBox_.size() > maxPage_ ? maxPage_ : comboBox_.size());
    if (isSelected_) {   painter->drawRect(QRectF(elementRect_.topRight().x()-harrowWidth,elementRect_.topRight().y(),harrowWidth,elementRect_.height()*tmpPage));
        painter->drawText(QRectF(elementRect_.topRight().x()-harrowWidth/3-font_.pointSize()*1.5,elementRect_.topRight().y(),harrowWidth,elementRect_.height()),"▲");
        painter->drawText(QRectF(elementRect_.topRight().x()-harrowWidth/3-font_.pointSize()*1.5,elementRect_.bottomRight().y()-font_.pointSize()*1.5+elementRect_.height()*(tmpPage-1),harrowWidth,elementRect_.height()*tmpPage),"▼");
    } else {
        painter->drawRect(QRectF(elementRect_.topRight().x()-harrowWidth,elementRect_.topRight().y(),harrowWidth,elementRect_.height()));
        painter->drawText(QRectF(elementRect_.topRight().x()-harrowWidth/3-font_.pointSize()*1.5,elementRect_.bottomRight().y()-font_.pointSize()-elementRect_.height()/2,harrowWidth,elementRect_.height()),"▼");
    }
}


void ElementComboBox::mouseMoveEvent(QMouseEvent *event) {
    Q_UNUSED(event)
}

void ElementComboBox::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    if( !bShow_ || !bEnable_) {
        return;
    }
    int isSelectedPos=elementHeight+(isSelected_==true?elementRect_.height()*(maxPage_):0);
    QPointF mousePoint = event->pos();
    if (mousePoint.x() <= (elementXPos + elementWidth) && mousePoint.x() >= (elementXPos) &&
        mousePoint.y() <= (elementYPos + isSelectedPos) && mousePoint.y() >= (elementYPos)) {
        isSelected_ = true;
        int harrowWidth=elementRect_.width()/3;
        harrowWidth=harrowWidth>150?150:harrowWidth;
        harrowWidth=harrowWidth<50?50:harrowWidth;
        if (mousePoint.x() <= (elementXPos+elementWidth) && mousePoint.x() >= (elementXPos+elementWidth-harrowWidth) &&
            mousePoint.y() <= (elementYPos+elementHeight) && mousePoint.y() >= (elementYPos)) { // 处理"按下▲功能
            if(iPage_<=1)iPage_=1;
            else iPage_--;
        } else if (mousePoint.x() <= (elementXPos+elementWidth) && mousePoint.x() >= (elementXPos+elementWidth-harrowWidth) &&
                  mousePoint.y() <= (elementYPos+isSelectedPos) && mousePoint.y()>= (elementYPos+isSelectedPos-elementHeight)) { // 处理"按下▼功能
            if(iPage_*maxPage_ > comboBox_.size()) iPage_ = comboBox_.size()/maxPage_+1;
            else iPage_++;
        }
        if (mousePoint.x() <= (elementXPos+elementWidth-harrowWidth) && mousePoint.x() >= (elementXPos) &&
            mousePoint.y() <= (elementYPos+isSelectedPos) && mousePoint.y() >= (elementYPos+elementHeight)) { // 下拉菜单选项选值处理功能
            QString szTagID = "";
            int pressID = (mousePoint.y()-(elementYPos+elementHeight))/elementHeight;
            pressID = pressID + (iPage_-1) * maxPage_;
            if (szTagSelected_ != "") {
                szTagID = RealTimeDB::getIdByTagName(szTagSelected_);
                if(pressID < comboBox_.size()) {
                    QString szValueColor = comboBox_.at(pressID);
                    QStringList listValueColor = szValueColor.split(":");
                    if(listValueColor.size() > 1) {
                        QString pressString = listValueColor.at(0);
                        if (szTagID != -1) {
                            RealTimeDB::SetDataString(szTagID, pressString);
                        }
                    }
                }
            }
        }
    } else {
        isSelected_ = false;
    }

    QWidget *pOwner = getOwnerWidget();
    if(pOwner != nullptr) {
        pOwner->update();
    }
}

void ElementComboBox::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    if(!bShow_ || !bEnable_) {
        return;
    }
    int isSelectedPos=elementHeight+(isSelected_==true?elementRect_.height()*(maxPage_):0);
    QPointF mousePoint = event->pos();
    if (mousePoint.x() <= (elementXPos + elementWidth) &&mousePoint.x() >= (elementXPos) &&
        mousePoint.y() <= (elementYPos + isSelectedPos)&&mousePoint.y() >= (elementYPos)) {
        isSelected_ = true;
    } else {
        isSelected_ = false;
        iPage_=1;
    }

    QWidget *pOwner = getOwnerWidget();
    if(pOwner != nullptr) {
        pOwner->update();
    }
}

void ElementComboBox::readFromXml(const QXmlStreamAttributes &attributes)
{
    if (attributes.hasAttribute("elementId")) {
        QString szID = attributes.value("elementId").toString();
        setElementId(szID);
    }

    if (attributes.hasAttribute("tag")) {
        szTagSelected_ = attributes.value("tag").toString();
    }

    if (attributes.hasAttribute("ComboBox")) {
        QString listString = attributes.value("ComboBox").toString();
        comboBox_ = listString.split('|');
    }

    if (attributes.hasAttribute("elementText")) {
        elementText = attributes.value("elementText").toString();
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


void ElementComboBox::readData(QDataStream &in)
{
    QString id;
    QString szTagSelected;
    QStringList ComboBox;
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
       >> ComboBox
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
    this->szTagSelected_ = szTagSelected;
    this->comboBox_ = ComboBox;
    this->elementText = text;
    this->szHAlign_ = hAlign;
    this->szVAlign_ = vAlign;
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

QDataStream &operator>>(QDataStream &in, ElementComboBox &ele)
{
    QString id;
    QString szTagSelected;
    QStringList ComboBox;
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
       >> ComboBox
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
    ele.szTagSelected_ = szTagSelected;
    ele.comboBox_ = ComboBox;
    ele.elementText = text;
    ele.szHAlign_ = hAlign;
    ele.szVAlign_ = vAlign;
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


