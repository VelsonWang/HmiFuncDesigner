#include "SaveStrategyDialog.h"
#include <QtDebug>

SaveStrategyDialog::SaveStrategyDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    setWindowTitle(trUtf8("保存类型选择"));
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    listWidget->setCurrentRow(0);
    teDescription->setText(trUtf8("保存画面的格式为XML扩展 .mdscreenxml"));

    connect(pbChoose, SIGNAL(clicked()), SLOT(slotChoose()));
    connect(pbCancel, SIGNAL(clicked()), SLOT(slotCancel()));
    connect(listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), SLOT(slotChooseDoubleClicked(QListWidgetItem*)));
    connect(listWidget, SIGNAL(itemClicked(QListWidgetItem*)), SLOT(slotItemClicked(QListWidgetItem*)));
}

SaveGraphPageType SaveStrategyDialog::getLastChoose() const {
    return lastChooseItem;
}

void SaveStrategyDialog::slotChoose() {

    QString itemText = listWidget->currentItem()->text().toUpper();
    if (itemText.contains(trUtf8("XML"), Qt::CaseInsensitive)) {
        lastChooseItem = SAVE_XML;
        accept();
    }
    else if (itemText.contains(trUtf8("BINARY"), Qt::CaseInsensitive)) {
        lastChooseItem = SAVE_BINARY;
        accept();
    }
    else if (itemText.contains(trUtf8("IMAGE"), Qt::CaseInsensitive)) {
        lastChooseItem = SAVE_IMAGE;
        accept();
    }
    else {
        reject();
    }
}

void SaveStrategyDialog::slotCancel() {
    reject();
}

void SaveStrategyDialog::slotChooseDoubleClicked(QListWidgetItem *item) {

    Q_UNUSED(item)

    slotChoose();
}

void SaveStrategyDialog::slotItemClicked(QListWidgetItem *item) {

    QString itemText = item->text().toUpper();
    if (itemText.contains(trUtf8("XML"),Qt::CaseInsensitive)) {
        teDescription->setText(trUtf8("保存画面的格式为 .mdscreenxml"));
    }
    else if (itemText.contains(trUtf8("BINARY"),Qt::CaseInsensitive)) {
        teDescription->setText(trUtf8("保存画面的格式为 .mdscreenbin"));
    }
    else if (itemText.contains(trUtf8("IMAGE"),Qt::CaseInsensitive)) {
        teDescription->setText(trUtf8("保存画面的格式为 IMAGE"));
    }
}
