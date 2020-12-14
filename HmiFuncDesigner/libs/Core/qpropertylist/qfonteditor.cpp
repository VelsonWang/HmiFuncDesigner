#include "qfonteditor.h"

#include "../../shared/property/qabstractproperty.h"

#include <QFontDialog>

QFontEditor::QFontEditor(QAbstractProperty *property,QUndoStack* stack, QWidget *parent):
    QButtonCommonEditor(property,stack,parent)
{
}

void QFontEditor::btn_clicked()
{
    QFont f=m_property->get_value().value<QFont>();

    bool ok;
    QFont font=QFontDialog::getFont(&ok,f,this,tr("Set Font"));
    if(ok)
    {
        if(f!=font)
        {
            QVariant v;
            v.setValue<QFont>(font);
            m_property->emit_edit_value(v);
        }
    }
}
