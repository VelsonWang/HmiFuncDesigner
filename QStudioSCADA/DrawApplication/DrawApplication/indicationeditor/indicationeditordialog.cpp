#include "indicationeditordialog.h"
#include "indicationruleparser.h"
#include "addruledialog.h"
#include "deviceeditor/devicemanager.h"
#include <QMessageBox>
#include <QtDebug>

IndicationEditorDialog::IndicationEditorDialog(QWidget *parent) :
    QDialog(parent),unsaved(false)
{
    setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle(trUtf8("Редактор правил индикации мнемознаков"));
    createView();
    createConnections();
    updateUi();
}

void IndicationEditorDialog::createConnections() {

    connect(pbAddRule,SIGNAL(clicked()),SLOT(slotAddRule()));
    connect(pbDeleteRule,SIGNAL(clicked()),SLOT(slotDeleteRule()));
    connect(pbApply,SIGNAL(clicked()),SLOT(slotApply()));
    connect(pbOk,SIGNAL(clicked()),SLOT(slotOk()));
    connect(cbLinkingType,SIGNAL(currentIndexChanged(QString)),SLOT(linkingTypeChanged(QString)));
    connect(cbDeviceLink,SIGNAL(currentIndexChanged(QString)),SLOT(deviceLinkChanged(QString)));
    connect(ruleModel,SIGNAL(changed()),SLOT(slotRuleModelChanged()));
}

void IndicationEditorDialog::createView() {

    ruleModel = new RuleTableModel;
    ruleView = new RuleTableView(ruleModel,this);
    horizontalLayout->addWidget(ruleView);

    teRuleView->setTextColor(QColor(Qt::green));
    teRuleView->setFontPointSize(12);
    teRuleView->setReadOnly(true);

    cbSignalLink->hide();
    lblSignal->hide();

    foreach (Device *dev,DeviceManager::getInstance()->getDevList()) {
        cbDeviceLink->addItem(dev->getName());
    }
}

void IndicationEditorDialog::updateUi() {

    pbApply->setEnabled(unsaved ? true : false);
    pbOk->setEnabled(unsaved ? false : true);
}

void IndicationEditorDialog::slotRuleModelChanged() {

    unsaved = true;
    teRuleView->clear();
    updateUi();
}

void IndicationEditorDialog::slotAddRule() {

    AddRuleDialog addDialog(this);
    addDialog.exec();

    if (addDialog.result() == QDialog::Accepted) {
        Features *f = new Features;
        f->state = addDialog.getState();
        f->background = QColor(addDialog.getBackground());
        f->text = addDialog.getText();
        f->bordercolor = QColor(addDialog.getBorderColor());
        f->signbackground = QColor(addDialog.getSignColor());
        ruleModel->addRule(f);
        ruleList.insert(ruleList.end(),f);

        unsaved = true;
        updateUi();
        createRuleString();
    }
}

void IndicationEditorDialog::slotDeleteRule() {

    ruleModel->deleteRule(ruleView->selectionModel()->currentIndex());
    ruleList = ruleModel->getRuleList();
    unsaved = true;
    updateUi();
}

void IndicationEditorDialog::setCurrentEditElement(Figure *editFigure) {

    editElement = editFigure;
    IndicationRuleParser ruleParser;

    if (ruleParser.setRule(editElement->getIndicationRule())) {

        ruleList = ruleParser.getRuleList();
        QListIterator <Features*> it(ruleList);

        while (it.hasNext()) {
            ruleModel->addRule(it.next());
        }
    }

    cbLinkingType->setCurrentIndex(cbLinkingType->findText(editElement->getLinkingType()));
    cbDeviceLink->setCurrentIndex(cbDeviceLink->findText(editElement->getDeviceLink()));
    cbSignalLink->setCurrentIndex(cbSignalLink->findText(editElement->getSignalLink()));
    createRulePreview(editElement->getIndicationRule());
}

void IndicationEditorDialog::slotApply() {

    unsaved = false;
    updateUi();
    createRuleString();
    editElement->setIndicationRule(ruleStr);
    teRuleView->clear();

    createRulePreview(ruleStr);

    editElement->setLinkingType(cbLinkingType->currentText());
    editElement->setDeviceLink(cbDeviceLink->currentText());
    editElement->setSignalLink(cbSignalLink->currentText());
}

void IndicationEditorDialog::createRulePreview(const QString &mRuleStr) {

    /** Для удобства просмотра получившихся правил*/
    QStringList ruleList = mRuleStr.split(";");
    ruleList.removeLast();
    int i = 1;
    foreach (QString s,ruleList) {
        s.prepend(QString("%1)").arg(i));
        s.append(";");
        teRuleView->append(s);
        i++;
    }
}

void IndicationEditorDialog::slotOk() {
    accept();
}

void IndicationEditorDialog::createRuleString() {

    QListIterator <Features*> it(ruleList);
    ruleStr.clear();

    while (it.hasNext()) {
        Features *f = it.next();
        QString tmp = QString("state = %1 : background = %2, text = %3, bordercolor = %4, signbackground = %5;")
                             .arg(f->state)
                             .arg(f->background.name())
                             .arg(f->text)
                             .arg(f->bordercolor.name())
                             .arg(f->signbackground.name());
        ruleStr.append(tmp);
    }
}

void IndicationEditorDialog::closeEvent(QCloseEvent *event) {

    Q_UNUSED(event)

    if (unsaved) {
        int r = QMessageBox::information(this,trUtf8("Выход"),trUtf8("Данные настройки индикации для элемента "
                                                                 "будут утеряны. Применить данную конфигурацию к элементу?"),
                                                                 QMessageBox::Yes | QMessageBox::No);
        if (r == QMessageBox::Yes) {
            slotApply();
        }
        else if (r == QMessageBox::No) {
            slotOk();
        }
    }
    else {
        slotOk();
    }
}

void IndicationEditorDialog::linkingTypeChanged(const QString &linkingType) {

    if (linkingType == trUtf8("По устройству")) {
        cbSignalLink->hide();
        lblSignal->hide();
    }
    else {
        cbSignalLink->show();
        cbSignalLink->clear();
        lblSignal->show();

        Device *dev = DeviceManager::getInstance()->getDeviceByName(cbDeviceLink->currentText());

        if (dev) {
            Signal *sig;
            foreach (sig, dev->getInputSignalList()) {
                cbSignalLink->addItem(sig->getName());
            }
        }
    }

    unsaved = true;
    updateUi();
}

void IndicationEditorDialog::deviceLinkChanged(const QString &deviceLink) {

    Device *dev = DeviceManager::getInstance()->getDeviceByName(deviceLink);
    cbSignalLink->clear();

    if (dev) {
        Signal *sig;
        foreach (sig, dev->getInputSignalList()) {
            cbSignalLink->addItem(sig->getName());
        }
    }

    unsaved = true;
    updateUi();
}
