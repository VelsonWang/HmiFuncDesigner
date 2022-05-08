#include "qswitchbutton.h"
#include "../qprojectcore.h"
#include "../pubtool.h"
#include <QFileInfo>
#include <QPainter>
#include <QPixmap>
#include <QFont>

QSwitchButton::QSwitchButton(QWidget *parent) : QWidget(parent)
{
    showContent = tr("文本");
    showContentText = true;
    szHAlign = tr("居中对齐");
    szVAlign = tr("居中对齐");
    font = QFont("宋体", 12);
    resetBackgroundColor = QColor(240, 240, 240);
    setBackgroundColor = QColor(240, 240, 240);
    borderWidth = 4;
    borderColor = QColor(112, 112, 112);
    textColor = QColor(Qt::black);
    stateOnInitial = true;
    enableOnInitial = true;
    showOnInitial = true;
    transparent = false;
    showNoScale = false;
    resetPictureFile = "";
    setPictureFile = "";
    resetText = tr("关");
    setText = tr("开");
    onFuncs.clear();
    offFuncs.clear();
    setPropertyInner();
}

void QSwitchButton::setPropertyInner()
{

}

void QSwitchButton::drawSwitchButton(QPainter *painter)
{
    painter->setRenderHints(QPainter::HighQualityAntialiasing | QPainter::TextAntialiasing);
    QRect rect = QRect(0, 0, width(), height());

    if(transparent) {
        painter->setPen(QPen(Qt::gray, 1, Qt::DashLine));
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(rect);
    } else {
        if(showContentText) { // 文本+背景
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

            QString szElementText = QString();
            QColor backgroundColor = QColor();

            if(stateOnInitial) {
                szElementText = setText;
                backgroundColor = setBackgroundColor;
            } else {
                szElementText = resetText;
                backgroundColor = resetBackgroundColor;
            }

            PubTool::FillFullRect(painter, rect, backgroundColor);
            painter->setPen(textColor);
            painter->setBrush(Qt::NoBrush);
            painter->setFont(font);

            int hFlags = Qt::AlignLeft;
            if(szHAlign == tr("左对齐")) {
                hFlags = Qt::AlignLeft;
            } else if(szHAlign == tr("居中对齐")) {
                hFlags = Qt::AlignHCenter;
            } else if(szHAlign == tr("右对齐")) {
                hFlags = Qt::AlignRight;
            }

            int vFlags = Qt::AlignVCenter;
            if(szVAlign == tr("上对齐")) {
                vFlags = Qt::AlignTop;
            } else if(szVAlign == tr("居中对齐")) {
                vFlags = Qt::AlignVCenter;
            } else if(szVAlign == tr("下对齐")) {
                vFlags = Qt::AlignBottom;
            }

            QRectF textRect = rect.normalized().adjusted(borderWidth, borderWidth, -borderWidth, -borderWidth);
            painter->drawText(textRect, hFlags | vFlags, szElementText);
        } else { // 图片
            QString szPictureFile = stateOnInitial ? setPictureFile : resetPictureFile;
            if(szPictureFile != QString()) {
                QImage scaleImage;
                if(stateOnInitial) {
                    scaleImage = setImageObj;
                } else {
                    scaleImage = resetImageObj;
                }
                if(showNoScale) {
                    QRect drawRect = QRect((width() - scaleImage.width()) / 2, (height() - scaleImage.height()) / 2, scaleImage.width(), scaleImage.height());
                    painter->drawImage(drawRect, scaleImage);
                } else {
                    painter->drawImage(rect, scaleImage);
                }
            }
            painter->setPen(QPen(Qt::gray, 1, Qt::DashDotLine));
            painter->drawRect(rect);
        }
    }
}

void QSwitchButton::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::HighQualityAntialiasing | QPainter::TextAntialiasing);
    drawSwitchButton(&painter);
    QWidget::paintEvent(event);
}

void QSwitchButton::resizeEvent(QResizeEvent *event)
{
    setPropertyInner();
    QWidget::resizeEvent(event);
}

QColor QSwitchButton::getTextColor() const
{
    return textColor;
}

void QSwitchButton::setTextColor(const QColor &value)
{
    if(value != textColor) {
        textColor = value;
        this->update();
    }
}

QColor QSwitchButton::getBorderColor() const
{
    return borderColor;
}

void QSwitchButton::setBorderColor(const QColor &value)
{
    if(value != borderColor) {
        borderColor = value;
        this->update();
    }
}

int QSwitchButton::getBorderWidth() const
{
    return borderWidth;
}

void QSwitchButton::setBorderWidth(int value)
{
    if(value != borderWidth) {
        borderWidth = value;
        this->update();
    }
}

bool QSwitchButton::getEnableOnInitial() const
{
    return enableOnInitial;
}

void QSwitchButton::setEnableOnInitial(bool value)
{
    if(value != enableOnInitial) {
        enableOnInitial = value;
        this->update();
    }
}

QFont QSwitchButton::getFont() const
{
    return font;
}

void QSwitchButton::setFont(const QFont &value)
{
    if(value != font) {
        font = value;
        this->update();
    }
}

bool QSwitchButton::getTransparent() const
{
    return transparent;
}

void QSwitchButton::setTransparent(bool value)
{
    if(value != transparent) {
        transparent = value;
        this->update();
    }
}

QColor QSwitchButton::getSetBackgroundColor() const
{
    return setBackgroundColor;
}

void QSwitchButton::setSetBackgroundColor(const QColor &value)
{
    if(value != setBackgroundColor) {
        setBackgroundColor = value;
        this->update();
    }
}

QColor QSwitchButton::getResetBackgroundColor() const
{
    return resetBackgroundColor;
}

void QSwitchButton::setResetBackgroundColor(const QColor &value)
{
    if(value != resetBackgroundColor) {
        resetBackgroundColor = value;
        this->update();
    }
}

QString QSwitchButton::getSzVAlign() const
{
    return szVAlign;
}

void QSwitchButton::setSzVAlign(const QString &value)
{
    if(value != szVAlign) {
        szVAlign = value;
        this->update();
    }
}

QString QSwitchButton::getSzHAlign() const
{
    return szHAlign;
}

void QSwitchButton::setSzHAlign(const QString &value)
{
    if(value != szHAlign) {
        szHAlign = value;
        this->update();
    }
}

QString QSwitchButton::getSetText() const
{
    return setText;
}

void QSwitchButton::setSetText(const QString &value)
{
    if(value != setText) {
        setText = value;
        this->update();
    }
}

QString QSwitchButton::getResetText() const
{
    return resetText;
}

void QSwitchButton::setResetText(const QString &value)
{
    if(value != resetText) {
        resetText = value;
        this->update();
    }
}

bool QSwitchButton::getShowNoScale() const
{
    return showNoScale;
}

void QSwitchButton::setShowNoScale(bool value)
{
    if(value != showNoScale) {
        showNoScale = value;
        this->update();
    }
}

QString QSwitchButton::getSetPictureFile() const
{
    return setPictureFile;
}

void QSwitchButton::setSetPictureFile(const QString &value)
{
    if(value != setPictureFile) {
        QStringList szListInfo = value.split("|"); // 图片名称|图片BASE64数据
        if(szListInfo.size() == 2) {
            QFileInfo info(szListInfo.at(0));
            setImageObj = PictureResourceManager::base64ToImage(szListInfo.at(1).toLocal8Bit(), info.suffix());
            this->update();
        }
        setPictureFile = value;
    }
}

QString QSwitchButton::getResetPictureFile() const
{
    return resetPictureFile;
}

void QSwitchButton::setResetPictureFile(const QString &value)
{
    if(value != resetPictureFile) {
        QStringList szListInfo = value.split("|"); // 图片名称|图片BASE64数据
        if(szListInfo.size() == 2) {
            QFileInfo info(szListInfo.at(0));
            resetImageObj = PictureResourceManager::base64ToImage(szListInfo.at(1).toLocal8Bit(), info.suffix());
            this->update();
        }
        resetPictureFile = value;
    }
}

QString QSwitchButton::getShowContent() const
{
    return showContent;
}

void QSwitchButton::setShowContent(const QString &value)
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

bool QSwitchButton::getStateOnInitial() const
{
    return stateOnInitial;
}

void QSwitchButton::setStateOnInitial(bool value)
{
    if(value != stateOnInitial) {
        stateOnInitial = value;
        this->update();
    }
}

QStringList QSwitchButton::getOnFuncs() const
{
    return onFuncs;
}

void QSwitchButton::setOnFuncs(const QStringList &value)
{
    if(value != onFuncs) {
        onFuncs = value;
        this->update();
    }
}

QStringList QSwitchButton::getOffFuncs() const
{
    return offFuncs;
}

void QSwitchButton::setOffFuncs(const QStringList &value)
{
    if(value != offFuncs) {
        offFuncs = value;
        this->update();
    }
}

bool QSwitchButton::getShowOnInitial() const
{
    return showOnInitial;
}

void QSwitchButton::setShowOnInitial(bool value)
{
    if(value != showOnInitial) {
        showOnInitial = value;
        this->update();
    }
}

QString QSwitchButton::getTagSelected() const
{
    return tagSelected;
}

void QSwitchButton::setTagSelected(const QString &value)
{
    if(value != tagSelected) {
        tagSelected = value;
        this->update();
    }
}
