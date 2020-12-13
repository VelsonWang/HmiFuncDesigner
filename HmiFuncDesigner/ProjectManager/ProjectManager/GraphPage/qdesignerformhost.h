#ifndef QDESIGNERFORMHOST_H
#define QDESIGNERFORMHOST_H

#include <QObject>
#include <QMap>
#include <QPaintEvent>
#include <QRubberBand>
#include <QUndoStack>

class QAbstractHost;
class WidgetHost;
class Selection;
class QAbstractProperty;

class QDesignerFormHost: public QObject
{
    Q_OBJECT
public:
    QDesignerFormHost(QAbstractHost* host,QWidget *parent = Q_NULLPTR);
    ~QDesignerFormHost();

    void show();
    void hide();

    QAbstractHost* getSelectWidgetObj();
    QAbstractHost* getRootHost();
    void setSelectWidgetObj(QAbstractHost* host);
    void updateGeometry();

    void sameLeft();
    void sameTop();
    void sameRight();
    void sameBottom();
    void sameWidth();
    void sameHeight();
    void sameGeometry();
    void sameVCenter();
    void sameHCenter();

protected:
    bool eventFilter(QObject *, QEvent *);
    void installAllEvent(QAbstractHost* host);
    void removeAllEvent(QAbstractHost* host);

protected:
    bool doPaintEvent(QAbstractHost *host,QPaintEvent *e);
    bool doMousePressEvent(QAbstractHost *host,QMouseEvent *e);
    bool doMouseReleaseEvent(QAbstractHost *host,QMouseEvent *e);
    bool doMouseMoveEvent(QAbstractHost *host,QMouseEvent *e);
    bool doMouseDoubleClickedEvent(QAbstractHost *host,QMouseEvent *e);
    bool doDropEvent(QAbstractHost* host,QDropEvent *e);
    bool doDragEnterEvent(QAbstractHost *host,QDragEnterEvent *e);
    bool doDragLeaveEvent(QAbstractHost *host,QDragLeaveEvent *e);

protected slots:
    void hostDestory();
    void formSizeChanged(const QRect &,const QRect &);
    void widgetSizeChanged(QWidget* widget,const QRect &,const QRect &);
    void insertHostSlot(QList<QAbstractHost*> hosts,QList<int> indexs);
    void removeHostSlot(QList<QAbstractHost*> hosts);

protected:
    QAbstractHost* hostFromObject(QObject* obj);
    void selectWidgetObj(QWidget *wid);

signals:
    void selectObj(QAbstractHost* host);

protected:
    QAbstractHost* m_root_host;
    QMap<QObject*, QAbstractHost*> m_widget_to_host;
    WidgetHost* m_widget_host;
    QRubberBand* m_rubber;
    QPoint m_clickPoint;
    QPoint m_move_point;
    Selection* m_selection;
    bool m_click;
};

#endif // QDESIGNERFORMHOST_H
