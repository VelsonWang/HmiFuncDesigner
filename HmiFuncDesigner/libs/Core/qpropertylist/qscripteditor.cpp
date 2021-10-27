#include "qscripteditor.h"
#include "script_editor/qscripteditdialog.h"
#include "../../shared/property/qabstractproperty.h"


QScriptEditor::QScriptEditor(QAbstractProperty *property,
                             QUndoStack* stack,
                             QWidget *parent) :
    QButtonCommonEditor(property, stack, parent),
    undoStack(stack)
{
}

void QScriptEditor::onBtnClicked()
{
    QScriptEditDialog dlg(property, undoStack, this);
    dlg.exec();
}
