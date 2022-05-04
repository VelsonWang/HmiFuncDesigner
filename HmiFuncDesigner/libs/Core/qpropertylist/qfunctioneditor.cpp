#include "qfunctioneditor.h"
#include "../../shared/property/qabstractproperty.h"
#include "function/functioneditordialog.h"
#include <QDialog>


QFunctionEditor::QFunctionEditor(QAbstractProperty *property, QUndoStack* stack, QWidget *parent) :
    QButtonCommonEditor(property, stack, parent)
{
}

void QFunctionEditor::onBtnClicked()
{
    QStringList szListFunc;
    if(m_property) {
        szListFunc = m_property->getValue().toStringList();
    }
    FunctionEditorDialog dlg(this);
    dlg.setFunctions(szListFunc);
    if(dlg.exec() == QDialog::Accepted) {
        QStringList szListFuncNew = dlg.getFunctions();
        QVariant v(szListFuncNew);
        if(m_property) {
            m_property->notifyEditValue(v);
        }
    }
}


