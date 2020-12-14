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
    QDesignerFormHost(QAbstractHost* host,QWidget *parent = 0);
    ~QDesignerFormHost();
    void    show();
    void    hide();
    QAbstractHost   *get_select_widget();
    QAbstractHost   *get_root_host();
    void            set_select_widget(QAbstractHost* host);

    void            property_edited(QAbstractProperty* pro,const QVariant& value);

    void            set_undo_stack(QUndoStack* stack);

    void            same_left();
    void            same_top();
    void            same_right();
    void            same_bottom();
    void            same_width();
    void            same_height();
    void            same_geometry();
    void            same_v_centre();
    void            same_h_centre();
protected:
    bool    eventFilter(QObject *, QEvent *);
    void    install_all_event(QAbstractHost* host);
    void    remove_all_event(QAbstractHost* host);
protected:
    bool    handle_paint_event(QAbstractHost *host,QPaintEvent *e);
    bool    handle_mouse_press_event(QAbstractHost *host,QMouseEvent *e);
    bool    handle_mouse_release_event(QAbstractHost *host,QMouseEvent *e);
    bool    handle_mouse_move_event(QAbstractHost *host,QMouseEvent *e);
    bool    handle_mouse_db_clicked_event(QAbstractHost *host,QMouseEvent *e);

    bool    handle_drop_event(QAbstractHost* host,QDropEvent *e);
    bool    handle_drag_enter_event(QAbstractHost *host,QDragEnterEvent *e);
    bool    handle_drag_leave_event(QAbstractHost *host,QDragLeaveEvent *e);
protected slots:
    void    host_destory();
    void    form_size_changed(const QRect &,const QRect &);
    void    widget_size_changed(QWidget* widget,const QRect &,const QRect &);
    void    insert_host_slot(QList<QAbstractHost*> hosts,QList<int> indexs);
    void    remove_host_slot(QList<QAbstractHost*> hosts);
protected:
    QAbstractHost* host_from_object(QObject* obj);
    void           select_widget(QWidget *wid);
signals:
    void           select(QAbstractHost* host);
protected:
    QAbstractHost       *m_root_host;
    QMap<QObject*,QAbstractHost*>   m_widget_to_host;
    WidgetHost          *m_widget_host;
    QRubberBand     *m_rubber;
    QPoint          m_clickPoint;
    QPoint          m_move_point;
    Selection       *m_selection;
    bool            m_click;
    QUndoStack      *m_undo_stack;
};

#endif // QDESIGNERFORMHOST_H
