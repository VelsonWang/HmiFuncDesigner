#include "qlistviewwidget.h"
#include "ui_qlistviewwidget.h"
#include "../../../libs/core/qtoolbarbutton.h"

QListViewWidget::QListViewWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QListViewWidget)
{
    ui->setupUi(this);

    QAction *ac = new QAction(QIcon(":/images/removesubmitfield.png"),"",this);
    ac->setToolTip("Remove");
    m_removeButton = new QToolBarButton(ac, this);
    ui->horizontalLayout->addWidget(m_removeButton);
    connect(ac, SIGNAL(triggered()), this, SIGNAL(remove()));

    m_removeButton->setVisible(false);
}

QListViewWidget::~QListViewWidget()
{
    delete ui;
}

void QListViewWidget::set_text(const QString &text)
{
    ui->text->setText(text);
}

void QListViewWidget::set_icon(const QString &icon)
{
    ui->icon->setPixmap(QPixmap(icon));
}

void QListViewWidget::enterEvent(QEvent *)
{
    m_removeButton->setVisible(true);
}

void QListViewWidget::leaveEvent(QEvent *)
{
    m_removeButton->setVisible(false);
}
