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
    QStringList szListColorList;
    if(m_property) {
        szListColorList = m_property->getValue().toStringList();
    }
    TagColorListEditorDialog dlg;
    dlg.setValueColorList(szListColorList);
    if(dlg.exec() == QDialog::Accepted) {
        QStringList szListColorListNew = dlg.getValueColorList();
        if (szListColorListNew != szListColorList) {
            QVariant v(szListColorListNew);
            if(m_property) {
                m_property->notifyEditValue(v);
            }
        }
    }
}






