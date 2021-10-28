#include "qitemcheckbox.h"
#include "ui_qitemcheckbox.h"
#include <QPainter>

QItemCheckBox::QItemCheckBox(const QString &text, bool show, QWidget *parent):
    QWidget(parent),
    ui(new Ui::QItemCheckBox)
{
    ui->setupUi(this);
    ui->checkBox->setText(text);

    if(!show) {
        ui->widget->setVisible(false);
        ui->comboBox->setVisible(false);
    }

    ui->comboBox->setCurrentIndex(0);
    connect(ui->checkBox, SIGNAL(stateChanged(int)), this, SIGNAL(changed()));
    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SIGNAL(changed()));
}

QItemCheckBox::~QItemCheckBox()
{
    delete ui;
}

void QItemCheckBox::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setPen(QColor(128, 128, 128));
    p.setBrush(Qt::transparent);
    p.drawRect(this->rect().adjusted(0, 0, -1, -1));
}

bool QItemCheckBox::get_check()
{
    return ui->checkBox->isChecked();
}

bool QItemCheckBox::get_yes()
{
    return ui->comboBox->currentIndex() == 0;
}

void QItemCheckBox::set_checked(bool checked)
{
    ui->checkBox->setChecked(checked);
}

void QItemCheckBox::set_yes(bool yes)
{
    ui->comboBox->setCurrentIndex(yes ? 0 : 1);
}
