#include "qfunctioneditor.h"
#include "../../shared/property/qabstractproperty.h"
//#include "functioneditordialog.h"
#include <QDialog>

QFunctionEditor::QFunctionEditor(QAbstractProperty *property, QUndoStack* stack, QWidget *parent) :
    QButtonCommonEditor(property, stack, parent)
{
}

void QFunctionEditor::onBtnClicked()
{
//    QStringList szListFunc = m_property->get_value().toStringList();
//    FunctionEditorDialog dlg(Q_NULLPTR, supportEvents_);
//    dlg.setFunctions(szListFunc);
//    if(dlg.exec() == QDialog::Accepted) {
//        QStringList szListFuncNew = dlg.getFunctions();
//        QVariant v(szListFuncNew);
//        m_property->notifyEditValue(v);
//    }
}

/**
 * @brief QFunctionEditor::setSupportEvents
 * @details 设置支持的事件列表
 * @param events 事件列表
 */
void QFunctionEditor::setSupportEvents(QStringList events)
{
    supportEvents_ = events;
}
