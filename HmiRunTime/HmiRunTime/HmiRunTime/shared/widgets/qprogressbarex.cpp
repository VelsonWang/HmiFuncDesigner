#include "qprogressbarex.h"
#include "../qprojectcore.h"
#include "../pubtool.h"
#include <QFileInfo>
#include <QPainter>
#include <QPixmap>
#include <QFont>

QProgressBarEx::QProgressBarEx(QWidget *parent) : QProgressBar(parent)
{
    tagSelected = "";
    m_tag = NULL;
}

QString QProgressBarEx::getTagSelected() const
{
    return tagSelected;
}

void QProgressBarEx::setTagSelected(const QString &value)
{
    if(value != tagSelected) {
        tagSelected = value;
        m_tag = RealTimeDB::instance()->tag(tagId(value));
        this->update();
    }
}

void QProgressBarEx::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::HighQualityAntialiasing | QPainter::TextAntialiasing);

    // 变量当前值
    double dTagValue = 0.0;
    QString szTagValue = "#";

    if(m_tag) {
        szTagValue = m_tag->toString();
    } else {
        m_tag = RealTimeDB::instance()->tag(tagId(tagSelected));
        szTagValue = "#";
    }

    if (szTagValue != "#") {
        bool ok;
        double dVal = szTagValue.toDouble(&ok);
        if (ok) {
            dTagValue = dVal;
        }
    }

    this->setValue((int)dTagValue);
    QProgressBar::paintEvent(event);
}

void QProgressBarEx::fromObject(XMLObject* xml)
{
    if(xml != NULL) {
        if(xml->getTagName() == "Object") {
            QList<XMLObject*> properties = xml->getChildren();
            foreach(XMLObject* pObj, properties) {
                if(pObj->getTagName() == PROPERTY_TITLE) {
                    QString propertyName = pObj->getProperty("name");
                    if(propertyName == "tag") {
                        this->setProperty("tag", pObj->getProperty("value"));
                    } else if(propertyName == "objectName") {
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
                    } else if(propertyName == "maximum") {
                        this->setProperty("maximum", pObj->getProperty("value"));
                    } else if(propertyName == "minimum") {
                        this->setProperty("minimum", pObj->getProperty("value"));
                    } else if(propertyName == "alignment") {
                        Qt::Alignment v = (Qt::Alignment)pObj->getProperty("value").toInt();
                        this->setAlignment(v);
                    } else if(propertyName == "invertedAppearance") {
                        this->setProperty("invertedAppearance", pObj->getProperty("value"));
                    } else if(propertyName == "textVisible") {
                        this->setProperty("textVisible", pObj->getProperty("value"));
                    }
                }
            }
        }
    }
}
