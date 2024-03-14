#ifndef QDESIGNERFORMHOST_H
#define QDESIGNERFORMHOST_H

#include <QObject>
#include <QMap>
#include <QPaintEvent>
#include <QRubberBand>
#include <QUndoStack>
#include <QMenu>
#include <QAction>

class QAbstractHost;
class WidgetHost;
class Selection;
class QAbstractProperty;

class QDesignerFormHost : public QObject
{
    Q_OBJECT
public:
    QDesignerFormHost(QAbstractHost* host, QWidget *parent = 0);
    ~QDesignerFormHost();

    void show();
    void hide();
    QAbstractHost *get_select_widget();
    QAbstractHost *get_root_host();
    void selectHostWidget(QAbstractHost* host);
    void propertyEdited(QAbstractProperty* pro, const QVariant& value);
    void setUndoStack(QUndoStack* stack);
    void onZoom();

    void sameLeft();
    void sameTop();
    void sameRight();
    void sameBottom();
    void sameWidth();
    void sameHeight();
    void sameGeometry();
    void sameVCenter();
    void sameHCenter();
    void horizontalUniformDistribution();
    void verticalUniformDistribution();

protected:
    bool eventFilter(QObject *, QEvent *);
    void installAllEvent(QAbstractHost* host);
    void removeAllEvent(QAbstractHost* host);

protected:
    bool handlePaintEvent(QAbstractHost *host, QPaintEvent *e);
    bool handleMousePressEvent(QAbstractHost *host, QMouseEvent *e);
    bool handleMouseReleaseEvent(QAbstractHost *host, QMouseEvent *e);
    bool handleMouseMoveEvent(QAbstractHost *host, QMouseEvent *e);
    bool handleMouseDoubleClickedEvent(QAbstractHost *host, QMouseEvent *e);
    bool handle_drop_event(QAbstractHost* host, QDropEvent *e);
    bool handle_drag_enter_event(QAbstractHost *host, QDragEnterEvent *e);
    bool handle_drag_leave_event(QAbstractHost *host, QDragLeaveEvent *e);

protected slots:
    void host_destory();
    void form_size_changed(const QRect &, const QRect &);
    void widget_size_changed(QWidget* widget,const QRect &, const QRect &);
    void insert_host_slot(QList<QAbstractHost*> hosts, QList<int> indexs);
    void remove_host_slot(QList<QAbstractHost*> hosts);

protected:
    QAbstractHost* host_from_object(QObject* obj);
    void select_widget(QWidget *wid);

signals:
    void select(QAbstractHost* host);
    void remove(QList<QAbstractHost*> hosts);

private slots:
    void slotMenuAction();

protected:
    QAbstractHost *m_root_host;
    QMap<QObject*, QAbstractHost*> m_widget_to_host;
    WidgetHost *m_widget_host;
    QRubberBand *m_rubber;
    QPoint m_clickPoint;
    QPoint m_move_point;
    Selection *m_selection;
    bool m_click;
    QUndoStack *m_undo_stack;

    QMenu *m_menu = NULL;
    QAction *m_actCopy = NULL; //复制
    QAction *m_actPaste = NULL; //粘贴
    QAction *m_actDelete = NULL; //删除
    QAction *m_actRaise = NULL; // 上一层
    QAction *m_actLower = NULL; // 下一层
};

#endif // QDESIGNERFORMHOST_H
