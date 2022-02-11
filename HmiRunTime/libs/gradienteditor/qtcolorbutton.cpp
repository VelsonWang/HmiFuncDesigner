#include "qtcolorbutton.h"
#include <QColorDialog>
#include <QPainter>
#include <QMimeData>
#include <QDragEnterEvent>
#include <QApplication>
#include <QDrag>


void QtColorButtonPrivate::slotEditColor()
{
    const QColor newColor = QColorDialog::getColor(m_color, q_ptr, QString(), QColorDialog::ShowAlphaChannel);
    if (!newColor.isValid() || newColor == q_ptr->color())
        return;
    q_ptr->setColor(newColor);
    emit q_ptr->colorChanged(m_color);
}

QColor QtColorButtonPrivate::shownColor() const
{
#ifndef QT_NO_DRAGANDDROP
    if (m_dragging)
        return m_dragColor;
#endif
    return m_color;
}

QPixmap QtColorButtonPrivate::generatePixmap() const
{
    QPixmap pix(24, 24);

    int pixSize = 20;
    QBrush br(shownColor());

    QPixmap pm(2 * pixSize, 2 * pixSize);
    QPainter pmp(&pm);
    pmp.fillRect(0, 0, pixSize, pixSize, Qt::lightGray);
    pmp.fillRect(pixSize, pixSize, pixSize, pixSize, Qt::lightGray);
    pmp.fillRect(0, pixSize, pixSize, pixSize, Qt::darkGray);
    pmp.fillRect(pixSize, 0, pixSize, pixSize, Qt::darkGray);
    pmp.fillRect(0, 0, 2 * pixSize, 2 * pixSize, shownColor());
    br = QBrush(pm);

    QPainter p(&pix);
    int corr = 1;
    QRect r = pix.rect().adjusted(corr, corr, -corr, -corr);
    p.setBrushOrigin((r.width() % pixSize + pixSize) / 2 + corr, (r.height() % pixSize + pixSize) / 2 + corr);
    p.fillRect(r, br);

    p.fillRect(r.width() / 4 + corr, r.height() / 4 + corr,
               r.width() / 2, r.height() / 2,
               QColor(shownColor().rgb()));
    p.drawRect(pix.rect().adjusted(0, 0, -1, -1));

    return pix;
}

///////////////

QtColorButton::QtColorButton(QWidget *parent)
    : QToolButton(parent), d_ptr(new QtColorButtonPrivate)
{
    d_ptr->q_ptr = this;
    d_ptr->m_dragging = false;
    d_ptr->m_backgroundCheckered = true;

    setAcceptDrops(true);

    connect(this, SIGNAL(clicked()), this, SLOT(slotEditColor()));
    setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
}

QtColorButton::~QtColorButton()
{
}

void QtColorButton::setColor(const QColor &color)
{
    if (d_ptr->m_color == color)
        return;
    d_ptr->m_color = color;
    update();
}

QColor QtColorButton::color() const
{
    return d_ptr->m_color;
}

void QtColorButton::setBackgroundCheckered(bool checkered)
{
    if (d_ptr->m_backgroundCheckered == checkered)
        return;
    d_ptr->m_backgroundCheckered = checkered;
    update();
}

bool QtColorButton::isBackgroundCheckered() const
{
    return d_ptr->m_backgroundCheckered;
}

void QtColorButton::paintEvent(QPaintEvent *event)
{
    QToolButton::paintEvent(event);
    if (!isEnabled())
        return;

    const int pixSize = 10;
    QBrush br(d_ptr->shownColor());
    if (d_ptr->m_backgroundCheckered) {
        QPixmap pm(2 * pixSize, 2 * pixSize);
        QPainter pmp(&pm);
        pmp.fillRect(0, 0, pixSize, pixSize, Qt::white);
        pmp.fillRect(pixSize, pixSize, pixSize, pixSize, Qt::white);
        pmp.fillRect(0, pixSize, pixSize, pixSize, Qt::black);
        pmp.fillRect(pixSize, 0, pixSize, pixSize, Qt::black);
        pmp.fillRect(0, 0, 2 * pixSize, 2 * pixSize, d_ptr->shownColor());
        br = QBrush(pm);
    }

    QPainter p(this);
    const int corr = 4;
    QRect r = rect().adjusted(corr, corr, -corr, -corr);
    p.setBrushOrigin((r.width() % pixSize + pixSize) / 2 + corr, (r.height() % pixSize + pixSize) / 2 + corr);
    p.fillRect(r, br);


    const QColor frameColor1(0, 0, 0, 26);
    p.setPen(frameColor1);
    p.drawRect(r.adjusted(1, 1, -2, -2));
    const QColor frameColor2(0, 0, 0, 51);
    p.setPen(frameColor2);
    p.drawRect(r.adjusted(0, 0, -1, -1));
}

void QtColorButton::mousePressEvent(QMouseEvent *event)
{
#ifndef QT_NO_DRAGANDDROP
    if (event->button() == Qt::LeftButton)
        d_ptr->m_dragStart = event->pos();
#endif
    QToolButton::mousePressEvent(event);
}

void QtColorButton::mouseMoveEvent(QMouseEvent *event)
{
#ifndef QT_NO_DRAGANDDROP
    if (event->buttons() & Qt::LeftButton &&
            (d_ptr->m_dragStart - event->pos()).manhattanLength() > QApplication::startDragDistance()) {
        QMimeData *mime = new QMimeData;
        mime->setColorData(color());
        QDrag *drg = new QDrag(this);
        drg->setMimeData(mime);
        drg->setPixmap(d_ptr->generatePixmap());
        setDown(false);
        event->accept();
        drg->start();
        return;
    }
#endif
    QToolButton::mouseMoveEvent(event);
}

#ifndef QT_NO_DRAGANDDROP
void QtColorButton::dragEnterEvent(QDragEnterEvent *event)
{
    const QMimeData *mime = event->mimeData();
    if (!mime->hasColor())
        return;

    event->accept();
    d_ptr->m_dragColor = qvariant_cast<QColor>(mime->colorData());
    d_ptr->m_dragging = true;
    update();
}

void QtColorButton::dragLeaveEvent(QDragLeaveEvent *event)
{
    event->accept();
    d_ptr->m_dragging = false;
    update();
}

void QtColorButton::dropEvent(QDropEvent *event)
{
    event->accept();
    d_ptr->m_dragging = false;
    if (d_ptr->m_dragColor == color())
        return;
    setColor(d_ptr->m_dragColor);
    emit colorChanged(color());
}
#endif
