#include "qlineeditex.h"
#include <QDebug>

QLineEditEx::QLineEditEx(QWidget *parent) : QLineEdit(parent)
{
    m_tagId = "";
    m_tag = NULL;
}

void QLineEditEx::paintEvent(QPaintEvent *event)
{
    if(m_tag) {
        this->setText(m_tag->toString());
    } else {
        m_tag = RealTimeDB::instance()->tag(tagId(m_tagId));
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
