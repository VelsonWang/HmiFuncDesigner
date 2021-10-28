#ifndef QSCRIPTEDITDIALOG_H
#define QSCRIPTEDITDIALOG_H

#include "../../corelibglobal.h"
#include <QDialog>
#include <QMetaMethod>
#include <QStatusBar>
#include <QUndoStack>

class QAbstractProperty;
class QScriptEditView;
class QWidgetView;
class QFunctionView;
class StyledBar;
class QWidgetViewToolBar;
class QFindWidget;

class CORELIB_EXPORT QScriptEditDialog : public QDialog
{
    Q_OBJECT
public:
    explicit QScriptEditDialog(QAbstractProperty* property,
                               QUndoStack* stack,
                               QWidget *parent = 0);

signals:

public slots:
    void save();
    void selectFunction(const QMetaMethod &method);

protected slots:
    void script_find();

protected:
    void keyPressEvent(QKeyEvent *);
    void closeEvent(QCloseEvent *);

protected:
    QScriptEditView *m_editView;
    QWidgetView *m_widgetView;
    QFunctionView *m_functionView;
    StyledBar *m_widgetStyledBar;
    QWidgetViewToolBar *m_widgetToolBar;
    StyledBar *m_functionStyledBar;
    QWidgetViewToolBar *m_functionToolBar;
    StyledBar *m_editStyledBar;
    QAbstractProperty *m_property;
    QFindWidget *m_findWidget;
    QUndoStack *m_undoStack;
};

#endif // QSCRIPTEDITDIALOG_H
