#include "qform.h"
#include <QPainter>

QForm::QForm(QWidget *parent) : QWidget(parent)
{
    backgroundColor = QColor();
}

QStringList QForm::getOpenFuncs() const
{
    return openFuncs;
}

void QForm::setOpenFuncs(const QStringList &value)
{
    if(value != openFuncs) {
        openFuncs = value;
        this->update();
    }
}

QStringList QForm::getCloseFuncs() const
{
    return closeFuncs;
}

void QForm::setCloseFuncs(const QStringList &value)
{
    if(value != closeFuncs) {
        closeFuncs = value;
        this->update();
    }
}

QColor QForm::getBackgroundColor()
{
    return backgroundColor;
}

void QForm::setBackgroundColor(QColor color)
{
    backgroundColor = color;
    this->update();
}

void QForm::paintEvent(QPaintEvent *event)
{
    if(backgroundColor.isValid()) {
        QPainter painter(this);
        painter.setRenderHints(QPainter::HighQualityAntialiasing | QPainter::TextAntialiasing);
        QRect rect(0, 0, this->width(), this->height());
        painter.fillRect(rect, backgroundColor);
    }
    QWidget::paintEvent(event);
}
