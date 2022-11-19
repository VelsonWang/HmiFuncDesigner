#include "qwidgetviewtoolbar.h"
#include "ui_qwidgetviewtoolbar.h"
#include "../../qfancylineedit.h"

QWidgetViewToolBar::QWidgetViewToolBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QWidgetViewToolBar)
{
    ui->setupUi(this);
    m_lineEdit = new QFancyLineEdit(this);
    m_lineEdit->setButtonVisible(QFancyLineEdit::Right, true);
    ui->horizontalLayout->insertWidget(1, m_lineEdit, 1);
    connect(m_lineEdit, SIGNAL(rightButtonClicked()), m_lineEdit, SLOT(clear()));
}

QWidgetViewToolBar::~QWidgetViewToolBar()
{
    delete ui;
}

void QWidgetViewToolBar::on_prevBtn_clicked()
{
    QString str = m_lineEdit->text();
    if(str != "") {
        emit findPrev(str);
    }
}

void QWidgetViewToolBar::on_nextBtn_clicked()
{
    QString str = m_lineEdit->text();
    if(str != "") {
        emit findNext(str);
    }
}

void QWidgetViewToolBar::setLabelText(const QString &text)
{
    ui->label->setText(text);
}
