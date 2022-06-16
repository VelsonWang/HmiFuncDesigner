#include "qblinkwidgettimer.h"

QBlinkWidgetTimer::QBlinkWidgetTimer(int interval, QObject *parent)
    : QObject(parent),
      m_interval(interval)
{
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(timeout()));
    m_timer.stop();
}

QBlinkWidgetTimer::~QBlinkWidgetTimer()
{
    m_vectWidgets.clear();
}


void QBlinkWidgetTimer::addWidget(QWidget* pObj)
{
    m_vectWidgets.append(pObj);
    if(!m_timer.isActive()) {
        m_timer.start(m_interval);
    }
}

void QBlinkWidgetTimer::removeWidget(QWidget* pObj)
{
    int index = m_vectWidgets.indexOf(pObj);
    if(index >= 0) {
        m_vectWidgets.remove(index);
        pObj->setVisible(true);
        if(m_vectWidgets.size() == 0) {
            m_timer.stop();
        }
    }
}

void QBlinkWidgetTimer::timeout()
{
    foreach (QWidget* pObj, m_vectWidgets) {
        pObj->setVisible(!pObj->isVisible());
    }
}
