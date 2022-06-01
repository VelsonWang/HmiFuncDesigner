#include "qimagebox.h"
#include "../qprojectcore.h"
#include "../xmlobject.h"
#include <QFileInfo>
#include <QPainter>
#include <QPixmap>

QImageBox::QImageBox(QWidget *parent) : QLabel(parent)
{
    m_szImageFile = "";
    m_bNoScale = false;
    m_iBoardWidth = 1;
    m_boardColorObj = Qt::black;

    this->setAlignment(Qt::AlignCenter);
    setPropertyInner();
}

void QImageBox::fromObject(XMLObject* xml)
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
                    } else if(propertyName == "ImageFile") {
                        this->setProperty("ImageFile", pObj->getProperty("value"));
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

QString QImageBox::getImageFile()
{
    return m_szImageFile;
}

void QImageBox::setImageFile(const QString szName)
{
    QStringList szListInfo = szName.split("|"); // 图片名称|图片BASE64数据
    if(szListInfo.size() == 2) {
        QFileInfo info(szListInfo.at(0));
        m_imageObj = PictureResourceManager::base64ToImage(szListInfo.at(1).toLocal8Bit(), info.suffix());
        setPropertyInner();
    }
    m_szImageFile = szName;
}

bool QImageBox::isNoScale()
{
    return m_bNoScale;
}

void QImageBox::setNoScale(bool bNoScale)
{
    m_bNoScale = bNoScale;
    setPropertyInner();
}

int QImageBox::getBoardWidth()
{
    return m_iBoardWidth;
}

void QImageBox::setBoardWidth(int iWidth)
{
    m_iBoardWidth = iWidth;
    setPropertyInner();
}

QColor QImageBox::getBoardColor()
{
    return m_boardColorObj;
}

void QImageBox::setBoardColor(QColor color)
{
    m_boardColorObj = color;
    setPropertyInner();
}

void QImageBox::setPropertyInner()
{
    setScaledContents(!m_bNoScale);
    if(!m_bNoScale && (m_szImageFile != "")) {
        setPixmap(QPixmap::fromImage(m_imageObj));
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

void QImageBox::resizeEvent(QResizeEvent *event)
{
    setPropertyInner();
    QLabel::resizeEvent(event);
}
