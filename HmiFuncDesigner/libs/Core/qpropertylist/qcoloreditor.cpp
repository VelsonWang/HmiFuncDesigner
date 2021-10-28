#include "qcoloreditor.h"
#include <QColorDialog>
#include <QStandardPaths>
#include "../../shared/property/qabstractproperty.h"

QColorEditor::QColorEditor(QAbstractProperty *property, QUndoStack* stack, QWidget *parent) :
    QButtonCommonEditor(property, stack, parent)
{
}

void QColorEditor::onBtnClicked()
{
    QColor oldColor;
    if(m_property) {
        oldColor = m_property->get_value().value<QColor>();
    }
    QColor newColor = QColorDialog::getColor(oldColor, this, tr("选择颜色"));
    if (newColor.isValid() && newColor != oldColor) {
        QVariant v;
        v.setValue<QColor>(newColor);
        if(m_property) {
            m_property->notifyEditValue(v);
        }
    }
}



