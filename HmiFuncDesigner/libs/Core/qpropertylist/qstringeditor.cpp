#include "qstringeditor.h"
#include "qstringeditdialog.h"
#include "../../shared/property/qabstractproperty.h"

QStringEditor::QStringEditor(QAbstractProperty *property, QUndoStack* stack, QWidget *parent):
    QButtonCommonEditor(property, stack, parent),
    m_undo_stack(stack)
{
}

void QStringEditor::onBtnClicked()
{
    QStringEditDialog dlg(m_property, m_undo_stack, this);
    dlg.exec();
}
