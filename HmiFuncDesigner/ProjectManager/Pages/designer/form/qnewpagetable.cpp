#include "qnewpagetable.h"

#include "../../../libs/core/stylehelper.h"

#include <QMenu>
#include <QPainter>
#include <QStyleOption>
#include <QMouseEvent>
#include <QHBoxLayout>


static const int MARGIN = 12;
static const int OVERFLOW_DROPDOWN_WIDTH = StyleHelper::navigationWidgetHeight();

static void drawSeparator(QPainter *painter, QPoint top, QPoint bottom)
{
    QLinearGradient grad(top, bottom);
    grad.setColorAt(0, QColor(255, 255, 255, 20));
    grad.setColorAt(0.4, QColor(255, 255, 255, 60));
    grad.setColorAt(0.7, QColor(255, 255, 255, 50));
    grad.setColorAt(1, QColor(255, 255, 255, 40));
    painter->setPen(QPen(grad, 0));
    painter->drawLine(top, bottom);
    grad.setColorAt(0, QColor(0, 0, 0, 30));
    grad.setColorAt(0.4, QColor(0, 0, 0, 70));
    grad.setColorAt(0.7, QColor(0, 0, 0, 70));
    grad.setColorAt(1, QColor(0, 0, 0, 40));
    painter->setPen(QPen(grad, 0));
    painter->drawLine(top - QPoint(1,0), bottom - QPoint(1,0));
}

QNewPageTable::QNewPageTable(QWidget *parent) :
    QWidget(parent),
    m_currentIndex(-1),
    m_lastVisibleIndex(-1),
    m_button(new QToolButton),
    m_stop(false)
{
    m_button->setFixedSize(22,22);
    QHBoxLayout *h=new QHBoxLayout;
    h->setMargin(0);
    h->setSpacing(0);
    h->insertWidget(0,m_button,1,Qt::AlignRight);
    this->setLayout(h);

    m_button->setArrowType(Qt::DownArrow);

    connect(m_button,SIGNAL(clicked()),this,SLOT(button_clicked()));
}

QSize QNewPageTable::minimumSizeHint() const
{
    return QSize(0, 25);
}

void QNewPageTable::insert_data(QString data, int index)
{
    if(index<0 || index>m_tabs.size())
    {
        index=m_tabs.size();
    }
    m_tabs.insert(index,data);
    calc();
    if(m_lastVisibleIndex<m_tabs.size()-1)
    {
        m_button->setVisible(true);
    }
    else
    {
        m_button->setVisible(false);
    }
    update();
}

void QNewPageTable::remove(QString data)
{
    m_tabs.removeAll(data);
    calc();
    if(m_lastVisibleIndex<m_tabs.size()-1)
    {
        m_button->setVisible(true);
    }
    else
    {
        m_button->setVisible(false);
    }
    update();
    if(m_currentIndex>=m_tabs.size())
    {
        m_currentIndex=m_tabs.size()-1;
    }
}

int QNewPageTable::current_index() const
{
    return m_currentIndex;
}

void QNewPageTable::set_current_index(int index)
{
    if (index == m_currentIndex)
    {
        return;
    }

    if(index<0 || index>=m_tabs.size())
    {
        index=0;
    }

    m_currentIndex = index;
    emit current_index_changed(m_currentIndex);

    update();
}

void QNewPageTable::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QRect r = rect();

    r.setHeight(24);

    QPoint offset = window()->mapToGlobal(QPoint(0, 0)) - mapToGlobal(r.topLeft());
    QRect gradientSpan = QRect(offset, window()->size());
    StyleHelper::horizontalGradient(&painter, gradientSpan, r);

    painter.setPen(StyleHelper::borderColor());

    QColor lighter(255, 255, 255, 40);
    painter.drawLine(r.bottomLeft(), r.bottomRight());
    painter.setPen(lighter);
    painter.drawLine(r.topLeft(), r.topRight());

    QFontMetrics fm(font());
    int baseline = (r.height() + fm.ascent()) / 2 - 1;

    QLinearGradient grad(QPoint(0, 0), QPoint(0, r.height() - 1));
    grad.setColorAt(0, QColor(247, 247, 247));
    grad.setColorAt(1, QColor(205, 205, 205));

    // calculate sizes
    QList<int> nameWidth;
    int width = 0;
    int indexSmallerThanOverflow = -1;
    int indexSmallerThanWidth = -1;
    for (int i = 0; i < m_tabs.size(); ++i) {
        int w = fm.width(m_tabs.at(i));
        nameWidth << w;
        width += 2 * MARGIN + w;
        if (width < r.width())
            indexSmallerThanWidth = i;
        if (width < r.width() - OVERFLOW_DROPDOWN_WIDTH)
            indexSmallerThanOverflow = i;
    }

    m_lastVisibleIndex = -1;
    m_currentTabIndices.resize(m_tabs.size());
    if (indexSmallerThanWidth == m_tabs.size() - 1)
    {
        for (int i = 0; i < m_tabs.size(); ++i)
        {
            m_currentTabIndices[i] = i;
        }
        m_lastVisibleIndex = m_tabs.size()-1;
    } else
    {

        if (m_currentIndex <= indexSmallerThanOverflow)
        {
            for (int i = 0; i < m_tabs.size(); ++i)
            {
                m_currentTabIndices[i] = i;
            }
            m_lastVisibleIndex = indexSmallerThanOverflow;
        }
        else
        {
            width = 0;
            int index = 0;
            bool handledCurrentIndex = false;
            for (int i = 0; i < m_tabs.size(); ++i)
            {
                if (index != m_currentIndex) {
                    if (!handledCurrentIndex)
                    {
                        if (width + 2 * MARGIN + nameWidth.at(index)
                                + 2 * MARGIN + nameWidth.at(m_currentIndex)
                                < r.width() - OVERFLOW_DROPDOWN_WIDTH)
                        {
                            m_currentTabIndices[i] = index;
                            ++index;
                            width += 2 * MARGIN + nameWidth.at(index);
                        }
                        else
                        {
                            m_currentTabIndices[i] = m_currentIndex;
                            handledCurrentIndex = true;
                            m_lastVisibleIndex = i;
                        }
                    }
                    else
                    {
                        m_currentTabIndices[i] = index;
                        ++index;
                    }
                }
                else
                {
                    ++index;
                    --i;
                }
            }
        }
    }

    int x=0;
    for (int i = 0; i <= m_lastVisibleIndex; ++i)
    {
        int actualIndex = m_currentTabIndices.at(i);
        QString name=m_tabs.at(actualIndex);
        if (actualIndex == m_currentIndex)
        {
            painter.setPen(StyleHelper::borderColor());
            painter.drawLine(x-1, 0,x-1, r.height() - 1);
            painter.fillRect(QRect(x, 0,
                                   2 * MARGIN + fm.width(name),
                                   r.height() + 1),
                             grad);

            if (actualIndex != 0)
            {
                painter.setPen(QColor(255, 255, 255, 170));
                painter.drawLine(x, 0, x, r.height());
            }
            x += MARGIN;
            painter.setPen(Qt::black);
            painter.drawText(x, baseline, name);
            x += nameWidth.at(actualIndex);
            x += MARGIN;
            painter.setPen(StyleHelper::borderColor());
            painter.drawLine(x, 0, x, r.height() - 1);
            painter.setPen(QColor(0, 0, 0, 20));
            painter.drawLine(x + 1, 0, x + 1, r.height() - 1);
            painter.setPen(QColor(255, 255, 255, 170));
            painter.drawLine(x - 1, 0, x - 1, r.height());
        }
        else
        {
            if (i == 0)
            {
                drawSeparator(&painter, QPoint(x, 0), QPoint(x, r.height()-1));
            }
            x += MARGIN;
            painter.setPen(StyleHelper::panelTextColor());
            painter.drawText(x + 1, baseline,name);
            x += nameWidth.at(actualIndex);
            x += MARGIN;
            drawSeparator(&painter, QPoint(x, 0), QPoint(x, r.height()-1));
        }
    }
}

void QNewPageTable::mousePressEvent(QMouseEvent *event)
{
    if(!m_stop)
    {
        QPair<HitArea, int> hit = convertPosToTab(event->pos());
        if (hit.first == HITTAB)
        {
            if (m_currentIndex != m_currentTabIndices.at(hit.second))
            {
                m_currentIndex = m_currentTabIndices.at(hit.second);
                update();
                event->accept();
                emit current_index_changed(m_currentIndex);
                return;
            }
        }
    }
    event->ignore();
}

QPair<QNewPageTable::HitArea, int> QNewPageTable::convertPosToTab(QPoint pos)
{
    if (pos.y() < StyleHelper::navigationWidgetHeight())
    {
        int eventX = pos.x();
        QFontMetrics fm(font());
        int x = 0;

        if (eventX <= x)
        {
            return qMakePair(HITNOTHING, -1);
        }

        int i;
        for (i = 0; i <= m_lastVisibleIndex; ++i)
        {
            int otherX = x + 2 * MARGIN + fm.width(m_tabs.at(m_currentTabIndices.at(i)));
            if (eventX > x && eventX < otherX)
            {
                break;
            }
            x = otherX;
        }

        if (i <= m_lastVisibleIndex)
        {
            return qMakePair(HITTAB, i);
        }
        else if (m_lastVisibleIndex < m_tabs.size() - 1)
        {
            if (eventX > x && eventX < x + OVERFLOW_DROPDOWN_WIDTH)
            {
                return qMakePair(HITOVERFLOW, -1);
            }
        }
    }
    return qMakePair(HITNOTHING, -1);
}

void QNewPageTable::button_clicked()
{
    QMenu overflowMenu;
    QList<QAction *> actions;
    for (int i = m_lastVisibleIndex + 1; i < m_tabs.size(); ++i)
    {
        actions << overflowMenu.addAction(m_tabs.at(m_currentTabIndices.at(i)));
    }
    if(actions.size()>0)
    {
        QPoint pt=m_button->pos();
        pt=this->mapToGlobal(pt+QPoint(0,24));
        if (QAction *action = overflowMenu.exec(pt))
        {
            int index = m_currentTabIndices.at(actions.indexOf(action) + m_lastVisibleIndex + 1);
            if (m_currentIndex != index)
            {
                m_currentIndex = index;
                update();
                emit current_index_changed(m_currentIndex);
                return;
            }
        }
    }
}

void QNewPageTable::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);
    calc();
    if(m_lastVisibleIndex<m_tabs.size()-1)
    {
        m_button->setVisible(true);
    }
    else
    {
        m_button->setVisible(false);
    }
    update();
}

void QNewPageTable::calc()
{

    QRect r = rect();
    QFontMetrics fm(font());
    r.setHeight(24);

    QList<int> nameWidth;
    int width = 0;
    int indexSmallerThanOverflow = -1;
    int indexSmallerThanWidth = -1;
    for (int i = 0; i < m_tabs.size(); ++i) {
        int w = fm.width(m_tabs.at(i));
        nameWidth << w;
        width += 2 * MARGIN + w;
        if (width < r.width())
            indexSmallerThanWidth = i;
        if (width < r.width() - OVERFLOW_DROPDOWN_WIDTH)
            indexSmallerThanOverflow = i;
    }

    m_lastVisibleIndex = -1;
    m_currentTabIndices.resize(m_tabs.size());
    if (indexSmallerThanWidth == m_tabs.size() - 1)
    {
        for (int i = 0; i < m_tabs.size(); ++i)
        {
            m_currentTabIndices[i] = i;
        }
        m_lastVisibleIndex = m_tabs.size()-1;
    } else
    {

        if (m_currentIndex <= indexSmallerThanOverflow)
        {
            for (int i = 0; i < m_tabs.size(); ++i)
            {
                m_currentTabIndices[i] = i;
            }
            m_lastVisibleIndex = indexSmallerThanOverflow;
        }
        else
        {
            width = 0;
            int index = 0;
            bool handledCurrentIndex = false;
            for (int i = 0; i < m_tabs.size(); ++i)
            {
                if (index != m_currentIndex) {
                    if (!handledCurrentIndex)
                    {
                        if (width + 2 * MARGIN + nameWidth.at(index)
                                + 2 * MARGIN + nameWidth.at(m_currentIndex)
                                < r.width() - OVERFLOW_DROPDOWN_WIDTH)
                        {
                            m_currentTabIndices[i] = index;
                            ++index;
                            width += 2 * MARGIN + nameWidth.at(index);
                        }
                        else
                        {
                            m_currentTabIndices[i] = m_currentIndex;
                            handledCurrentIndex = true;
                            m_lastVisibleIndex = i;
                        }
                    }
                    else
                    {
                        m_currentTabIndices[i] = index;
                        ++index;
                    }
                }
                else
                {
                    ++index;
                    --i;
                }
            }
        }
    }
}

void QNewPageTable::set_stop(bool stop)
{
    m_stop=stop;
    m_button->setEnabled(!m_stop);
}
