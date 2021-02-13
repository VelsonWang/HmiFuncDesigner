#include "qstylesheeteditor.h"

#include "qstylesheetdialog.h"

#include "../../../shared/property/qabstractproperty.h"

QStyleSheetEditor::QStyleSheetEditor(QAbstractProperty *property,QUndoStack* stack, QWidget *parent):
    QButtonCommonEditor(property,stack,parent),
    m_undo_stack(stack)
{
}

void QStyleSheetEditor::onBtnClicked()
{
    QStyleSheetDialog dlg(m_property,m_undo_stack,this);
    dlg.exec();
}
