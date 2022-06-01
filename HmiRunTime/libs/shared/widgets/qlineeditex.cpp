#include "qlineeditex.h"
#include "keyboard/inputmethodnumber.h"
#include "../xmlobject.h"
#include <QFocusEvent>
#include <QDebug>

QLineEditEx::QLineEditEx(QWidget *parent) : QLineEdit(parent)
{
    m_tagId = "";
    m_tag = NULL;
    m_editing = false;
}

void QLineEditEx::fromObject(XMLObject* xml)
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
                    } else if(propertyName == "alignment") {
                        this->setProperty("alignment", pObj->getProperty("value"));
                    } else if(propertyName == "readOnly") {
                        this->setProperty("readOnly", pObj->getProperty("value"));
                    } else if(propertyName == "frame") {
                        this->setProperty("frame", pObj->getProperty("value"));
                    } else if(propertyName == "textChanged") {
                        this->setProperty("textChanged", pObj->getProperty("value"));
                    } else if(propertyName == "editFinish") {
                        this->setProperty("editFinish", pObj->getProperty("value"));
                    }
                }
            }
        }
    }
}

void QLineEditEx::paintEvent(QPaintEvent *event)
{
    if(!m_editing) {
        if(m_tag) {
            this->setText(m_tag->toString());
        } else {
            m_tag = RealTimeDB::instance()->tag(tagId(m_tagId));
        }
    }
    QLineEdit::paintEvent(event);
}


QString QLineEditEx::tag()
{
    return m_tagId;
}

void QLineEditEx::setTag(const QString &szTag)
{
    if(m_tagId != szTag) {
        m_tagId = szTag;
        m_tag = RealTimeDB::instance()->tag(tagId(szTag));
    }
}

void QLineEditEx::mousePressEvent(QMouseEvent *)
{
    m_editing = true;
    if(m_tag) {
        this->setText(m_tag->toString());
    } else {
        m_tag = RealTimeDB::instance()->tag(tagId(m_tagId));
    }
    if(tag() != "") {
        connect(InputMethodNumber::instance(), SIGNAL(enterPressed()), this, SLOT(enterPressed()));
        InputMethodNumber::instance()->setFocusWidget(this);
        this->setText(m_tag->toString());
        this->setFocus();
    }
}

void QLineEditEx::enterPressed()
{
    m_editing = false;
    if(tag() != "") {
        m_tag->fromString(this->text(), true);
    }
    disconnect(InputMethodNumber::instance(), SIGNAL(enterPressed()), this, SLOT(enterPressed()));
}







