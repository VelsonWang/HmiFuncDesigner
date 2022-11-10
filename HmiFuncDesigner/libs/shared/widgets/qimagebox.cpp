#include "qimagebox.h"
#include "../qprojectcore.h"
#include <QFileInfo>
#include <QPainter>

QImageBox::QImageBox(QWidget *parent) : QLabel(parent)
{
    m_szImageFile = "";
    m_bNoScale = false;
    m_iBoardWidth = 1;
    m_boardColorObj = Qt::black;

    this->setAlignment(Qt::AlignCenter);
    this->update();
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
        this->update();
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
    this->update();
}

int QImageBox::getBoardWidth()
{
    return m_iBoardWidth;
}

void QImageBox::setBoardWidth(int iWidth)
{
    m_iBoardWidth = iWidth;
    this->update();
}

QColor QImageBox::getBoardColor()
{
    return m_boardColorObj;
}

void QImageBox::setBoardColor(QColor color)
{
    m_boardColorObj = color;
    this->update();
}

void QImageBox::resizeEvent(QResizeEvent *event)
{
    QLabel::resizeEvent(event);
}

void QImageBox::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::HighQualityAntialiasing | QPainter::TextAntialiasing);
    painter.setPen(QPen(m_boardColorObj, m_iBoardWidth, Qt::SolidLine));
    QRect rect(0, 0, this->width()-1, this->height()-1);
    painter.drawRect(rect);
    if(!m_imageObj.isNull()) {
        if(m_bNoScale) {
            QRect imgRect((this->width()-m_imageObj.width())/2,
                          (this->height()-m_imageObj.height())/2,
                          m_imageObj.width(),
                          m_imageObj.height());
            painter.drawImage(imgRect, m_imageObj);
        } else {
            painter.drawImage(rect, m_imageObj.scaled(this->width()-1, this->height()-1));
        }
    }
    QLabel::paintEvent(event);
}
