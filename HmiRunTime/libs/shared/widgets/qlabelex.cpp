#include "qlabelex.h"
#include <QDebug>

QLabelEx::QLabelEx(QWidget *parent) : QLabel(parent)
{
    m_tagId = "";
    m_tag = NULL;
}

void QLabelEx::fromObject(XMLObject* xml)
{
    qDebug() << __FILE__ << __LINE__ << __FUNCTION__;
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


