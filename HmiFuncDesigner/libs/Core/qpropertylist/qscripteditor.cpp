#include "qscripteditor.h"
#include "script_editor/qscripteditdialog.h"
#include "../../shared/property/qabstractproperty.h"


QScriptEditor::QScriptEditor(QAbstractProperty *property,
                             QUndoStack* stack,
                             QWidget *parent) :
    QButtonCommonEditor(property, stack, parent),
    m_undoStack(stack)
{
}

void QScriptEditor::onBtnClicked()
{
    QScriptEditDialog dlg(m_property, m_undoStack, this);
    dlg.exec();
}
