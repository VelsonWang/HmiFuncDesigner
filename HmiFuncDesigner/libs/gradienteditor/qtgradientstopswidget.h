/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the tools applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QTGRADIENTSTOPSWIDGET_H
#define QTGRADIENTSTOPSWIDGET_H

#include <QAbstractScrollArea>
#include <QMap>
#include <QImage>
#include <QPainter>
#include <QScrollBar>
#include <QMouseEvent>
#include <QRubberBand>
#include <QMenu>

class QtGradientStopsModel;
class QtGradientStopsWidget;
class QtGradientStop;

class QtGradientStopsWidgetPrivate
{
    QtGradientStopsWidget *q_ptr;
    Q_DECLARE_PUBLIC(QtGradientStopsWidget)
public:
    typedef QMap<qreal, QColor> PositionColorMap;
    typedef QMap<QtGradientStop *, qreal> StopPositionMap;

    void slotStopAdded(QtGradientStop *stop);
    void slotStopRemoved(QtGradientStop *stop);
    void slotStopMoved(QtGradientStop *stop, qreal newPos);
    void slotStopsSwapped(QtGradientStop *stop1, QtGradientStop *stop2);
    void slotStopChanged(QtGradientStop *stop, const QColor &newColor);
    void slotStopSelected(QtGradientStop *stop, bool selected);
    void slotCurrentStopChanged(QtGradientStop *stop);
    void slotNewStop();
    void slotDelete();
    void slotFlipAll();
    void slotSelectAll();
    void slotZoomIn();
    void slotZoomOut();
    void slotResetZoom();

    double fromViewport(int x) const;
    double toViewport(double x) const;
    QtGradientStop *stopAt(const QPoint &viewportPos) const;
    QList<QtGradientStop *> stopsAt(const QPoint &viewportPos) const;
    void setupMove(QtGradientStop *stop, int x);
    void ensureVisible(double x); // x = stop position
    void ensureVisible(QtGradientStop *stop);
    QtGradientStop *newStop(const QPoint &viewportPos);

    bool m_backgroundCheckered;
    QtGradientStopsModel *m_model;
    double m_handleSize;
    int m_scaleFactor;
    double m_zoom;

#ifndef QT_NO_DRAGANDDROP
    QtGradientStop *m_dragStop;
    QtGradientStop *m_changedStop;
    QtGradientStop *m_clonedStop;
    QtGradientStopsModel *m_dragModel;
    QColor m_dragColor;
    void clearDrag();
    void removeClonedStop();
    void restoreChangedStop();
    void changeStop(qreal pos);
    void cloneStop(qreal pos);
#endif

    QRubberBand *m_rubber;
    QPoint m_clickPos;

    QList<QtGradientStop *> m_stops;

    bool m_moving;
    int m_moveOffset;
    StopPositionMap m_moveStops;

    PositionColorMap m_moveOriginal;
};

class QtGradientStopsWidget : public QAbstractScrollArea
{
    Q_OBJECT
    Q_PROPERTY(bool backgroundCheckered READ isBackgroundCheckered WRITE setBackgroundCheckered)
public:
    QtGradientStopsWidget(QWidget *parent = 0);
    ~QtGradientStopsWidget();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    bool isBackgroundCheckered() const;
    void setBackgroundCheckered(bool checkered);

    void setGradientStopsModel(QtGradientStopsModel *model);

    void setZoom(double zoom);
    double zoom() const;

signals:

    void zoomChanged(double zoom);

protected:
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseDoubleClickEvent(QMouseEvent *e);
    void keyPressEvent(QKeyEvent *e);
    void focusInEvent(QFocusEvent *e);
    void focusOutEvent(QFocusEvent *e);
    void contextMenuEvent(QContextMenuEvent *e);
    void wheelEvent(QWheelEvent *e);
#ifndef QT_NO_DRAGANDDROP
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dropEvent(QDropEvent *event);
#endif

private:
    QScopedPointer<QtGradientStopsWidgetPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtGradientStopsWidget)
    Q_DISABLE_COPY(QtGradientStopsWidget)
    Q_PRIVATE_SLOT(d_func(), void slotStopAdded(QtGradientStop *stop))
    Q_PRIVATE_SLOT(d_func(), void slotStopRemoved(QtGradientStop *stop))
    Q_PRIVATE_SLOT(d_func(), void slotStopMoved(QtGradientStop *stop, qreal newPos))
    Q_PRIVATE_SLOT(d_func(), void slotStopsSwapped(QtGradientStop *stop1, QtGradientStop *stop2))
    Q_PRIVATE_SLOT(d_func(), void slotStopChanged(QtGradientStop *stop, const QColor &newColor))
    Q_PRIVATE_SLOT(d_func(), void slotStopSelected(QtGradientStop *stop, bool selected))
    Q_PRIVATE_SLOT(d_func(), void slotCurrentStopChanged(QtGradientStop *stop))
    Q_PRIVATE_SLOT(d_func(), void slotNewStop())
    Q_PRIVATE_SLOT(d_func(), void slotDelete())
    Q_PRIVATE_SLOT(d_func(), void slotFlipAll())
    Q_PRIVATE_SLOT(d_func(), void slotSelectAll())
    Q_PRIVATE_SLOT(d_func(), void slotZoomIn())
    Q_PRIVATE_SLOT(d_func(), void slotZoomOut())
    Q_PRIVATE_SLOT(d_func(), void slotResetZoom())
};

#endif
