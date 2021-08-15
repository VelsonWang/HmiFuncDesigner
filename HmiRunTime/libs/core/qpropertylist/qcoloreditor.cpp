#include "qcoloreditor.h"
#include <QColorDialog>
#include <QStandardPaths>
#include "../../shared/property/qabstractproperty.h"

QColorEditor::QColorEditor(QAbstractProperty *property, QUndoStack* stack, QWidget *parent):
    QButtonCommonEditor(property, stack, parent)
{
}

void QColorEditor::onBtnClicked()
{
    QColor color = m_property->get_value().value<QColor>();
    bool ok = false;
    QRgb oldRgba = color.rgba();
    QRgb newRgba = QColorDialog::getRgba(oldRgba, &ok, this);
    if (ok && newRgba != oldRgba) {
        QVariant v;
        v.setValue<QColor>(QColor::fromRgba(newRgba));
        m_property->notifyEditValue(v);
    }
}



