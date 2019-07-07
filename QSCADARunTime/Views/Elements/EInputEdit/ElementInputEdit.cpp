#include "ElementInputEdit.h"
#include "InputMethodNumber.h"
#include "InputMethodAlphabet.h"
#include "RealTimeDB.h"
#include <QDebug>

ElementInputEdit::ElementInputEdit() {
    elementId = QString(tr("InputEdit"));
    internalElementType = tr("InputEdit");
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
    elementText = tr("输入编辑框");
    inputLineEdit_ = nullptr;
    bInEditMode_ = false;
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
    elementXPos = position.x();
    elementYPos = position.y();
    elementRect.setRect(0, 0, elementWidth, elementHeight);
}

void ElementInputEdit::updateBoundingElement() {
    elementRect.setRect(0, 0, elementWidth, elementHeight);
}

void ElementInputEdit::paint(QPainter *painter) {
    if(!showOnInitial_ || !bShow_) {
        return;
    }
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    painter->translate(QPoint(elementXPos, elementYPos));
    painter->rotate(elemAngle);
    drawInputEdit(painter);
    painter->restore();
}

/**
 * @brief ElementInputEdit::refreshTagValue
 * @details 刷新变量值
 */
void ElementInputEdit::refreshTagValue() {
    qint32 id = RealTimeDB::getIdByTagName(szTagSelected_);
    if(id != -1) {
        elementText = RealTimeDB::GetDataString(id);
    }else {
        elementText = "#";
    }
}

void ElementInputEdit::drawInputEdit(QPainter *painter) {
    refreshTagValue();
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
    QString szDrawText = "";
    if(inputPassword_) {
        int iLen = elementText.length();
        szDrawText = "";
        for(int i=0; i<iLen; i++) {
            szDrawText.append(QChar('*'));
        }
    } else {
        if(szTagSelected_ == "") {
            elementText = "";
        }
        szDrawText = elementText;
    }
    if(!bInEditMode_)
        painter->drawText(textRect, hFlags|vFlags, szDrawText);
}

void ElementInputEdit::mouseMoveEvent(QMouseEvent *event) {
    Q_UNUSED(event)
}

void ElementInputEdit::mousePressEvent(QMouseEvent *event) {
    Q_UNUSED(event)
    if(!enableOnInitial_ || !enableEdit_ || !bShow_ || !bEnable_) {
        return;
    }

    QWidget *pOwner = getOwnerWidget();
    QPointF mousePoint = event->pos();
    if (mousePoint.x() <= (elementXPos + elementWidth) &&
            mousePoint.x() >= (elementXPos) &&
            mousePoint.y() <= (elementYPos + elementHeight) &&
            mousePoint.y() >= (elementYPos)) {
        bInEditMode_ = true;
        QString szTmpElementText = elementText;
        elementText = "";
        if(pOwner != nullptr) {
            pOwner->update();
        }

        if(enableEdit_ && szTagSelected_ != "") {
            InputMethodAlphabet::instance()->setVisible(false);
            InputMethodNumber::instance()->setVisible(false);
            connect(InputMethodAlphabet::instance(), SIGNAL(enterPressed()),
                    this, SLOT(enterPressed()));
            connect(InputMethodNumber::instance(), SIGNAL(enterPressed()),
                    this, SLOT(enterPressed()));
            if(inputPassword_) {
                InputMethodAlphabet::instance()->init("control", "silvery", 14, 14);
                InputMethodNumber::instance()->unInstallInputMethod();
                InputMethodAlphabet::instance()->installInputMethod();
            } else {
                InputMethodNumber::instance()->init("silvery", 14);
                InputMethodAlphabet::instance()->unInstallInputMethod();
                InputMethodNumber::instance()->installInputMethod();
            }

            if(inputLineEdit_ == nullptr) {
                if(pOwner != nullptr) {
                    inputLineEdit_ = new InputLineEdit(pOwner);
                    inputLineEdit_->move(elementXPos+borderWidth_/2, elementYPos+borderWidth_/2);
                    inputLineEdit_->resize(elementWidth-borderWidth_, elementHeight-borderWidth_);
                    inputLineEdit_->setText(szTmpElementText);
                    inputLineEdit_->setFocus();
                    inputLineEdit_->show();
                }
            } else {
                inputLineEdit_->setText(szTmpElementText);
                inputLineEdit_->setFocus();
                inputLineEdit_->show();
            }
        } 
    } else {
        if(enableEdit_ && szTagSelected_ != "") {
            disconnect(InputMethodAlphabet::instance(), SIGNAL(enterPressed()),
                       this, SLOT(enterPressed()));
            disconnect(InputMethodNumber::instance(), SIGNAL(enterPressed()),
                       this, SLOT(enterPressed()));
        }

        if(inputLineEdit_ != nullptr) {
            inputLineEdit_->setText("");
            inputLineEdit_->hide();
        }

        if(pOwner != nullptr && bInEditMode_) {
            pOwner->setFocus();
        }

        bInEditMode_ = false;
    }
}

void ElementInputEdit::mouseReleaseEvent(QMouseEvent *event) {
    Q_UNUSED(event)
    if(!enableOnInitial_ || !bShow_ || !bEnable_) {
        return;
    }
}

void ElementInputEdit::enterPressed() {
    bInEditMode_ = false;
    elementText = inputLineEdit_->text();
    if(szTagSelected_ != "") {
        qint32 id = RealTimeDB::getIdByTagName(szTagSelected_);
        if(id != -1) {
            RealTimeDB::SetDataString(id, elementText);
        }
    }
    inputLineEdit_->setText("");
    inputLineEdit_->hide();
}

void ElementInputEdit::closePressed() {
    bInEditMode_ = false;
    inputLineEdit_->setText("");
    inputLineEdit_->hide();
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
        setElementZValue(attributes.value("z").toString().toInt());
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

    return in;
}

