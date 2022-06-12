#ifndef QNEWPAGEDIALOG_H
#define QNEWPAGEDIALOG_H

#include <QDialog>
#include <QStackedWidget>
#include <QUndoStack>

namespace Ui {
class QNewPageDialog;
}

class QNewPageTable;
class QPageWidget;
class QAbstractHost;

class QNewPageDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit QNewPageDialog(QUndoStack* stack,QWidget *parent = 0);
    ~QNewPageDialog();

private slots:
    void ok(QAbstractHost* host);

private:
    Ui::QNewPageDialog *ui;

protected:
    QNewPageTable *m_tab_bar;
    QStackedWidget *m_stacked_widget;
    QList<QPageWidget*> m_widget_list;
    QUndoStack *m_undo_stack;
};

#endif // QNEWPAGEDIALOG_H
