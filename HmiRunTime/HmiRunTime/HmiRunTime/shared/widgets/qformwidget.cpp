#include "qformwidget.h"
#include "../xmlobject.h"
#include <QPainter>

#define FORM_TITLE "form"

QFormWidget::QFormWidget(QWidget *parent) : QWidget(parent)
{
    setProperty("title", FORM_TITLE);
    backgroundColor = QColor();
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
                        this->setProperty("background", pObj->getProperty("value"));
                    } else if(propertyName == "openFuncs") {
                        this->setProperty("openFuncs", praseFunctions(pObj->getProperty("value")));
                    } else if(propertyName == "closeFuncs") {
                        this->setProperty("closeFuncs", praseFunctions(pObj->getProperty("value")));
                    }
                }
            }
        }
    }
}

QStringList QFormWidget::getOpenFuncs() const
{
    return openFuncs;
}

void QFormWidget::setOpenFuncs(const QStringList &value)
{
    if(value != openFuncs) {
        openFuncs = value;
        this->update();
    }
}

QStringList QFormWidget::getCloseFuncs() const
{
    return closeFuncs;
}

void QFormWidget::setCloseFuncs(const QStringList &value)
{
    if(value != closeFuncs) {
        closeFuncs = value;
        this->update();
    }
}

void QFormWidget::showEvent(QShowEvent *event)
{
    (void)event;
    execFunction(openFuncs);
}

void QFormWidget::hideEvent(QHideEvent *event)
{
    (void)event;
    execFunction(closeFuncs);
}

QColor QFormWidget::getBackgroundColor()
{
    return backgroundColor;
}

void QFormWidget::setBackgroundColor(QColor color)
{
    backgroundColor = color;
    this->update();
}

void QFormWidget::paintEvent(QPaintEvent *event)
{
    if(backgroundColor.isValid()) {
        QPainter painter(this);
        painter.setRenderHints(QPainter::HighQualityAntialiasing | QPainter::TextAntialiasing);
        QRect rect(0, 0, this->width(), this->height());
        painter.fillRect(rect, backgroundColor);
    }
    QWidget::paintEvent(event);
}
