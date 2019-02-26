#include "addruledialog.h"
#include "swatch.h"

AddRuleDialog::AddRuleDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle(trUtf8("Новое правило"));
    creaView();

    connect(pbOk,SIGNAL(clicked()),SLOT(accept()));
    connect(pbCancel,SIGNAL(clicked()),SLOT(reject()));
}

void AddRuleDialog::creaView() {

    foreach (const QString &name, QColor::colorNames()) {
        QColor color(name);
        cbBackground->addItem(colorSwatch(color), name, color);
        cbBorderColor->addItem(colorSwatch(color),name,color);
        cbSignColor->addItem(colorSwatch(color),name,color);
    }
}

QString AddRuleDialog::getState() const {

    return leState->text();
}

QString AddRuleDialog::getBackground() const {

    return cbBackground->currentText();
}

QString AddRuleDialog::getText() const {

    return leText->text();
}

QString AddRuleDialog::getBorderColor() const {

    return cbBorderColor->currentText();
}

QString AddRuleDialog::getSignColor() const {

    return cbSignColor->currentText();
}
