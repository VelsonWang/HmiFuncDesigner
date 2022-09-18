#include "qindicationlamp.h"
#include "../qprojectcore.h"
#include <QFileInfo>
#include <QPainter>
#include <QPixmap>

QIndicationLamp::QIndicationLamp(QWidget *parent) : QLabel(parent)
{
    m_tagId = "";
    m_tag = NULL;
    m_bStateOnInitial = false;
    m_szResetImageFile = "";
    m_szSetImageFile = "";
    m_bNoScale = false;
    m_iBoardWidth = 1;
    m_boardColorObj = Qt::black;
    this->setAlignment(Qt::AlignCenter);
    setPropertyInner();
}

void QIndicationLamp::fromObject(XMLObject* xml)
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
                    } else if(propertyName == "tag") {
                        this->setProperty("tag", pObj->getProperty("value"));
                    } else if(propertyName == "StateOnInitial") {
                        this->setProperty("StateOnInitial", pObj->getProperty("value"));
                    } else if(propertyName == "ResetImageFile") {
                        this->setProperty("ResetImageFile", pObj->getProperty("value"));
                    } else if(propertyName == "SetImageFile") {
                        this->setProperty("SetImageFile", pObj->getProperty("value"));
                    } else if(propertyName == "NoScale") {
                        this->setProperty("NoScale", pObj->getProperty("value"));
                    } else if(propertyName == "BoardWidth") {
                        this->setProperty("BoardWidth", pObj->getProperty("value"));
                    } else if(propertyName == "BoardColor") {
                        this->setProperty("BoardColor", pObj->getProperty("value"));
                    }
                }
            }
        }
    }
}

QString QIndicationLamp::getTag()
{
    return m_tagId;
}

void QIndicationLamp::setTag(const QString szTag)
{
    if(m_tagId != szTag) {
        m_tagId = szTag;
        m_tag = RealTimeDB::instance()->tag(tagId(m_tagId));
    }
}

bool QIndicationLamp::getStateOnInitial()
{
    return m_bStateOnInitial;
}

void QIndicationLamp::setStateOnInitial(bool bState)
{
    m_bStateOnInitial = bState;
    setPropertyInner();
}

QString QIndicationLamp::getResetImageFile()
{
    return m_szResetImageFile;
}

void QIndicationLamp::setResetImageFile(const QString szName)
{
    QStringList szListInfo = szName.split("|"); // 图片名称|图片BASE64数据
    if(szListInfo.size() == 2) {
        QFileInfo info(szListInfo.at(0));
        m_resetImageObj = PictureResourceManager::base64ToImage(szListInfo.at(1).toLocal8Bit(), info.suffix());
        setPropertyInner();
    }
    m_szResetImageFile = szName;
}

QString QIndicationLamp::getSetImageFile()
{
    return m_szSetImageFile;
}

void QIndicationLamp::setSetImageFile(const QString szName)
{
    QStringList szListInfo = szName.split("|"); // 图片名称|图片BASE64数据
    if(szListInfo.size() == 2) {
        QFileInfo info(szListInfo.at(0));
        m_setImageObj = PictureResourceManager::base64ToImage(szListInfo.at(1).toLocal8Bit(), info.suffix());
        setPropertyInner();
    }
    m_szSetImageFile = szName;
}

bool QIndicationLamp::isNoScale()
{
    return m_bNoScale;
}

void QIndicationLamp::setNoScale(bool bNoScale)
{
    m_bNoScale = bNoScale;
    setPropertyInner();
}

int QIndicationLamp::getBoardWidth()
{
    return m_iBoardWidth;
}

void QIndicationLamp::setBoardWidth(int iWidth)
{
    m_iBoardWidth = iWidth;
    setPropertyInner();
}

QColor QIndicationLamp::getBoardColor()
{
    return m_boardColorObj;
}

void QIndicationLamp::setBoardColor(QColor color)
{
    m_boardColorObj = color;
    setPropertyInner();
}

void QIndicationLamp::setPropertyInner()
{
    setScaledContents(!m_bNoScale);
    if(!m_bNoScale) {
        if(m_tag == NULL) {
            if(!m_bStateOnInitial && (m_szResetImageFile != "")) {
                setPixmap(QPixmap::fromImage(m_resetImageObj));
            }
            if(m_bStateOnInitial && (m_szSetImageFile != "")) {
                setPixmap(QPixmap::fromImage(m_setImageObj));
            }
        } else {
            if(!m_tag->toBool() && (m_szResetImageFile != "")) {
                setPixmap(QPixmap::fromImage(m_resetImageObj));
            }
            if(m_tag->toBool() && (m_szSetImageFile != "")) {
                setPixmap(QPixmap::fromImage(m_setImageObj));
            }
        }

    }
    if(m_iBoardWidth > 0) {
        QString szStyleSheet = "";
        szStyleSheet += QString("border-width: %1px; border-style: solid;").arg(QString::number(m_iBoardWidth));
        szStyleSheet += QString("border-color: rgb(%1, %2, %3);")
                        .arg(QString::number(m_boardColorObj.red()))
                        .arg(QString::number(m_boardColorObj.green()))
                        .arg(QString::number(m_boardColorObj.blue()));
        this->setStyleSheet(szStyleSheet);
    } else {
        this->setStyleSheet("");
    }
}

void QIndicationLamp::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::HighQualityAntialiasing | QPainter::TextAntialiasing);
    QString szFileIndicationLamp = m_szResetImageFile;
    if(m_tag) {
        szFileIndicationLamp = m_tag->toBool() ? m_szSetImageFile : m_szResetImageFile;
    } else {
        m_tag = RealTimeDB::instance()->tag(tagId(m_tagId));
    }

    if(szFileIndicationLamp == "") {
        QRect rect = QRect(0, 0, width(), height());
        qreal fHalfWidth = rect.width() / 2;
        qreal fHalfHeight = rect.height() / 2;
        qreal fRadius = (fHalfWidth < fHalfHeight) ? fHalfWidth : fHalfHeight;
        QRadialGradient radialGradient(fHalfWidth, fHalfHeight, fRadius, fHalfWidth, fHalfHeight);
        // 创建了一个QRadialGradient对象实例，参数分别为中心坐标，半径长度和焦点坐标,
        // 如果需要对称那么中心坐标和焦点坐标要一致
        if(m_tag && m_tag->toBool()) {
            radialGradient.setColorAt(0, Qt::yellow);
            radialGradient.setColorAt(0.8, Qt::blue); // 设置50%处的半径为蓝色
        } else {
            radialGradient.setColorAt(0, Qt::black);
            radialGradient.setColorAt(0.8, Qt::white); // 设置50%处的半径为蓝色
        }
        radialGradient.setColorAt(1, Qt::darkGray);
        painter.setBrush(QBrush(radialGradient));
        QRectF tmpRect;
        tmpRect.setX(rect.x() + rect.width() / 2 - fRadius);
        tmpRect.setY(rect.y() + rect.height() / 2 - fRadius);
        tmpRect.setWidth(fRadius * 2);
        tmpRect.setHeight(fRadius * 2);
        painter.drawEllipse(tmpRect);
        return;
    }
    QLabel::paintEvent(event);
}

void QIndicationLamp::resizeEvent(QResizeEvent *event)
{
    setPropertyInner();
    QLabel::resizeEvent(event);
}
