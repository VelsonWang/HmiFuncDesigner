#include "QIPAddressEdit.h"
#include "ui_QIPAddressEdit.h"
#include <QRegExp>

QIPAddressEdit::QIPAddressEdit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QIPAddressEdit)
{
    ui->setupUi(this);

    QValidator *validator = new QIntValidator(0, 255, this);

    ui->lineEditIP1->setValidator(validator);
    ui->lineEditIP1->setStyleSheet("border:0px; background: transparent;");
    ui->lineEditIP1->setMaxLength(3);

    ui->lineEditIP2->setValidator(validator);
    ui->lineEditIP2->setStyleSheet("border:0px; background: transparent;");
    ui->lineEditIP2->setMaxLength(3);

    ui->lineEditIP3->setValidator(validator);
    ui->lineEditIP3->setStyleSheet("border:0px; background: transparent;");
    ui->lineEditIP3->setMaxLength(3);

    ui->lineEditIP4->setValidator(validator);
    ui->lineEditIP4->setStyleSheet("border:0px; background: transparent;");
    ui->lineEditIP4->setMaxLength(3);

    QWidget::setTabOrder(ui->lineEditIP1, ui->lineEditIP2);
    QWidget::setTabOrder(ui->lineEditIP2, ui->lineEditIP3);
    QWidget::setTabOrder(ui->lineEditIP3, ui->lineEditIP4);

    ui->lineEditIP1->setFocus();
}

QIPAddressEdit::~QIPAddressEdit()
{
    delete ui;
}

void QIPAddressEdit::setIPString(const QString &ip)
{
    QString ippart1 = "", ippart2 = "", ippart3 = "", ippart4 = "";
    QString szValidate = ip;

    // IP地址验证
    QRegExp regexp("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)");
    QRegExpValidator regexp_validator(regexp, this);
    int nPos = 0;
    QValidator::State state = regexp_validator.validate(szValidate, nPos);
    // IP合法
    if (state == QValidator::Acceptable)
    {
        QStringList ippartlist = ip.split(".");
        if (ippartlist.size() == 4)
        {
            ippart1 = ippartlist.at(0);
            ippart2 = ippartlist.at(1);
            ippart3 = ippartlist.at(2);
            ippart4 = ippartlist.at(3);

            ui->lineEditIP1->setText(ippart1);
            ui->lineEditIP2->setText(ippart2);
            ui->lineEditIP3->setText(ippart3);
            ui->lineEditIP4->setText(ippart4);
        }
    }
}

QString QIPAddressEdit::getIPString() const
{
    // 检查数据

    QString szIP = QString("%1.%2.%3.%4")
            .arg(ui->lineEditIP1->text())
            .arg(ui->lineEditIP2->text())
            .arg(ui->lineEditIP3->text())
            .arg(ui->lineEditIP4->text());
    return szIP;
}

