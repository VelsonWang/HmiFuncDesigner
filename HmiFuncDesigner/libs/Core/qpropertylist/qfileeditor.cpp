#include "qfileeditor.h"
#include <QFileDialog>
#include <QStandardPaths>
#include "../../shared/property/qabstractproperty.h"

QFileEditor::QFileEditor(QAbstractProperty *property, QUndoStack* stack, QWidget *parent):
    QButtonCommonEditor(property, stack, parent)
{
}

void QFileEditor::onBtnClicked()
{
    QString filePath = QFileDialog::getOpenFileName(this,
                                                    tr("选择文件"),
                                                    QStandardPaths::displayName(QStandardPaths::DesktopLocation),
                                                    "Images (*.png *.xpm *.jpg);;Text files (*.txt)");
    if (filePath.isNull())
        return;
    m_property->notifyEditValue(filePath);
}



