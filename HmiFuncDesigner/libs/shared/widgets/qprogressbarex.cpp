#include "qprogressbarex.h"
#include "../qprojectcore.h"
#include "../pubtool.h"
#include <QFileInfo>
#include <QPainter>
#include <QPixmap>
#include <QFont>

QProgressBarEx::QProgressBarEx(QWidget *parent) : QProgressBar(parent)
{

}

QString QProgressBarEx::getTagSelected() const
{
    return tagSelected;
}

void QProgressBarEx::setTagSelected(const QString &value)
{
    if(value != tagSelected) {
        tagSelected = value;
        this->update();
    }
}
