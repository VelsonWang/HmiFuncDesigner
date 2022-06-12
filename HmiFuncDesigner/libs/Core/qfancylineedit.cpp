#include "qfancylineedit.h"
#include <QAbstractItemView>
#include <QApplication>
#include <QDebug>
#include <QDesktopWidget>
#include <QKeyEvent>
#include <QMenu>
#include <QPainter>
#include <QPropertyAnimation>
#include <QStyle>

static void execMenuAtWidget(QMenu *menu, QWidget *widget)
{
    QPoint p;
    QRect screen = qApp->desktop()->availableGeometry(widget);
    QSize sh = menu->sizeHint();
    QRect rect = widget->rect();
    if (widget->isRightToLeft()) {
        if (widget->mapToGlobal(QPoint(0, rect.bottom())).y() + sh.height() <= screen.height()) {
            p = widget->mapToGlobal(rect.bottomRight());
        } else {
            p = widget->mapToGlobal(rect.topRight() - QPoint(0, sh.height()));
        }
        p.rx() -= sh.width();
    } else {
        if (widget->mapToGlobal(QPoint(0, rect.bottom())).y() + sh.height() <= screen.height()) {
            p = widget->mapToGlobal(rect.bottomLeft());
        } else {
            p = widget->mapToGlobal(rect.topLeft() - QPoint(0, sh.height()));
        }
    }
    p.rx() = qMax(screen.left(), qMin(p.x(), screen.right() - sh.width()));
    p.ry() += 1;

    menu->exec(p);
}

enum { margin = 6 };

#define ICONBUTTON_HEIGHT 18
#define FADE_TIME 160

class QFancyLineEditPrivate : public QObject
{
public:
    explicit QFancyLineEditPrivate(QFancyLineEdit *parent);
    virtual bool eventFilter(QObject *obj, QEvent *event);

    QFancyLineEdit  *m_lineEdit;
    QPixmap m_pixmap[2];
    QMenu *m_menu[2];
    bool m_menuTabFocusTrigger[2];
    IconButton *m_iconbutton[2];
    bool m_iconEnabled[2];
};


QFancyLineEditPrivate::QFancyLineEditPrivate(QFancyLineEdit *parent) :
    QObject(parent),
    m_lineEdit(parent)
{
    for (int i = 0; i < 2; ++i) {
        m_menu[i] = 0;
        m_menuTabFocusTrigger[i] = false;
        m_iconbutton[i] = new IconButton(parent);
        m_iconbutton[i]->installEventFilter(this);
        m_iconbutton[i]->hide();
        m_iconbutton[i]->setAutoHide(true);
        m_iconEnabled[i] = false;
    }
}

bool QFancyLineEditPrivate::eventFilter(QObject *obj, QEvent *event)
{
    int buttonIndex = -1;
    for (int i = 0; i < 2; ++i) {
        if (obj == m_iconbutton[i]) {
            buttonIndex = i;
            break;
        }
    }
    if (buttonIndex == -1) {
        return QObject::eventFilter(obj, event);
    }
    switch (event->type()) {
        case QEvent::FocusIn:
            if (m_menuTabFocusTrigger[buttonIndex] && m_menu[buttonIndex]) {
                m_lineEdit->setFocus();
                execMenuAtWidget(m_menu[buttonIndex], m_iconbutton[buttonIndex]);
                return true;
            }
        default:
            break;
    }
    return QObject::eventFilter(obj, event);
}


QFancyLineEdit::QFancyLineEdit(QWidget *parent) :
    QLineEdit(parent),
    d(new QFancyLineEditPrivate(this))
{
    ensurePolished();
    updateMargins();

    connect(this, SIGNAL(textChanged(QString)), this, SLOT(checkButtons(QString)));
    connect(d->m_iconbutton[Left], SIGNAL(clicked()), this, SLOT(iconClicked()));
    connect(d->m_iconbutton[Right], SIGNAL(clicked()), this, SLOT(iconClicked()));

    QIcon icon = QIcon(QLatin1String(":/images/editclear.png"));

    setButtonPixmap(Right, icon.pixmap(16));
}

void QFancyLineEdit::checkButtons(const QString &text)
{
    if (m_oldText.isEmpty() || text.isEmpty()) {
        for (int i = 0; i < 2; ++i) {
            if (d->m_iconbutton[i]->hasAutoHide()) {
                d->m_iconbutton[i]->animateShow(!text.isEmpty());
            }
        }
        m_oldText = text;
    }
}

QFancyLineEdit::~QFancyLineEdit()
{
}

void QFancyLineEdit::setButtonVisible(Side side, bool visible)
{
    d->m_iconbutton[side]->setVisible(visible);
    d->m_iconEnabled[side] = visible;
    updateMargins();
    checkButtons("");
}

bool QFancyLineEdit::isButtonVisible(Side side) const
{
    return d->m_iconEnabled[side];
}

void QFancyLineEdit::iconClicked()
{
    IconButton *button = qobject_cast<IconButton *>(sender());
    int index = -1;
    for (int i = 0; i < 2; ++i)
        if (d->m_iconbutton[i] == button) {
            index = i;
        }
    if (index == -1) {
        return;
    }
    if (d->m_menu[index]) {
        execMenuAtWidget(d->m_menu[index], button);
    } else {
        emit buttonClicked((Side)index);
        if (index == Left) {
            emit leftButtonClicked();
        } else if (index == Right) {
            emit rightButtonClicked();
        }
    }
}

void QFancyLineEdit::updateMargins()
{
    bool leftToRight = (layoutDirection() == Qt::LeftToRight);
    Side realLeft = (leftToRight ? Left : Right);
    Side realRight = (leftToRight ? Right : Left);

    int leftMargin = d->m_iconbutton[realLeft]->pixmap().width() + 8;
    int rightMargin = d->m_iconbutton[realRight]->pixmap().width() + 8;
    // Note KDE does not reserve space for the highlight color
    if (style()->inherits("OxygenStyle")) {
        leftMargin = qMax(24, leftMargin);
        rightMargin = qMax(24, rightMargin);
    }

    QMargins margins((d->m_iconEnabled[realLeft] ? leftMargin : 0), 0,
                     (d->m_iconEnabled[realRight] ? rightMargin : 0), 0);

    setTextMargins(margins);
}

void QFancyLineEdit::updateButtonPositions()
{
    QRect contentRect = rect();
    for (int i = 0; i < 2; ++i) {
        Side iconpos = (Side)i;
        if (layoutDirection() == Qt::RightToLeft) {
            iconpos = (iconpos == Left ? Right : Left);
        }

        if (iconpos == QFancyLineEdit::Right) {
            const int iconoffset = textMargins().right() + 4;
            d->m_iconbutton[i]->setGeometry(contentRect.adjusted(width() - iconoffset, 0, 0, 0));
        } else {
            const int iconoffset = textMargins().left() + 4;
            d->m_iconbutton[i]->setGeometry(contentRect.adjusted(0, 0, -width() + iconoffset, 0));
        }
    }
}

void QFancyLineEdit::resizeEvent(QResizeEvent *)
{
    updateButtonPositions();
}

bool QFancyLineEdit::event(QEvent *e)
{
    return QLineEdit::event(e);
}

void QFancyLineEdit::setButtonPixmap(Side side, const QPixmap &buttonPixmap)
{
    d->m_iconbutton[side]->setPixmap(buttonPixmap);
    updateMargins();
    updateButtonPositions();
    update();
}

QPixmap QFancyLineEdit::buttonPixmap(Side side) const
{
    return d->m_pixmap[side];
}

void QFancyLineEdit::setButtonMenu(Side side, QMenu *buttonMenu)
{
    d->m_menu[side] = buttonMenu;
    d->m_iconbutton[side]->setIconOpacity(1.0);
}

QMenu *QFancyLineEdit::buttonMenu(Side side) const
{
    return  d->m_menu[side];
}

bool QFancyLineEdit::hasMenuTabFocusTrigger(Side side) const
{
    return d->m_menuTabFocusTrigger[side];
}

void QFancyLineEdit::setMenuTabFocusTrigger(Side side, bool v)
{
    if (d->m_menuTabFocusTrigger[side] == v) {
        return;
    }

    d->m_menuTabFocusTrigger[side] = v;
    d->m_iconbutton[side]->setFocusPolicy(v ? Qt::TabFocus : Qt::NoFocus);
}

bool QFancyLineEdit::hasAutoHideButton(Side side) const
{
    return d->m_iconbutton[side]->hasAutoHide();
}

void QFancyLineEdit::setAutoHideButton(Side side, bool h)
{
    d->m_iconbutton[side]->setAutoHide(h);
    if (h) {
        d->m_iconbutton[side]->setIconOpacity(text().isEmpty() ?  0.0 : 1.0);
    } else {
        d->m_iconbutton[side]->setIconOpacity(1.0);
    }
}

void QFancyLineEdit::setButtonToolTip(Side side, const QString &tip)
{
    d->m_iconbutton[side]->setToolTip(tip);
}

void QFancyLineEdit::setButtonFocusPolicy(Side side, Qt::FocusPolicy policy)
{
    d->m_iconbutton[side]->setFocusPolicy(policy);
}


IconButton::IconButton(QWidget *parent)
    : QAbstractButton(parent), m_autoHide(true)
{
    setCursor(Qt::ArrowCursor);
    setFocusPolicy(Qt::NoFocus);
}

void IconButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QRect pixmapRect = QRect(0, 0, m_pixmap.width(), m_pixmap.height());
    pixmapRect.moveCenter(rect().center());

    if (m_autoHide) {
        painter.setOpacity(m_iconOpacity);
    }

    painter.drawPixmap(pixmapRect, m_pixmap);
}

void IconButton::animateShow(bool visible)
{
    if (visible) {
        QPropertyAnimation *animation = new QPropertyAnimation(this, "iconOpacity");
        animation->setDuration(FADE_TIME);
        animation->setEndValue(1.0);
        animation->start(QAbstractAnimation::DeleteWhenStopped);
    } else {
        QPropertyAnimation *animation = new QPropertyAnimation(this, "iconOpacity");
        animation->setDuration(FADE_TIME);
        animation->setEndValue(0.0);
        animation->start(QAbstractAnimation::DeleteWhenStopped);
    }
}
