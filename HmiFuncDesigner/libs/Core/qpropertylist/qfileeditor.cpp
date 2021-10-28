#include "qfileeditor.h"
#include <QFileDialog>
#include <QStandardPaths>
#include "../../shared/property/qabstractproperty.h"


QFileEditor::QFileEditor(QAbstractProperty *property, QUndoStack* stack, QWidget *parent) :
    QButtonCommonEditor(property, stack, parent)
{
}

void QFileEditor::onBtnClicked()
{
    QString szFilter;
    if(m_property) {
        szFilter = m_property->getAttribute("filters").toString();
    }
    // 使用Windows自带的文件对话框程序会崩溃
    QString szFileName = QFileDialog::getOpenFileName(this,
                         tr("选择文件"),
                         QStandardPaths::displayName(QStandardPaths::DesktopLocation),
                         szFilter,
                         NULL,
                         QFileDialog::DontUseNativeDialog);
    if (!szFileName.isEmpty()) {
        QVariant v(szFileName);
        if(m_property) {
            m_property->notifyEditValue(v);
        }
    }
}



