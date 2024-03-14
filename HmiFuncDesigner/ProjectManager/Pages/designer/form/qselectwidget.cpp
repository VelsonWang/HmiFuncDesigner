#include "qselectwidget.h"
#include <QPainter>
#include <QMouseEvent>


WidgetHandle::WidgetHandle(QWidget *parent, Type t) :
    QWidget(parent),
    m_type(t),
    m_current(false)
{
    setAttribute(Qt::WA_NoChildEventsForParent);
    setMouseTracking(false);
    setAutoFillBackground(true);
    setBackgroundRole(QPalette::Text);
    setFixedSize(6, 6);
    updateCursor();
}

void WidgetHandle::updateCursor()
{
    switch (m_type) {
        case LeftTop:
            setCursor(Qt::SizeFDiagCursor);
            break;
        case Top:
            setCursor(Qt::SizeVerCursor);
            break;
        case RightTop:
            setCursor(Qt::SizeBDiagCursor);
            break;
        case Right:
            setCursor(Qt::SizeHorCursor);
            break;
        case RightBottom:
            setCursor(Qt::SizeFDiagCursor);
            break;
        case Bottom:
            setCursor(Qt::SizeVerCursor);
            break;
        case LeftBottom:
            setCursor(Qt::SizeBDiagCursor);
            break;
        case Left:
            setCursor(Qt::SizeHorCursor);
            break;
        default:
            setCursor(Qt::ArrowCursor);
            break;
    }
}

void WidgetHandle::setCurrent(bool b)
{
    m_current = b;
    this->update();
}

void WidgetHandle::setWidget(QWidget *w)
{
    m_widget = w;
}

void WidgetHandle::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setPen(m_current ? Qt::blue : Qt::black);
    p.setBrush(QBrush(QColor(m_current ? Qt::darkBlue : Qt::black)));
    p.drawRect(0, 0, width() - 1, height() - 1);
}

void WidgetHandle::mousePressEvent(QMouseEvent *e)
{
    e->accept();

    if (!(m_widget && e->button() == Qt::LeftButton)) {
        return;
    }

    QWidget *container = m_widget->parentWidget();

    m_origPressPos = container->mapFromGlobal(e->globalPos());
    m_geom = m_origGeom = m_widget->geometry();
}


void WidgetHandle::mouseMoveEvent(QMouseEvent *e)
{
    if (!(m_widget && e->buttons() & Qt::LeftButton)) {
        return;
    }

    e->accept();

    QWidget *container = m_widget->parentWidget();

    const QPoint rp = container->mapFromGlobal(e->globalPos());
    const QPoint deltaPt = rp - m_origPressPos;

    const QRect pr = container->rect();

    switch (m_type) {
        case LeftTop: {
            if (rp.x() > pr.width() - 2 * width() || rp.y() > pr.height() - 2 * height()) {
                return;
            }
            int w = m_origGeom.width() - deltaPt.x();
            int h = m_origGeom.height() - deltaPt.y();
            m_geom = m_origGeom.adjusted(deltaPt.x(), deltaPt.y(), 0, 0);
            const int dx = m_widget->width() - w;
            const int dy = m_widget->height() - h;
            trySetGeometry(m_widget, m_widget->x() + dx, m_widget->y() + dy, w, h);
        }
        break;
        case Top: {
            if (rp.y() > pr.height() - 2 * height()) {
                return;
            }

            int h = m_origGeom.height() - deltaPt.y();
            m_geom = m_origGeom.adjusted(0, deltaPt.y(), 0, 0);
            const int dy = m_widget->height() - h;
            trySetGeometry(m_widget, m_widget->x(), m_widget->y() + dy, m_widget->width(), h);
        }
        break;
        case RightTop: {
            if (rp.x() < 2 * width() || rp.y() > pr.height() - 2 * height()) {
                return;
            }
            int h = m_origGeom.height() - deltaPt.y();
            const int dy = m_widget->height() - h;
            int w = m_origGeom.width() + deltaPt.x();
            m_geom = m_origGeom.adjusted(0, deltaPt.y(), deltaPt.x(), 0);
            trySetGeometry(m_widget, m_widget->x(), m_widget->y() + dy, w, h);
        }
        break;
        case Right: {
            if (rp.x() < 2 * width()) {
                return;
            }
            int w = m_origGeom.width() + deltaPt.x();
            m_geom = m_origGeom.adjusted(0, 0, deltaPt.x(), 0);
            tryResize(m_widget, w, m_widget->height());
        }
        break;
        case RightBottom: {
            if (rp.x() < 2 * width() || rp.y() < 2 * height()) {
                return;
            }
            int w = m_origGeom.width() + deltaPt.x();
            int h = m_origGeom.height() + deltaPt.y();
            m_geom = m_origGeom.adjusted(0, 0, deltaPt.x(), deltaPt.y());
            tryResize(m_widget, w, h);
        }
        break;
        case Bottom: {
            if (rp.y() < 2 * height()) {
                return;
            }
            int h = m_origGeom.height() + deltaPt.y();
            m_geom = m_origGeom.adjusted(0, 0, 0, deltaPt.y());
            tryResize(m_widget, m_widget->width(), h);
        }
        break;
        case LeftBottom: {
            if (rp.x() > pr.width() - 2 * width() || rp.y() < 2 * height()) {
                return;
            }
            int w = m_origGeom.width() - deltaPt.x();
            int h = m_origGeom.height() + deltaPt.y();
            m_geom = m_origGeom.adjusted(deltaPt.x(), 0, 0, deltaPt.y());
            int dx = m_widget->width() - w;
            trySetGeometry(m_widget, m_widget->x() + dx, m_widget->y(), w, h);
        }
        break;
        case Left: {
            if (rp.x() > pr.width() - 2 * width()) {
                return;
            }
            int w = m_origGeom.width() - deltaPt.x();
            m_geom = m_origGeom.adjusted(deltaPt.x(), 0, 0, 0);
            const int dx = m_widget->width() - w;
            trySetGeometry(m_widget, m_widget->x() + dx, m_widget->y(), w, m_widget->height());
        }
        break;
        default:
            break;
    }
}

void WidgetHandle::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->button() != Qt::LeftButton) {
        return;
    }

    e->accept();
    emit mouse_button_release(m_origGeom, m_geom);
}

void WidgetHandle::trySetGeometry(QWidget *w, int x, int y, int width, int height)
{
    int minw = w->minimumSize().width();
    minw = qMax(minw, 20);

    int minh = w->minimumSize().height();
    minh = qMax(minh, 20);

    if (qMax(minw, width) > w->maximumWidth() || qMax(minh, height) > w->maximumHeight()) {
        return;
    }

    if (width < minw && x != w->x()) {
        x -= minw - width;
    }

    if (height < minh && y != w->y()) {
        y -= minh - height;
    }

    emit sizeChanged(x, y, qMax(minw, width), qMax(minh, height));
}

void WidgetHandle::tryResize(QWidget *w, int width, int height)
{
    int minw = w->minimumSize().width();
    minw = qMax(minw, 16);

    int minh = w->minimumSize().height();
    minh = qMax(minh, 16);

    emit sizeChanged(w->x(), w->y(), qMax(minw, width), qMax(minh, height));
}

WidgetSelection::WidgetSelection(QWidget *parent) :
    m_widget(0),
    m_formWindow(parent)
{
    for (int i = WidgetHandle::LeftTop; i < WidgetHandle::TypeCount; ++i) {
        m_handles[i] = new WidgetHandle(m_formWindow, static_cast<WidgetHandle::Type>(i));
        connect(m_handles[i], SIGNAL(sizeChanged(int, int, int, int)), this, SLOT(changedsize(int, int, int, int)));
        connect(m_handles[i], SIGNAL(mouse_button_release(QRect, QRect)), this, SLOT(mouse_button_release(QRect, QRect)));
    }
    hide();
}

void WidgetSelection::setCurrent(bool b)
{
    for (int i = WidgetHandle::LeftTop; i < WidgetHandle::TypeCount; ++i) {
        m_handles[i]->setCurrent(b);
    }
}

void WidgetSelection::setWidget(QWidget *w)
{
    if (m_widget != 0) {
        QWidget* wid = m_widget;
        while(wid != NULL) {
            wid->removeEventFilter(this);
            wid = wid->parentWidget();
            if(wid == m_formWindow) {
                break;
            }
        }
    }

    if (w == 0) {
        hide();
        m_widget = 0;
        return;
    }

    m_widget = w;

    for (int i = WidgetHandle::LeftTop; i < WidgetHandle::TypeCount; ++i) {
        m_handles[i]->setWidget(m_widget);
    }

    QWidget* wid = m_widget;
    while(wid != NULL) {
        wid->installEventFilter(this);
        wid = wid->parentWidget();
        if(wid == m_formWindow) {
            break;
        }
    }

    updateGeometry();
    show();
}

bool WidgetSelection::isUsed() const
{
    return m_widget != 0;
}

QWidget *WidgetSelection::widget()
{
    return m_widget;
}

void WidgetSelection::updateGeometry()
{
    if (!m_widget || !m_widget->parentWidget()) {
        return;
    }

    QPoint p = m_widget->mapToGlobal(QPoint(0, 0));
    p = m_formWindow->mapFromGlobal(p);
    const QRect r(p, m_widget->size());

    const int w = 6;
    const int h = 6;

    for (int i = WidgetHandle::LeftTop; i < WidgetHandle::TypeCount; ++i) {
        WidgetHandle *hndl = m_handles[ i ];
        if (!hndl) {
            continue;
        }
        switch (i) {
            case WidgetHandle::LeftTop:
                hndl->move(r.x() - w / 2, r.y() - h / 2);
                break;
            case WidgetHandle::Top:
                hndl->move(r.x() + r.width() / 2 - w / 2, r.y() - h / 2);
                break;
            case WidgetHandle::RightTop:
                hndl->move(r.x() + r.width() - w / 2, r.y() - h / 2);
                break;
            case WidgetHandle::Right:
                hndl->move(r.x() + r.width() - w / 2, r.y() + r.height() / 2 - h / 2);
                break;
            case WidgetHandle::RightBottom:
                hndl->move(r.x() + r.width() - w / 2, r.y() + r.height() - h / 2);
                break;
            case WidgetHandle::Bottom:
                hndl->move(r.x() + r.width() / 2 - w / 2, r.y() + r.height() - h / 2);
                break;
            case WidgetHandle::LeftBottom:
                hndl->move(r.x() - w / 2, r.y() + r.height() - h / 2);
                break;
            case WidgetHandle::Left:
                hndl->move(r.x() - w / 2, r.y() + r.height() / 2 - h / 2);
                break;
            default:
                break;
        }
    }
}

void WidgetSelection::hide()
{
    for (int i = WidgetHandle::LeftTop; i < WidgetHandle::TypeCount; ++i) {
        WidgetHandle *h = m_handles[ i ];
        if (h) {
            h->hide();
        }
    }
}

void WidgetSelection::show()
{
    for (int i = WidgetHandle::LeftTop; i < WidgetHandle::TypeCount; ++i) {
        WidgetHandle *h = m_handles[ i ];
        if (h) {
            h->show();
            h->raise();
        }
    }
}

void WidgetSelection::update()
{
    for (int i = WidgetHandle::LeftTop; i < WidgetHandle::TypeCount; ++i) {
        WidgetHandle *h = m_handles[ i ];
        if (h) {
            h->update();
        }
    }
}

bool WidgetSelection::eventFilter(QObject *object, QEvent *event)
{
    QObject *o = m_widget;
    while(1) {
        if(o == object) {
            break;
        }
        o = o->parent();
        if(o == NULL) {
            return false;
        }
    }

    switch (event->type()) {
        default:
            break;

        case QEvent::Move:
        case QEvent::Resize:
        case QEvent::ParentChange:
            updateGeometry();
            break;
        case QEvent::ZOrderChange:
            show();
            break;
    }

    return false;
}

void WidgetSelection::changedsize(int x, int y, int width, int height)
{
    m_widget->setProperty("geometry", QRect(x, y, width, height));
    updateGeometry();
}

void WidgetSelection::mouse_button_release(const QRect &old, const QRect &now)
{
    emit sizeChanged(m_widget, old, now);
}

Selection::Selection(QWidget *formwindow):
    m_current(NULL),
    m_formWindow(formwindow)
{
}

Selection::~Selection()
{
    clearSelectionPool();
}

void Selection::clear()
{
    if (!m_usedSelections.empty()) {
        const SelectionHash::iterator mend = m_usedSelections.end();
        for (SelectionHash::iterator it = m_usedSelections.begin(); it != mend; ++it) {
            it.value()->setWidget(0);
        }
        m_usedSelections.clear();
    }
}

void  Selection::clearSelectionPool()
{
    clear();
    qDeleteAll(m_selectionPool);
    m_selectionPool.clear();
}

WidgetSelection *Selection::addWidget(QWidget *w)
{
    WidgetSelection *rc = m_usedSelections.value(w);
    if (rc != 0) {
        rc->show();
        return rc;
    }
    const SelectionPool::iterator pend = m_selectionPool.end();
    for (SelectionPool::iterator it = m_selectionPool.begin(); it != pend; ++it) {
        if (! (*it)->isUsed()) {
            rc = *it;
            break;
        }
    }

    if (rc == 0) {
        rc = new WidgetSelection(m_formWindow);
        connect(rc, SIGNAL(sizeChanged(QWidget*, QRect, QRect)), this, SIGNAL(sizeChanged(QWidget*, QRect, QRect)));
        m_selectionPool.push_back(rc);

    }
    m_usedSelections.insert(w, rc);
    rc->setWidget(w);
    return rc;
}

QWidget* Selection::removeWidget(QWidget *w)
{
    WidgetSelection *s = m_usedSelections.value(w);
    if (!s) {
        return w;
    }

    if(m_current == s) {
        m_current->setCurrent(false);
        m_current = NULL;
    }
    s->setWidget(0);
    m_usedSelections.remove(w);
    if (m_usedSelections.isEmpty()) {
        return 0;
    }

    return (*m_usedSelections.begin())->widget();
}

void Selection::repaintSelection(QWidget *w)
{
    if (WidgetSelection *s = m_usedSelections.value(w)) {
        s->update();
    }
}

void Selection::repaintSelection()
{
    const SelectionHash::iterator mend = m_usedSelections.end();
    for (SelectionHash::iterator it = m_usedSelections.begin(); it != mend; ++it) {
        it.value()->update();
    }
}

bool Selection::isWidgetSelected(QWidget *w) const
{
    return  m_usedSelections.contains(w);
}

QWidgetList Selection::selectedWidgets() const
{
    return m_usedSelections.keys();
}

void Selection::raiseList(const QWidgetList& l)
{
    const SelectionHash::iterator mend = m_usedSelections.end();
    for (SelectionHash::iterator it = m_usedSelections.begin(); it != mend; ++it) {
        WidgetSelection *w = it.value();
        if (l.contains(w->widget())) {
            w->show();
        }
    }
}

void Selection::raiseWidget(QWidget *w)
{
    if (WidgetSelection *s = m_usedSelections.value(w)) {
        s->show();
    }
}

void Selection::updateGeometry(QWidget *w)
{
    if (WidgetSelection *s = m_usedSelections.value(w)) {
        s->updateGeometry();
    }
}

void Selection::hide(QWidget *w)
{
    if (WidgetSelection *s = m_usedSelections.value(w)) {
        s->hide();
    }
}

void Selection::show(QWidget *w)
{
    if (WidgetSelection *s = m_usedSelections.value(w)) {
        s->show();
    }
}

void Selection::setCurrent(QWidget *w)
{
    WidgetSelection *s = m_usedSelections.value(w);
    if(m_current == s) {
        return;
    }
    if(m_current != NULL) {
        m_current->setCurrent(false);
    }
    m_current = s;
    if(m_current != NULL) {
        m_current->setCurrent(true);
    }
}

QWidget* Selection::current()
{
    return m_current == NULL ? NULL : m_current->widget();
}
