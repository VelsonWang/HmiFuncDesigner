#include "qtagtextlisteditor.h"
#include <QColorDialog>
#include <QStandardPaths>
#include "../../shared/property/qabstractproperty.h"
#include "tagtextlisteditordialog.h"

QTagTextListEditor::QTagTextListEditor(QAbstractProperty *property, QUndoStack* stack, QWidget *parent):
    QButtonCommonEditor(property, stack, parent)
{
}

void QTagTextListEditor::onBtnClicked()
{
    QStringList szListTextList;
    if(m_property) {
        szListTextList = m_property->getValue().toStringList();
    }
    TagTextListEditorDialog dlg;
    dlg.setValueTextList(szListTextList);
    if(dlg.exec() == QDialog::Accepted) {
        QStringList szListTextListNew = dlg.getValueTextList();
        if (szListTextListNew != szListTextList) {
            QVariant v(szListTextListNew);
            if(m_property) {
                m_property->notifyEditValue(v);
            }
        }
    }
}






