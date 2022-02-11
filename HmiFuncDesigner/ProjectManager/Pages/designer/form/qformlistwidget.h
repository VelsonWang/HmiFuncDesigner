#ifndef QFORMLISTWIDGET_H
#define QFORMLISTWIDGET_H

#include <QWidget>
#include <QMap>
#include <QList>
#include <QUndoStack>

class QDesignerFormHost;
class QAbstractHost;
class QAbstractProperty;

class QFormListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QFormListWidget(QWidget *parent = 0);
    ~QFormListWidget();

    void insert_form(QAbstractHost *host,int index=-1);
    void remove_form(QAbstractHost* host);
    void show_form(QAbstractHost* host);
    void setUndoStack(QUndoStack* stack);
    void clear();
    void set_select(QAbstractHost* host);

public slots:
    void property_edit_slot(QAbstractProperty* pro, const QVariant& value);
    void same_left();
    void same_top();
    void same_right();
    void same_bottom();
    void same_width();
    void same_height();
    void same_geometry();
    void same_v_centre();
    void same_h_centre();

protected slots:
    void insert_page_slot(QAbstractHost *host);
    void remove_page_slot(QAbstractHost *host);

private slots:
    void onSelect(QAbstractHost* host);

signals:
    void select(QAbstractHost* host);

protected:
    QList<QAction*> m_form_action_list;
    QList<QDesignerFormHost*> m_forms;
    QMap<QAbstractHost*, QDesignerFormHost*> m_host_to_form;
    QDesignerFormHost *m_current_form;
    QUndoStack *m_undo_stack;
};

#endif // QFORMLISTWIDGET_H
