#include "qformwidget.h"
#include "../xmlobject.h"

#define FORM_TITLE "form"

QFormWidget::QFormWidget(QWidget *parent) : QWidget(parent)
{
    setProperty("title", FORM_TITLE);
}

void QFormWidget::fromObject(XMLObject* xml)
{
    if(xml != NULL) {
        if(xml->getTagName() == FORM_TITLE) {
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
                    } else if(propertyName == "background") {
                        //this->setProperty("background", pObj->getProperty("value"));
                    } else if(propertyName == "function") {
                        //this->setProperty("function", pObj->getProperty("value"));
                    }
                }
            }
        }
    }
}
