#include "qpushbuttonex.h"
#include "../qprojectcore.h"
#include "../pubtool.h"
#include <QFileInfo>
#include <QPainter>
#include <QPixmap>
#include <QFont>
#include <QDebug>

QPushButtonEx::QPushButtonEx(QWidget *parent) : QPushButton(parent)
{
    showContent = tr("文本");
    showContentText = true;
    hAlign = tr("居中对齐");
    vAlign = tr("居中对齐");
    font = QFont("宋体", 12);
    backgroundColor = QColor(240, 240, 240);
    borderWidth = 4;
    borderColor = QColor(112, 112, 112);
    text = tr("弹出按钮");
    enableOnInitial = true;
    showOnInitial = true;
    transparent = false;
    script = "";
    clickedDownFuncs.clear();
    clickedUpFuncs.clear();
    enableOnInitial = true;
    showOnInitial = true;
    transparent = false;

    this->setProperty("geometry", QRect(0, 0, 100, 30));
    this->setProperty("text", "button");

    setPropertyInner();
}


void QPushButtonEx::fromObject(XMLObject* xml)
{
    if(xml != NULL) {
        if(xml->getTagName() == "Object") {
            QList<XMLObject*> properties = xml->getChildren();
            foreach(XMLObject* pObj, properties) {
                if(pObj->getTagName() == PROPERTY_TITLE) {
                    QString propertyName = pObj->getProperty("name");
                    if(propertyName == "objectName") {
                        this->setProperty("objectName", pObj->getProperty("value"));
                    } else if(propertyName == "geometry") {
                        int x, y, width, height;
                        QList<XMLObject*> tmpProps = pObj->getChildren();
                        foreach(XMLObject* propObj, tmpProps) {
                            QString propertyName = propObj->getProperty("name");
                            if(propertyName == "x") {
                                x = propObj->getProperty("value").toInt();
                            } else if(propertyName == "y") {
                                y = propObj->getProperty("value").toInt();
                            } else if(propertyName == "Width") {
                                width = propObj->getProperty("value").toInt();
                            } else if(propertyName == "Height") {
                                height = propObj->getProperty("value").toInt();
                            }
                        }
                        this->setProperty("geometry", QRect(x, y, width, height));
                    } else if(propertyName == "clickedDownFuncs") {
                        this->setProperty("clickedDownFuncs", praseFunctions(pObj->getProperty("value")));
                    } else if(propertyName == "clickedUpFuncs") {
                        this->setProperty("clickedUpFuncs", praseFunctions(pObj->getProperty("value")));
                    } else if(propertyName == "script") {
                        this->setProperty("script", pObj->getProperty("value"));
                    } else if(propertyName == "showContent") {
                        this->setProperty("showContent", pObj->getProperty("value"));
                    } else if(propertyName == "picture") {
                        this->setProperty("picture", pObj->getProperty("value"));
                    } else if(propertyName == "text") {
                        this->setProperty("text", pObj->getProperty("value"));
                    } else if(propertyName == "textColor") {
                        this->setProperty("textColor", pObj->getProperty("value"));
                    } else if(propertyName == "hAlign") {
                        this->setProperty("hAlign", pObj->getProperty("value"));
                    } else if(propertyName == "vAlign") {
                        this->setProperty("vAlign", pObj->getProperty("value"));
                    } else if(propertyName == "backgroundColor") {
                        this->setProperty("backgroundColor", pObj->getProperty("value"));
                    } else if(propertyName == "transparent") {
                        this->setProperty("transparent", pObj->getProperty("value"));
                    } else if(propertyName == "borderWidth") {
                        this->setProperty("borderWidth", pObj->getProperty("value"));
                    } else if(propertyName == "borderColor") {
                        this->setProperty("borderColor", pObj->getProperty("value"));
                    } else if(propertyName == "font") {
                        this->setProperty("font", pObj->getProperty("value"));
                    } else if(propertyName == "enableOnInitial") {
                        this->setProperty("enableOnInitial", pObj->getProperty("value"));
                    } else if(propertyName == "showOnInitial") {
                        this->setProperty("showOnInitial", pObj->getProperty("value"));
                    }
                }
            }
        }
    }
}

void QPushButtonEx::setPropertyInner()
{

}

void QPushButtonEx::drawPushButton(QPainter *painter)
{
    painter->setRenderHints(QPainter::HighQualityAntialiasing | QPainter::TextAntialiasing);
    QRect rect = QRect(0, 0, width(), height());

    if(transparent) {
        painter->setPen(QPen(Qt::gray, 1, Qt::DashLine));
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(rect);
    } else {
        for(int i = 0; i < borderWidth; i++) {
            PubTool::DrawFrameRect(painter, rect, borderColor);
            if(i < borderWidth / 2) {
                rect.adjust(1, 1, -1, -1);
            } else {
                rect.adjust(1, 1, 0, 0);
            }
        }

        PubTool::DrawFrameRect(painter, rect, QColor(252, 252, 252));
        rect.adjust(1, 1, -1, -1);
        rect.adjust(-1, -1, 0, 0);

        if(showContentText) { // 文本+背景
            PubTool::FillFullRect(painter, rect, backgroundColor);
            painter->setPen(textColor);
            painter->setBrush(Qt::NoBrush);
            painter->setFont(font);

            int hFlags = Qt::AlignLeft;
            if(hAlign == tr("左对齐")) {
                hFlags = Qt::AlignLeft;
            } else if(hAlign == tr("居中对齐")) {
                hFlags = Qt::AlignHCenter;
            } else if(hAlign == tr("右对齐")) {
                hFlags = Qt::AlignRight;
            }

            int vFlags = Qt::AlignVCenter;
            if(vAlign == tr("上对齐")) {
                vFlags = Qt::AlignTop;
            } else if(vAlign == tr("居中对齐")) {
                vFlags = Qt::AlignVCenter;
            } else if(vAlign == tr("下对齐")) {
                vFlags = Qt::AlignBottom;
            }

            QRectF textRect = rect.normalized().adjusted(borderWidth, borderWidth, -borderWidth, -borderWidth);
            painter->drawText(textRect, hFlags | vFlags, text);

        } else { // 图片按钮
            borderWidth = 0;
            if(filePicture != QString()) {
                if(rect.contains(imageObj.rect())) {
                    QRect drawRect = QRect((width() - imageObj.width()) / 2, (height() - imageObj.height()) / 2, imageObj.width(), imageObj.height());
                    painter->drawImage(drawRect, imageObj);
                } else {
                    painter->drawImage(rect, imageObj);
                }
            }
        }

        // 绘制选中状态
        if (selected) {
            int rectWidth = borderWidth + 4;
            QRect rect = QRect(rectWidth, rectWidth, width() - 2 * rectWidth, height() - 2 * rectWidth);
            QPen pen(Qt::lightGray, 1, Qt::DashLine);
            painter->setBrush(Qt::NoBrush);
            painter->setPen(pen);
            painter->drawRect(rect);
        }
    }
}

void QPushButtonEx::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::HighQualityAntialiasing | QPainter::TextAntialiasing);
    drawPushButton(&painter);
}

void QPushButtonEx::resizeEvent(QResizeEvent *event)
{
    setPropertyInner();
    QPushButton::resizeEvent(event);
}

void QPushButtonEx::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
}

void QPushButtonEx::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    if(!enableOnInitial) {
        return;
    }
    execFunction(clickedDownFuncs);
    selected = true;
    this->update();
}

void QPushButtonEx::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    if(!enableOnInitial) {
        return;
    }
    execFunction(clickedUpFuncs);
    selected = false;
    this->update();
}

QFont QPushButtonEx::getFont() const
{
    return font;
}

void QPushButtonEx::setFont(const QFont &value)
{
    if(value != font) {
        font = value;
        this->update();
    }
}

bool QPushButtonEx::getTransparent() const
{
    return transparent;
}

void QPushButtonEx::setTransparent(bool value)
{
    if(value != transparent) {
        transparent = value;
        this->update();
    }
}

QColor QPushButtonEx::getBackgroundColor() const
{
    return backgroundColor;
}

void QPushButtonEx::setBackgroundColor(const QColor &value)
{
    if(value != backgroundColor) {
        backgroundColor = value;
        this->update();
    }
}

QString QPushButtonEx::getVAlign() const
{
    return vAlign;
}

void QPushButtonEx::setVAlign(const QString &value)
{
    if(value != vAlign) {
        vAlign = value;
        this->update();
    }
}

QString QPushButtonEx::getHAlign() const
{
    return hAlign;
}

void QPushButtonEx::setHAlign(const QString &value)
{
    if(value != hAlign) {
        hAlign = value;
        this->update();
    }
}

QString QPushButtonEx::getFilePicture() const
{
    return filePicture;
}

void QPushButtonEx::setFilePicture(const QString &value)
{
    if(value != filePicture) {
        QStringList szListInfo = value.split("|"); // 图片名称|图片BASE64数据
        if(szListInfo.size() == 2) {
            QFileInfo info(szListInfo.at(0));
            imageObj = PictureResourceManager::base64ToImage(szListInfo.at(1).toLocal8Bit(), info.suffix());
            this->update();
        }
        filePicture = value;
    }
}

QString QPushButtonEx::getShowContent() const
{
    return showContent;
}

void QPushButtonEx::setShowContent(const QString &value)
{
    if(value != showContent) {
        showContent = value;
        showContentText = true;
        if(showContent == tr("图片")) {
            showContentText = false;
        }
        this->update();
    }
}

QString QPushButtonEx::getScript() const
{
    return script;
}

void QPushButtonEx::setScript(const QString &value)
{
    script = value;
}

QColor QPushButtonEx::getTextColor() const
{
    return textColor;
}

void QPushButtonEx::setTextColor(const QColor &value)
{
    if(value != textColor) {
        textColor = value;
        this->update();
    }
}

QColor QPushButtonEx::getBorderColor() const
{
    return borderColor;
}

void QPushButtonEx::setBorderColor(const QColor &value)
{
    if(value != borderColor) {
        borderColor = value;
        this->update();
    }
}

int QPushButtonEx::getBorderWidth() const
{
    return borderWidth;
}

void QPushButtonEx::setBorderWidth(int value)
{
    if(value != borderWidth) {
        borderWidth = value;
        this->update();
    }
}

bool QPushButtonEx::getEnableOnInitial() const
{
    return enableOnInitial;
}

void QPushButtonEx::setEnableOnInitial(bool value)
{
    if(value != enableOnInitial) {
        enableOnInitial = value;
        this->update();
    }
}

QString QPushButtonEx::getText() const
{
    return text;
}

void QPushButtonEx::setText(const QString &value)
{
    if(value != text) {
        text = value;
        this->update();
    }
}

QStringList QPushButtonEx::getClickedDownFuncs() const
{
    return clickedDownFuncs;
}

void QPushButtonEx::setClickedDownFuncs(const QStringList &value)
{
    if(value != clickedDownFuncs) {
        clickedDownFuncs = value;
        this->update();
    }
}

QStringList QPushButtonEx::getClickedUpFuncs() const
{
    return clickedUpFuncs;
}

void QPushButtonEx::setClickedUpFuncs(const QStringList &value)
{
    if(value != clickedUpFuncs) {
        clickedUpFuncs = value;
        this->update();
    }
}

bool QPushButtonEx::getShowOnInitial() const
{
    return showOnInitial;
}

void QPushButtonEx::setShowOnInitial(bool value)
{
    if(value != showOnInitial) {
        showOnInitial = value;
        this->update();
    }
}

