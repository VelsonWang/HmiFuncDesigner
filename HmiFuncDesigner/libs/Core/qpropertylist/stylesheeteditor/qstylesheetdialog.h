#ifndef QSTYLESHEETDIALOG_H
#define QSTYLESHEETDIALOG_H

#include <QDialog>
#include <QUndoStack>
#include <QStackedWidget>
#include <QMap>

class QSheetItemList;
class QAbstractProperty;

class QAbstractStylesheetItem;
class QShowWidget;
class QAbstractHost;

class QBaseEditorWidget;

class QStyleSheetDialog : public QDialog
{
    Q_OBJECT
public:
    explicit QStyleSheetDialog(QAbstractProperty *property,QUndoStack* stack, QWidget *parent = 0);
    ~QStyleSheetDialog();

protected:
    void    add_item(QAbstractStylesheetItem* item);

protected slots:
    void    select_changed(QAbstractStylesheetItem *item);
    void    item_changed();
    void    clear();
    void    clearall();
    void    ok();
    void    add();

    void    remove(QAbstractStylesheetItem* item);
    void    changed(QAbstractStylesheetItem* item);
protected:
    QSheetItemList      *m_item_list;
    QShowWidget         *m_show_widget;
    QAbstractProperty   *m_property;
    QUndoStack          *m_undo_stack;
    QList<QAbstractStylesheetItem*> m_items;
    QStackedWidget      *m_stacked_widget;
    QAbstractHost       *m_host;
    QMap<QBaseEditorWidget*,QAbstractStylesheetItem*>   m_editor_to_item;
    QMap<QAbstractStylesheetItem*,QBaseEditorWidget*>   m_item_to_editor;

};

#endif // QSTYLESHEETDIALOG_H
