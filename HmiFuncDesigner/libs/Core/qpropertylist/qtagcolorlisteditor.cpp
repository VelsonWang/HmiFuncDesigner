#include "qtagcolorlisteditor.h"
#include <QColorDialog>
#include <QStandardPaths>
#include "../../shared/property/qabstractproperty.h"
#include "tagcolorlisteditordialog.h"

QTagColorListEditor::QTagColorListEditor(QAbstractProperty *property, QUndoStack* stack, QWidget *parent):
    QButtonCommonEditor(property, stack, parent)
{
}

void QTagColorListEditor::onBtnClicked()
{
    QStringList szListColorList = m_property->get_value().toStringList();
    TagColorListEditorDialog dlg;
    dlg.setValueColorList(szListColorList);
    if(dlg.exec() == QDialog::Accepted) {
        QStringList szListColorListNew = dlg.getValueColorList();
        if (szListColorListNew != szListColorList) {
            QVariant v(szListColorListNew);
            m_property->notifyEditValue(v);
        }
    }
}






