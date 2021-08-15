#include "qfindwidget.h"
#include "ui_qfindwidget.h"

#include "../../qfancylineedit.h"

QFindWidget::QFindWidget(QWidget *parent) :
    StyledBar(parent),
    ui(new Ui::QFindWidget)
{
    ui->setupUi(this);
    m_findEdit = new QFancyLineEdit(this);
    ui->gridLayout_2->addWidget(m_findEdit, 0, 1, 1, 1);
    m_replaceEdit = new QFancyLineEdit(this);
    ui->verticalLayout->addWidget(m_replaceEdit);
    this->setFixedHeight(45);
    setProperty("topBorder", true);
    m_findEdit->setAttribute(Qt::WA_MacShowFocusRect, false);
    m_replaceEdit->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->close->setIcon(QIcon(QLatin1String(":/images/button_close.png")));
    connect(ui->close, SIGNAL(clicked()), this, SLOT(hideAndResetFocus()));

    m_findEdit->setButtonVisible(QFancyLineEdit::Right,true);
    m_replaceEdit->setButtonVisible(QFancyLineEdit::Right,true);

    connect(m_findEdit,SIGNAL(buttonClicked(QFancyLineEdit::Side)),this,SLOT(clearFindEdit()));
    connect(m_replaceEdit,SIGNAL(buttonClicked(QFancyLineEdit::Side)),this,SLOT(clearReplaceEdit()));
    connect(m_findEdit,SIGNAL(textChanged(QString)),this,SLOT(checkFind()));
    connect(ui->findNextButton,SIGNAL(clicked()),this,SIGNAL(findNext()));
    connect(ui->findPreviousButton,SIGNAL(clicked()),this,SIGNAL(findPrev()));
    connect(ui->replaceButton,SIGNAL(clicked()),this,SLOT(replace_signal()));
    connect(ui->replaceAllButton,SIGNAL(clicked()),this,SLOT(replaceAll_signal()));
    connect(ui->replaceNextButton,SIGNAL(clicked()),this,SLOT(replaceAndNext_signal()));
}

QFindWidget::~QFindWidget()
{
    delete ui;
}

void QFindWidget::hideAndResetFocus()
{
    hide();
    emit find("");
}

void QFindWidget::clearFindEdit()
{
    m_findEdit->clear();
}

void QFindWidget::clearReplaceEdit()
{
    m_replaceEdit->clear();
}

void QFindWidget::checkFind()
{
    emit find(m_findEdit->text());
}

void QFindWidget::showFindWidget()
{
    setVisible(true);
    checkFind();
}

void QFindWidget::setFindText(const QString &text)
{
    m_findEdit->setText(text);
    m_findEdit->setFocus();
}

void QFindWidget::replace_signal()
{
    emit replace(m_replaceEdit->text());
}

void QFindWidget::replaceAndNext_signal()
{
    emit replaceAndNext(m_replaceEdit->text());
}

void QFindWidget::replaceAll_signal()
{
    emit replaceAll(m_replaceEdit->text());
}
