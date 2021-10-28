#include "qfunctioneditor.h"
#include "../../shared/property/qabstractproperty.h"
#include "functioneditordialog.h"
#include <QDialog>


QFunctionEditor::QFunctionEditor(QAbstractProperty *property, QUndoStack* stack, QWidget *parent) :
    QButtonCommonEditor(property, stack, parent)
{
}

void QFunctionEditor::onBtnClicked()
{
    QStringList szListFunc;
    QString szSupportEvents;
    if(m_property) {
        szListFunc = m_property->get_value().toStringList();
        szSupportEvents = m_property->getAttribute("supportevents").toString();
    }
    QStringList szListSupportEvents = szSupportEvents.split("|");
    FunctionEditorDialog dlg(this, szListSupportEvents);
    dlg.setFunctions(szListFunc);
    if(dlg.exec() == QDialog::Accepted) {
        QStringList szListFuncNew = dlg.getFunctions();
        QVariant v(szListFuncNew);
        if(m_property) {
            m_property->notifyEditValue(v);
        }
    }
}


