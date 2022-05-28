#include "qswitchbutton.h"
#include "../qprojectcore.h"
#include "../pubtool.h"
#include <QFileInfo>
#include <QPainter>
#include <QPixmap>
#include <QFont>
#include <QDebug>

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

    setPropertyInner();
}

void QSwitchButton::fromObject(XMLObject* xml)
{
    qDebug() << __FILE__ << __LINE__ << __FUNCTION__ << "\r\n\r\n\r\n******************\r\n\r\n****************\r\n";

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
                    } /* else if(propertyName == "funcs") {
                        this->setProperty("funcs", pObj->getProperty("value"));
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
                    }*/
                }
            }
        }
    }


#if 0
    QAbstractProperty* pObj = QPropertyFactory::create_property("Tag");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "tag");
        pObj->setAttribute("show_name", tr("选择变量"));
        pObj->setAttribute("group", "HMI");
        pObj->setAttribute(ATTR_CAN_SAME, true);
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("Function");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "onFuncs");
        pObj->setAttribute("show_name", tr("切换到开时执行功能"));
        pObj->setAttribute("group", "HMI");
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("Function");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "offFuncs");
        pObj->setAttribute("show_name", tr("切换到关时执行功能"));
        pObj->setAttribute("group", "HMI");
        insertProperty(pObj);
    }

    QWidgetHost::initProperty();

    pObj = QPropertyFactory::create_property("Bool");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "stateOnInitial");
        pObj->setAttribute("show_name", tr("初始状态"));
        pObj->setAttribute("group", "Attributes");
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("Enum");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "showContent");
        pObj->setAttribute("show_name", tr("显示内容"));
        pObj->setAttribute("group", "Attributes");
        pObj->setAttribute(ATTR_CAN_SAME, true);
        ComboItems items;
        QStringList contents;
        contents << tr("文本") << tr("图片");
        foreach(QString szEv, contents) {
            tagComboItem item;
            item.m_text = szEv;
            item.m_value = szEv;
            items.append(item);
        }
        QVariant v;
        v.setValue<ComboItems>(items);
        pObj->setAttribute("items", v);
        pObj->setValue(tr("文本"));
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("Image");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "resetPictureFile");
        pObj->setAttribute("show_name", tr("选择关时图片"));
        pObj->setAttribute("filters", "Images (*.png *.xpm *.jpg)");
        pObj->setAttribute("group", "Attributes");
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("Image");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "setPictureFile");
        pObj->setAttribute("show_name", tr("选择开时图片"));
        pObj->setAttribute("filters", "Images (*.png *.xpm *.jpg)");
        pObj->setAttribute("group", "Attributes");
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("Bool");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "showNoScale");
        pObj->setAttribute("show_name", tr("原尺寸显示"));
        pObj->setAttribute("group", "Attributes");
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("String");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "resetText");
        pObj->setAttribute("show_name", tr("关时文本")); // tr("Text")
        pObj->setAttribute("group", "Attributes");
        pObj->setValue(QString(tr("关")));
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("String");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "setText");
        pObj->setAttribute("show_name", tr("开时文本")); // tr("Text")
        pObj->setAttribute("group", "Attributes");
        pObj->setValue(QString(tr("开")));
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("Font");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "font");
        pObj->setAttribute("show_name", tr("字体"));
        pObj->setAttribute("group", "Attributes");
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("Color");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "textColor");
        pObj->setAttribute("show_name", tr("文本颜色"));
        pObj->setAttribute("group", "Attributes");
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("Enum");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "szHAlign");
        pObj->setAttribute("show_name", tr("水平对齐"));
        pObj->setAttribute("group", "Attributes");
        pObj->setAttribute(ATTR_CAN_SAME, true);
        ComboItems items;
        QStringList alignList;
        alignList << tr("左对齐") << tr("居中对齐") << tr("右对齐");
        foreach(QString szEv, alignList) {
            tagComboItem item;
            item.m_text = szEv;
            item.m_value = szEv;
            items.append(item);
        }
        QVariant v;
        v.setValue<ComboItems>(items);
        pObj->setAttribute("items", v);
        pObj->setValue(tr("居中对齐"));
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("Enum");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "szVAlign");
        pObj->setAttribute("show_name", tr("垂直对齐"));
        pObj->setAttribute("group", "Attributes");
        pObj->setAttribute(ATTR_CAN_SAME, true);
        ComboItems items;
        QStringList alignList;
        alignList << tr("上对齐") << tr("居中对齐") << tr("下对齐");
        foreach(QString szEv, alignList) {
            tagComboItem item;
            item.m_text = szEv;
            item.m_value = szEv;
            items.append(item);
        }
        QVariant v;
        v.setValue<ComboItems>(items);
        pObj->setAttribute("items", v);
        pObj->setValue(tr("居中对齐"));
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("Color");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "resetBackgroundColor");
        pObj->setAttribute("show_name", tr("关时背景颜色"));
        pObj->setAttribute("group", "Attributes");
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("Color");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "setBackgroundColor");
        pObj->setAttribute("show_name", tr("开时背景颜色"));
        pObj->setAttribute("group", "Attributes");
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("Number");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "borderWidth");
        pObj->setAttribute("show_name", tr("边框宽度"));
        pObj->setAttribute("group", "Attributes");
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("Color");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "borderColor");
        pObj->setAttribute("show_name", tr("边框颜色"));
        pObj->setAttribute("group", "Attributes");
        insertProperty(pObj);
    }


    pObj = QPropertyFactory::create_property("Bool");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "transparent");
        pObj->setAttribute("show_name", tr("透明"));
        pObj->setAttribute("group", "Attributes");
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("Bool");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "enableOnInitial");
        pObj->setAttribute("show_name", tr("初始有效"));
        pObj->setAttribute("group", "Attributes");
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("Bool");
    if(pObj != NULL) {
        pObj->setObjectProperty("name", "showOnInitial");
        pObj->setAttribute("show_name", tr("初始可见"));
        pObj->setAttribute("group", "Attributes");
        insertProperty(pObj);
    }

    setPropertyValue("geometry", QRect(0, 0, 120, 60));

    <Object id = "19" type = "buttons.switchbutton">
                             < Property name = "onFuncs" type = "Function" value = "MoveControlElement(&quot;1.3,1,2&quot;);:移动控件(&quot;main.label,1,2&quot;);" / >

                                     < / Object >
#endif
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
            borderWidth = 0;
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

void QSwitchButton::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
}

void QSwitchButton::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    if(!enableOnInitial) {
        return;
    }
    selected = true;
    this->update();
}

void QSwitchButton::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    if(!enableOnInitial) {
        return;
    }
    selected = false;
    this->update();
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

QStringList QSwitchButton::getFuncs() const
{
    return funcs;
}

void QSwitchButton::setFuncs(const QStringList &value)
{
    if(value != funcs) {
        funcs = value;
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
