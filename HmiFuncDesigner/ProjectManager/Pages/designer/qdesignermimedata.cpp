#include "qdesignermimedata.h"

#include "../../libs/shared/qhostfactory.h"
#include "../../libs/shared/host/qabstracthost.h"


#include <QPainter>
#include <QBitmap>
#include <QPixmap>
#include <QImage>
#include <QLabel>
#include <QDrag>
#include <QCursor>
#include <QDropEvent>
#include <QRgb>
#include <QApplication>

QDesignerDnDItem::QDesignerDnDItem(DropType type) :
    m_type(type),
    m_host(0)
{
}

void QDesignerDnDItem::init( QAbstractHost *host, const QPoint &global_mouse_pos)
{
    Q_ASSERT(host != 0);
    m_host = host;
    m_hot_spot = global_mouse_pos - ((QWidget*)m_host->getObject())->mapToGlobal(QPoint(0, 0));
}

QDesignerDnDItem::~QDesignerDnDItem()
{
}

QPoint QDesignerDnDItem::hotSpot() const
{
    return m_hot_spot;
}

QAbstractHost *QDesignerDnDItem::host() const
{
    return m_host;
}

QDesignerDnDItem::DropType QDesignerDnDItem::type() const
{
    return m_type;
}

QString QDesignerDnDItem::name()const
{
    return m_name;
}

WidgetBoxDnDItem::WidgetBoxDnDItem(const QString &name, const QPoint &global_mouse_pos) :
    QDesignerDnDItem(CopyDrop)
{
    QAbstractHost *pHostObj = NULL;
    pHostObj = QHostFactory::createHost(name);
    if(pHostObj) {
        m_name = name;
        QWidget* wid = (QWidget*)pHostObj->getObject();
        wid->move(global_mouse_pos - QPoint(5, 5));
        wid->setWindowOpacity(0.5);
        init(pHostObj, global_mouse_pos);
    }
}

WidgetBoxDnDItem::~WidgetBoxDnDItem()
{
    if(m_host != NULL) {
        delete m_host;
    }
}

FormDnItem::FormDnItem(QAbstractHost *host, const QPoint &global_mouse_pos):
    QDesignerDnDItem(MoveDrop)
{
    init(host, global_mouse_pos);
}


QDesignerMimeData::QDesignerMimeData(const QDesignerDnDItems &items, QDrag *drag) :
    m_items(items)
{
    enum { Alpha = 200 };
    QPoint decorationTopLeft;
    switch (m_items.size()) {
        case 0:
            break;
        case 1: {
            QWidget *deco = (QWidget*)m_items.first()->host()->getObject();
            decorationTopLeft = deco->pos();
            const QPixmap widgetPixmap = QPixmap::grabWidget(deco);
            QImage image(widgetPixmap.size(), QImage::Format_ARGB32);
            image.fill(QColor(Qt::transparent).rgba());
            QPainter painter(&image);
            painter.drawPixmap(QPoint(0, 0), widgetPixmap);
            painter.end();
            setImageTransparency(image, Alpha);
            drag->setPixmap(QPixmap::fromImage(image));
        }
        break;
        default: {
            const QDesignerDnDItems::const_iterator cend = m_items.constEnd();
            QDesignerDnDItems::const_iterator it = m_items.constBegin();
            QRect unitedGeometry = ((QWidget*)(*it)->host()->getObject())->geometry();
            for (++it; it != cend; ++it ) {
                unitedGeometry  = unitedGeometry .united(((QWidget*)(*it)->host()->getObject())->geometry());
            }

            // paint with offset. At the same time, create a mask bitmap, containing widget rectangles.
            QImage image(unitedGeometry.size(), QImage::Format_ARGB32);
            image.fill(QColor(Qt::transparent).rgba());
            QBitmap mask(unitedGeometry.size());
            mask.clear();
            // paint with offset, determine action
            QPainter painter(&image);
            QPainter maskPainter(&mask);
            decorationTopLeft = unitedGeometry.topLeft();
            for (it = m_items.constBegin() ; it != cend; ++it ) {
                QWidget *w = (QWidget*)(*it)->host()->getObject();
                const QPixmap wp = QPixmap::grabWidget(w);
                const QPoint pos = w->pos() - decorationTopLeft;
                painter.drawPixmap(pos, wp);
                maskPainter.fillRect(QRect(pos, wp.size()), Qt::color1);
            }
            painter.end();
            maskPainter.end();
            setImageTransparency(image, Alpha);
            QPixmap pixmap = QPixmap::fromImage(image);
            pixmap.setMask(mask);
            drag->setPixmap(pixmap);
        }
        break;
    }
    // determine hot spot and reconstruct the exact starting position as form window
    // introduces some offset when detecting DnD
    m_globalStartPos =  ((QWidget*)m_items.first()->host()->getObject())->pos() +  m_items.first()->hotSpot();
    m_hotSpot = m_globalStartPos - decorationTopLeft;
    drag->setHotSpot(m_hotSpot);

    drag->setMimeData(this);
}

QDesignerMimeData::~QDesignerMimeData()
{
    const QDesignerDnDItems::const_iterator cend = m_items.constEnd();
    for (QDesignerDnDItems::const_iterator it = m_items.constBegin(); it != cend; ++it ) {
        delete *it;
    }
}

Qt::DropAction QDesignerMimeData::proposedDropAction() const
{
    return m_items.first()->type() == QDesignerDnDItemInterface::CopyDrop ? Qt::CopyAction : Qt::MoveAction;
}

Qt::DropAction QDesignerMimeData::execDrag(const QDesignerDnDItems &items, QWidget * dragSource)
{
    if (items.empty()) {
        return Qt::IgnoreAction;
    }

    QDrag *drag = new QDrag(dragSource);
    QDesignerMimeData *mimeData = new QDesignerMimeData(items, drag);

    QWidgetList reshowWidgets;
    const QDesignerDnDItems::const_iterator cend = items.constEnd();
    for (QDesignerDnDItems::const_iterator it = items.constBegin(); it != cend; ++it )
        if (QWidget *w = (QWidget*)(*it)->host()->getObject())
            if ((*it)->type() ==  QDesignerDnDItemInterface::MoveDrop) {
                reshowWidgets.push_back(w);
            }

    const Qt::DropAction executedAction = drag->exec(Qt::CopyAction | Qt::MoveAction, mimeData->proposedDropAction());

    if (executedAction == Qt::IgnoreAction && !reshowWidgets.empty())
        foreach (QWidget *w, reshowWidgets) {
            w->show();
        }

    return executedAction;
}

void QDesignerMimeData::acceptEventWithAction(Qt::DropAction desiredAction, QDropEvent *e)
{
    if (e->proposedAction() == desiredAction) {
        e->acceptProposedAction();
    } else {
        e->setDropAction(desiredAction);
        e->accept();
    }
}

void QDesignerMimeData::acceptEvent(QDropEvent *e) const
{
    acceptEventWithAction(proposedDropAction(), e);
}

void QDesignerMimeData::setImageTransparency(QImage &image, int alpha)
{
    const int height = image.height();
    for (int l = 0; l < height; l++) {
        QRgb *line = reinterpret_cast<QRgb *>(image.scanLine(l));
        QRgb *lineEnd = line + image.width();
        for ( ; line < lineEnd; line++) {
            const QRgb rgba = *line;
            *line = qRgba(qRed(rgba), qGreen(rgba), qBlue(rgba), alpha);
        }
    }
}

void QDesignerMimeData::moveDecoration(const QPoint &globalPos) const
{
    const QPoint relativeDistance = globalPos - m_globalStartPos;
    const QDesignerDnDItems::const_iterator cend = m_items.constEnd();
    for (QDesignerDnDItems::const_iterator it = m_items.constBegin(); it != cend; ++it ) {
        QWidget *w = (QWidget*)(*it)->host()->getObject();
        w->move(w->pos() + relativeDistance);
    }
}
