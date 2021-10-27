#include "qstringeditor.h"
#include "qstringeditdialog.h"
#include "../../shared/property/qabstractproperty.h"

QStringEditor::QStringEditor(QAbstractProperty *property, QUndoStack* stack, QWidget *parent) :
    QButtonCommonEditor(property, stack, parent),
    undoStack(stack)
{
}

void QStringEditor::onBtnClicked()
{
    QStringEditDialog dlg(property, undoStack, this);
    dlg.exec();
}
