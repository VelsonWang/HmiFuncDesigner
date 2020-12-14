#include "qscripteditor.h"

#include "script_editor/qscripteditdialog.h"

#include "../../shared/property/qabstractproperty.h"


QScriptEditor::QScriptEditor(QAbstractProperty *property,QUndoStack* stack, QWidget *parent):
    QButtonCommonEditor(property,stack,parent),
    m_undo_stack(stack)
{
}

void QScriptEditor::btn_clicked()
{
    QScriptEditDialog dlg(m_property,m_undo_stack,this);
    dlg.exec();
}
