#include "qtoolbarbutton.h"
#include "stylehelper.h"
#include <QPropertyAnimation>
#include <QPainter>
#include <QEvent>
#include <QAction>

QToolBarButton::QToolBarButton(QAction *action, QWidget *parent)
    : QToolButton(parent),
      m_fader(0)
{
    setDefaultAction(action);
    setAttribute(Qt::WA_Hover, true);
    if(action->isSeparator()) {
        //分隔符的大小
        setFixedSize(6, 22);
    } else {
        //不是分隔符的大小
        setFixedSize(22, 22);
    }

}

void QToolBarButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    QRect re = rect();
    QAction *ac = defaultAction();
    if(ac->isSeparator()) {
        //绘制分隔符
        QLinearGradient grad(re.topRight(), re.bottomRight());
        grad.setColorAt(0, QColor(255, 255, 255, 20));
        grad.setColorAt(0.4, QColor(255, 255, 255, 60));
        grad.setColorAt(0.7, QColor(255, 255, 255, 50));
        grad.setColorAt(1, QColor(255, 255, 255, 40));
        painter.setPen(QPen(grad, 0));
        painter.drawLine(re.center() - QPoint(0, re.height() / 2), re.center() + QPoint(0, re.height() / 2));
        grad.setColorAt(0, QColor(0, 0, 0, 30));
        grad.setColorAt(0.4, QColor(0, 0, 0, 70));
        grad.setColorAt(0.7, QColor(0, 0, 0, 70));
        grad.setColorAt(1, QColor(0, 0, 0, 40));
        painter.setPen(QPen(grad, 0));
        painter.drawLine(re.center() - QPoint(-1, re.height() / 2), re.center() + QPoint(1, re.height() / 2));
        return;
    }

    if (m_fader > 0 && isEnabled() && !isDown() && !isChecked()) {
        //绘制半透明方块
        painter.save();
        int fader = int(40 * m_fader);
        painter.fillRect(rect(), QColor(120, 120, 120, 80).lighter(100 + fader * 2));
        painter.restore();
    } else if(isChecked() || isDown()) {
        painter.save();
        painter.fillRect(rect(), QColor(80, 80, 80, 80).darker(180));
        painter.restore();
    }
    QRect iconRect(0, 0, 32, 32);
    iconRect.moveCenter(rect().center());
    //绘制图标
    StyleHelper::drawIconWithShadow(icon(), iconRect, &painter, isEnabled() ? QIcon::Normal : QIcon::Disabled);
}

bool QToolBarButton::event(QEvent *e)
{
    switch (e->type()) {
        case QEvent::Enter: {
            //鼠标进入
            QPropertyAnimation *animation = new QPropertyAnimation(this, "fader");
            animation->setDuration(125);
            animation->setEndValue(1.0);
            animation->start(QAbstractAnimation::DeleteWhenStopped);
        }
        break;
        case QEvent::Leave: {
            //鼠标离开
            QPropertyAnimation *animation = new QPropertyAnimation(this, "fader");
            animation->setDuration(125);
            animation->setEndValue(0.0);
            animation->start(QAbstractAnimation::DeleteWhenStopped);
        }
        break;
        default:
            return QToolButton::event(e);
    }
    return false;
}

QSize QToolBarButton::sizeHint() const
{
    //按钮大小
    QSizeF buttonSize = iconSize().expandedTo(QSize(64, 38));
    return buttonSize.toSize();
}

QSize QToolBarButton::minimumSizeHint() const
{
    //按钮最小大小
    return QSize(6, 6);
}

void QToolBarButton::actionChanged()
{
    //当action属性变化时调用
    if(QAction* action = defaultAction()) {
        setVisible(action->isVisible());
        setCheckable(action->isCheckable());
        setChecked(action->isChecked());
    }
}

float QToolBarButton::fader()
{
    //透明因素
    return m_fader;
}

void QToolBarButton::setFader(float value)
{
    //设置透明因素
    m_fader = value;
    update();
}
