#include "qimageeditor.h"
#include <QFileDialog>
#include <QStandardPaths>
#include "../../shared/property/qabstractproperty.h"
#include "../../shared/projdata/pictureresourcemanager.h"

QImageEditor::QImageEditor(QAbstractProperty *property, QUndoStack* stack, QWidget *parent) :
    QButtonCommonEditor(property, stack, parent)
{
}

void QImageEditor::onBtnClicked()
{
    QString szFilter = "";
    if(property) {
        szFilter = property->getAttribute("filters").toString();
    }
    // 使用Windows自带的文件对话框程序会崩溃
    QString szFileName = QFileDialog::getOpenFileName(this,
                         tr("选择文件"),
                         QStandardPaths::displayName(QStandardPaths::DesktopLocation),
                         szFilter,
                         NULL,
                         QFileDialog::DontUseNativeDialog);
    QFileInfo info(szFileName);
    if(!info.exists()) {
        return;
    }

    QImage imageObj(szFileName);
    QStringList szListData;
    szListData.append(info.fileName());
    szListData.append(PictureResourceManager::imageToBase64(imageObj, info.suffix()));

    if (!szFileName.isEmpty()) {
        QVariant v(szListData.join("|"));
        if(property) {
            property->notifyEditValue(v);
        }
    }
}



