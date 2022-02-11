#include "qaddsheetitemdialog.h"
#include "ui_qaddsheetitemdialog.h"
#include "qstatecheckwidget.h"
#include "../../../shared/host/qabstractbuttonhost.h"
#include "../../../shared/property/qabstractproperty.h"

QAddSheetItemDialog::QAddSheetItemDialog(QAbstractProperty *property,
        const QString &title,
        QWidget *parent):
    QDialog(parent),
    ui(new Ui::QAddSheetItemDialog)
{
    ui->setupUi(this);

    QString fir, sec;
    QString s;
    int index;
    if(title != "") {
        s = title;
        s = s.replace("::", "//");
        index = s.indexOf(":");
        fir = title.left(index);
        sec = title.mid(index);
    }
    m_checkWidget = new QStateCheckWidget(property, sec);
    ui->verticalLayout->insertWidget(2, m_checkWidget);
    connect(m_checkWidget, SIGNAL(changed()), this, SLOT(checkChanged()));

    QStringList list = property->getHost()->property("children_widget").toStringList();

    if(list.size() > 0) {
        ui->children_list->addItems(list);
    } else {
        ui->label_3->setVisible(false);
        ui->children_list->setVisible(false);
    }

    list = property->getHost()->property("sub_control").toStringList();

    if(list.size() > 0) {
        list.insert(0, tr("None"));
        ui->sub_control_list->addItems(list);
        if(fir == "") {
            ui->sub_control_list->setCurrentIndex(0);
        } else {
            int in = ui->sub_control_list->findText(fir);
            if(in < 0) {
                ui->sub_control_list->setCurrentIndex(0);
            } else {
                ui->sub_control_list->setCurrentIndex(in);
            }
        }
        connect(ui->sub_control_list, SIGNAL(currentIndexChanged(int)), this, SLOT(makeTitle()));
    } else {
        ui->label_4->setVisible(false);
        ui->sub_control_list->setVisible(false);
    }
}

QAddSheetItemDialog::~QAddSheetItemDialog()
{
    delete ui;
}

void QAddSheetItemDialog::makeTitle()
{
    QString str;
    QString s = ui->sub_control_list->currentText();
    if(s != tr("None")) {
        str += s;
    }
    str += m_checkWidget->getTagName();
    ui->title->setText(str);
}

void QAddSheetItemDialog::checkChanged()
{
    makeTitle();
}

QString QAddSheetItemDialog::getTagName()
{
    return m_title;
}

void QAddSheetItemDialog::on_pushButton_2_clicked()
{
    m_title = ui->title->text();
    close();
}

void QAddSheetItemDialog::on_pushButton_clicked()
{
    close();
}
