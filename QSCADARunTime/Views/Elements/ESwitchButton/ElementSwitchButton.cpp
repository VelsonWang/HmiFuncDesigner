#include "ElementSwitchButton.h"
#include "SCADARunTime.h"
#include "Log.h"
#include "PubTool.h"
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QDebug>

ElementSwitchButton::ElementSwitchButton()
{
    internalElementType = tr("SwitchButton");
    showContent_ = tr("文本");
    bShowContentText_ = true;
    szHAlign_ = tr("居中对齐");
    szVAlign_ = tr("居中对齐");
    font_ = QFont("宋体", 12);
    init();
    elementWidth = 100;
    elementHeight = 40;
    resetBackgroundColor_ = QColor(240, 240, 240);
    setBackgroundColor_ = QColor(240, 240, 240);
    signBackgroundColor = QColor(Qt::black);
    borderWidth = 4;
    borderColor = QColor(112, 112, 112);
    enableOnInitial_ = true;
    showOnInitial_ = true;
    transparent_ = false;
    showNoScale_ = false;
    resetPictureFile_ = "";
    setPictureFile_ = "";
    resetText_ = tr("关");
    setText_ = tr("开");
}

QRectF ElementSwitchButton::boundingRect() const
{
    qreal extra = 6;
    QRectF rect(elementRect.toRect());
    return rect.normalized().adjusted(extra, extra, -extra, -extra);
}


QPainterPath ElementSwitchButton::shape() const
{
    QPainterPath path;
    path.addRect(elementRect);
    return path;
}


void ElementSwitchButton::setClickPosition(QPointF position)
{
    elementXPos = position.x();
    elementYPos = position.y();
    elementRect.setRect(0, 0, elementWidth, elementHeight);
}

void ElementSwitchButton::updateBoundingElement()
{
    elementRect.setRect(0, 0, elementWidth, elementHeight);
}

void ElementSwitchButton::paint(QPainter *painter)
{
    if(!showOnInitial_ || !bShow_) {
        return;
    }

    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    painter->translate(QPoint(elementXPos, elementYPos));
    painter->rotate(elemAngle);

    drawSwitchButton(painter);

    if (isSelected_) {
        painter->setPen(QPen(Qt::gray, 2, Qt::DashLine));
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(boundingRect());
    }
    painter->restore();
}

void ElementSwitchButton::execScriptFunction(bool bVal)
{
    if(bLastTagVal_ != bVal) {
        if(!bLastTagVal_ && bVal) {
            // 处理"切换到开事件"功能
            SCADARunTime::execScriptFunction(funcs_, tr("切换到开事件"));
        } else if(bLastTagVal_ && !bVal) {
            // 处理"切换到关事件"功能
            SCADARunTime::execScriptFunction(funcs_, tr("切换到关事件"));
        } else {

        }
        bLastTagVal_ = bVal;
    }
}


void ElementSwitchButton::drawSwitchButton(QPainter *painter)
{
    QRect rect(elementRect.x(), elementRect.y(), elementRect.width(), elementRect.height());

    if(transparent_) {
        painter->setPen(QPen(Qt::gray, 1, Qt::DashLine));
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(rect);
    } else {

        QString szFileIndicationLamp_ = QString();
        qint32 id = -1;
        if (szTagSelected_ != "") {
            id = RealTimeDB::getIdByTagName(szTagSelected_);
        }

        if(bShowContentText_) { // 文本+背景
            for(int i=0; i<borderWidth; i++) {
                PubTool::DrawFrameRect(painter, rect, borderColor);
                if(i<borderWidth/2) rect.adjust(1, 1, -1, -1);
                else rect.adjust(1, 1, 0, 0);
            }

            PubTool::DrawFrameRect(painter, rect, QColor(252, 252, 252));
            rect.adjust(1, 1, -1, -1);
            rect.adjust(-1, -1, 0, 0);

            QString szElementText = QString();
            QColor backgroundColor = QColor();

            bool bVal = false;
            if(id != -1) {
                bVal = RealTimeDB::GetDataString(id).toInt() > 0 ? true : false;
                // 执行功能脚本
                execScriptFunction(bVal);
                if(bVal) {
                    szElementText = setText_;
                    backgroundColor = setBackgroundColor_;
                } else {
                    szElementText = resetText_;
                    backgroundColor = resetBackgroundColor_;
                }
            }

            PubTool::FillFullRect(painter, rect, backgroundColor);
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
            QRectF textRect = rect.normalized().adjusted(borderWidth, borderWidth, -borderWidth, -borderWidth);
            painter->drawText(textRect, hFlags|vFlags, szElementText);
        } else { // 图片
            QString szPictureFile = QString();
            bool bVal = false;
            if(id != -1) {
                bVal = RealTimeDB::GetDataString(id).toInt() > 0 ? true : false;
                // 执行功能脚本
                execScriptFunction(bVal);
                if(bVal) {
                    szPictureFile = setPictureFile_;
                } else {
                    szPictureFile = resetPictureFile_;
                }
            }

            if(szPictureFile != QString()) {
                QString picture = getProjectPath() + "/Pictures/" + szPictureFile;
                if(QFile::exists(picture)) {
                    QImage image(picture);
                    QImage scaleImage;
                    if(showNoScale_) {
                        scaleImage = image;
                    } else {
                        scaleImage = image.scaled((int)elementRect.width(), (int)elementRect.height(), Qt::IgnoreAspectRatio);
                    }
                    painter->setRenderHints(QPainter::HighQualityAntialiasing | QPainter::TextAntialiasing);
                    painter->drawImage(elementRect, scaleImage);
                }
            }
        }
    }
}

void ElementSwitchButton::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
}

void ElementSwitchButton::mousePressEvent(QMouseEvent *event)
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
        qint32 iTagId = -1;
        if (!isSelected_ && szTagSelected_ != "") {
            iTagId = RealTimeDB::getIdByTagName(szTagSelected_);
            if (iTagId != -1) {
                RealTimeDB::SetDataString(iTagId, bLastTagVal_ ? "0" : "1");
            }
        }
        isSelected_ = true;
    } else {
        isSelected_ = false;
    }

    QWidget *pOwner = getOwnerWidget();
    if(pOwner != nullptr) {
        pOwner->update();
    }
}

void ElementSwitchButton::mouseReleaseEvent(QMouseEvent *event)
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
    }

    QWidget *pOwner = getOwnerWidget();
    if(pOwner != nullptr) {
        pOwner->update();
    }
}


void ElementSwitchButton::readFromXml(const QXmlStreamAttributes &attributes)
{
    if (attributes.hasAttribute("elementId")) {
        QString szID = attributes.value("elementId").toString();
        setElementId(szID);
    }

    if (attributes.hasAttribute("tag")) {
        szTagSelected_ = attributes.value("tag").toString();
    }

    if (attributes.hasAttribute("stateOnInitial")) {
        QString value = attributes.value("stateOnInitial").toString();
        stateOnInitial_ = false;
        if(value == "true") {
            stateOnInitial_ = true;
        }
    }

    if (attributes.hasAttribute("resetPicture")) {
        resetPictureFile_ = attributes.value("resetPicture").toString();
    }

    if (attributes.hasAttribute("setPicture")) {
        setPictureFile_ = attributes.value("setPicture").toString();
    }

    if (attributes.hasAttribute("showNoScale")) {
        QString value = attributes.value("showNoScale").toString();
        showNoScale_ = false;
        if(value == "true") {
            showNoScale_ = true;
        }
    }

    if (attributes.hasAttribute("resettext")) {
        resetText_ = attributes.value("resettext").toString();
    }

    if (attributes.hasAttribute("settext")) {
        setText_ = attributes.value("settext").toString();
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
        bShowContentText_ = true;
        if(showContent_ == tr("图片")) {
            bShowContentText_ = false;
        }
    }

    if (attributes.hasAttribute("halign")) {
        QString align = attributes.value("halign").toString();
        this->setHAlignString(align, szHAlign_);
    }

    if (attributes.hasAttribute("valign")) {
        QString align = attributes.value("valign").toString();
        this->setVAlignString(align, szVAlign_);
    }

    if (attributes.hasAttribute("resetBackgroundColor")) {
        resetBackgroundColor_ = QColor(attributes.value("resetBackgroundColor").toString());
    }

    if (attributes.hasAttribute("setBackgroundColor")) {
        setBackgroundColor_ = QColor(attributes.value("setBackgroundColor").toString());
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

    updateBoundingElement();

    qint32 iTagId = -1;
    if (szTagSelected_ != "") {
        iTagId = RealTimeDB::getIdByTagName(szTagSelected_);
        if (iTagId != -1) {
            if(stateOnInitial_) {
                RealTimeDB::SetDataString(iTagId, "1");
                bLastTagVal_ = true;
            } else {
                RealTimeDB::SetDataString(iTagId, "0");
                bLastTagVal_ = false;
            }
        }
    }
}

void ElementSwitchButton::readData(QDataStream &in)
{
    QString id;
    QString szTagSelected;
    bool stateOnInitial;
    QString resetPic;
    QString setPic;
    bool showNoScale;
    QString szResetText;
    QString szSetText;
    qreal xpos;
    qreal ypos;
    qreal zvalue;
    int width;
    int height;
    QString showContent;
    QString text;
    QString hAlign;
    QString vAlign;
    QColor resetBackgroundColor;
    QColor setBackgroundColor;
    bool transparent;
    QString font;
    QColor textColor;
    int fontSize;
    qreal angle;
    bool enableOnInitial;
    bool showOnInitial;
    QStringList funcs;

    in >> id
            >> szTagSelected
            >> stateOnInitial
            >> resetPic
            >> setPic
            >> showNoScale
            >> szResetText
            >> szSetText
            >> xpos
            >> ypos
            >> zvalue
            >> width
            >> height
            >> showContent
            >> text
            >> hAlign
            >> vAlign
            >> resetBackgroundColor
            >> setBackgroundColor
            >> transparent
            >> font
            >> textColor
            >> fontSize
            >> angle
            >> enableOnInitial
            >> showOnInitial
            >> funcs;

    this->setElementId(id);
    this->szTagSelected_ = szTagSelected;
    this->stateOnInitial_ = stateOnInitial;
    this->resetPictureFile_ = resetPic;
    this->setPictureFile_ = setPic;
    this->showNoScale_ = showNoScale;
    this->resetText_ = szResetText;
    this->setText_ = szSetText;
    this->setElementXPos(xpos);
    this->setElementYPos(ypos);
    this->setElementZValue(zvalue);
    this->setElementWidth(width);
    this->setElementHeight(height);
    this->showContent_ = showContent;
    this->setHAlignString(hAlign, szHAlign_);
    this->setVAlignString(vAlign, szVAlign_);
    this->resetBackgroundColor_ = resetBackgroundColor;
    this->setBackgroundColor_ = setBackgroundColor;
    this->transparent_ = transparent;
    this->textColor = textColor;
    this->fontSize = fontSize;
    this->setAngle(angle);
    this->enableOnInitial_ = enableOnInitial;
    this->showOnInitial_ = showOnInitial;
    this->funcs_ = funcs;
    this->updateBoundingElement();

    qint32 iTagId = -1;
    if (szTagSelected_ != "") {
        iTagId = RealTimeDB::getIdByTagName(szTagSelected_);
        if (iTagId != -1) {
            if(stateOnInitial_) {
                RealTimeDB::SetDataString(iTagId, "1");
                bLastTagVal_ = true;
            } else {
                RealTimeDB::SetDataString(iTagId, "0");
                bLastTagVal_ = false;
            }
        }
    }
}


QDataStream &operator>>(QDataStream &in,ElementSwitchButton &ele)
{
    QString id;
    QString szTagSelected;
    bool stateOnInitial;
    bool showNoScale;
    QString szResetText;
    QString szSetText;
    qreal xpos;
    qreal ypos;
    qreal zvalue;
    int width;
    int height;
    QString showContent;
    QColor resetBackgroundColor;
    QColor setBackgroundColor;
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

    in >> id
            >> szTagSelected
            >> stateOnInitial
            >> showNoScale
            >> szResetText
            >> szSetText
            >> xpos
            >> ypos
            >> zvalue
            >> width
            >> height
            >> showContent
            >> text
            >> hAlign
            >> vAlign
            >> resetBackgroundColor
            >> setBackgroundColor
            >> transparent
            >> font
            >> textColor
            >> fontSize
            >> angle
            >> enableOnInitial
            >> showOnInitial
            >> funcs;

    ele.setElementId(id);
    ele.szTagSelected_ = szTagSelected;
    ele.stateOnInitial_ = stateOnInitial;
    ele.showNoScale_ = showNoScale;
    ele.resetText_ = szResetText;
    ele.setText_ = szSetText;
    ele.setElementXPos(xpos);
    ele.setElementYPos(ypos);
    ele.setElementZValue(zvalue);
    ele.setElementWidth(width);
    ele.setElementHeight(height);
    ele.showContent_ = showContent;
    ele.setHAlignString(hAlign, ele.szHAlign_);
    ele.setVAlignString(vAlign, ele.szVAlign_);
    ele.resetBackgroundColor_ = resetBackgroundColor;
    ele.setBackgroundColor_ = setBackgroundColor;
    ele.transparent_ = transparent;
    ele.font_ = font;
    ele.textColor = textColor;
    ele.fontSize = fontSize;
    ele.setAngle(angle);
    ele.enableOnInitial_ = enableOnInitial;
    ele.showOnInitial_ = showOnInitial;
    ele.funcs_ = funcs;
    ele.updateBoundingElement();

    qint32 iTagId = -1;
    if (ele.szTagSelected_ != "") {
        iTagId = RealTimeDB::getIdByTagName(ele.szTagSelected_);
        if (iTagId != -1) {
            if(ele.stateOnInitial_) {
                RealTimeDB::SetDataString(iTagId, "1");
                ele.bLastTagVal_ = true;
            } else {
                RealTimeDB::SetDataString(iTagId, "0");
                ele.bLastTagVal_ = false;
            }
        }
    }

    return in;
}


