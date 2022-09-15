#include "qlabelex.h"
#include "../xmlobject.h"
#include <QDebug>

QLabelEx::QLabelEx(QWidget *parent) : QLabel(parent)
{
    m_tagId = "";
    m_tag = NULL;
    this->setProperty("geometry", QRect(0, 0, 80, 20));
    this->setProperty("text", "label");
}

void QLabelEx::fromObject(XMLObject* xml)
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
                    } else if(propertyName == "text") {
                        this->setProperty("text", pObj->getProperty("value"));
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
                    } else if(propertyName == "wordWrap") {
                        this->setProperty("wordWrap", pObj->getProperty("value"));
                    } else if(propertyName == "frameShape") {
                        this->setProperty("frameShape", pObj->getProperty("value").toInt());
                    } else if(propertyName == "alignment") {
                        int hAlignmen, vAlignmen;
                        QList<XMLObject*> tmpProps = pObj->getChildren();
                        foreach(XMLObject* propObj, tmpProps) {
                            QString propertyName = propObj->getProperty("name");
                            if(propertyName == "Horizontal") {
                                hAlignmen = propObj->getProperty("value").toInt();
                            } else if(propertyName == "Vertical") {
                                vAlignmen = propObj->getProperty("value").toInt();
                            }
                        }
                        this->setProperty("alignment", (Qt::AlignmentFlag)(hAlignmen|vAlignmen));
                    } else if(propertyName == "text_sheet") {
                        this->setProperty("text_sheet", pObj->getProperty("value"));
                    }
                }
            }
        }
    }
}

void QLabelEx::paintEvent(QPaintEvent *event)
{
    if(m_tag) {
        this->setText(m_tag->toString());
    } else {
        m_tag = RealTimeDB::instance()->tag(tagId(m_tagId));
    }
    QLabel::paintEvent(event);
}


QString QLabelEx::tag()
{
    return m_tagId;
}

void QLabelEx::setTag(const QString &szTag)
{
    if(m_tagId != szTag) {
        m_tagId = szTag;
        m_tag = RealTimeDB::instance()->tag(tagId(szTag));
    }
}


