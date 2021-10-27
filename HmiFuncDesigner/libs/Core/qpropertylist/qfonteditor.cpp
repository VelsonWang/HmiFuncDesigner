#include "qfonteditor.h"
#include "../../shared/property/qabstractproperty.h"
#include <QFontDialog>

QFontEditor::QFontEditor(QAbstractProperty *property, QUndoStack* stack, QWidget *parent) :
    QButtonCommonEditor(property, stack, parent)
{
}

void QFontEditor::onBtnClicked()
{
    QFont oldFont;
    if(property) {
        oldFont = property->get_value().value<QFont>();
    }

    bool ok;
    QFont newFont = QFontDialog::getFont(&ok, oldFont, this, tr("选择字体"));
    if(ok) {
        if(oldFont != newFont) {
            QVariant v;
            v.setValue<QFont>(newFont);
            if(property) {
                property->notifyEditValue(v);
            }
        }
    }
}
